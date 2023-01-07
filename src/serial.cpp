#include "serial.hpp"
#include "logging.hpp"
#include <boost/asio/io_service.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/asio/write.hpp>
#include <boost/bind/bind.hpp>
#include <fcntl.h>
#include <functional>
#include <iostream>
#include <sstream>

Serial::Serial() : m_open(false), m_serial_port(m_io), m_read_queue_size(0), m_read_queue(), m_read_buffer() {}

Serial::~Serial() noexcept {
    close();
}

void Serial::open(const std::string& device, uint32_t baud_rate) {
    if (!m_open) {
        m_io.restart();
        m_serial_port.open(device);
        m_serial_port.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
        m_serial_port.async_read_some(boost::asio::buffer(m_read_buffer, READ_BUFFER_SIZE),
                                      [this](auto&& error, auto&& bytes_transferred) {
                                          read_handler(std::forward<decltype(error)>(error),
                                                       std::forward<decltype(bytes_transferred)>(bytes_transferred));
                                      });
        std::thread worker([this]() { m_io.run(); });
        m_worker_thread.swap(worker);
        m_open = true;
    }
}

void Serial::close() {
    if (m_open) {
        try {
            m_serial_port.close();
        } catch (boost::system::system_error& e) {
            log_error("error closing the serial port");
        }
        m_io.stop();
        if (m_worker_thread.joinable()) {
            m_worker_thread.join();
        }
        m_open = false;
    }
}

bool Serial::is_open() const {
    return m_open;
}

void Serial::write(char* buffer, size_t size) {
    boost::asio::write(m_serial_port, boost::asio::buffer(buffer, size));
}

size_t Serial::read_some(std::vector<char>& buffer, size_t byte_count) {
    if (m_read_queue_size >= byte_count) {
        buffer.reserve(m_read_queue_size);
        std::lock_guard<std::mutex> l{m_read_queue_mutex};
        std::copy(std::begin(m_read_queue), std::begin(m_read_queue) + m_read_queue_size, std::back_inserter(buffer));
        size_t copied_bytes = m_read_queue_size;
        m_read_queue_size = 0;
        return copied_bytes;
    }
    return 0;
}

void Serial::read_handler(const boost::system::error_code& error, size_t bytes_transferred) {
    log_debug("reading thread");
    if (!error) {
        if (m_read_queue_size + bytes_transferred < READ_QUEUE_SIZE) {
            std::lock_guard<std::mutex> l{m_read_queue_mutex};
            auto* begin = std::begin(m_read_buffer);
            auto* end = begin;
            std::advance(end, bytes_transferred);
            auto* begin_target = std::begin(m_read_queue) + m_read_queue_size;
            auto* end_target = std::copy(begin, end, begin_target);
            m_read_queue_size = (size_t)(end_target - std::begin(m_read_queue));
            log_debug("bytes transfered: {}", bytes_transferred);
            log_debug("read queue size: ", m_read_queue_size);
        } else {
            log_warning("read queue overflow, discarding data");
        }
        m_serial_port.async_read_some(boost::asio::buffer(m_read_buffer, READ_BUFFER_SIZE),
                                      [this](auto&& error, auto&& bytes_transferred) {
                                          read_handler(std::forward<decltype(error)>(error),
                                                       std::forward<decltype(bytes_transferred)>(bytes_transferred));
                                      });
    } else {
        log_debug("reading thread exit");
    }
}
