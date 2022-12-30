#include "serial.hpp"

#include <fcntl.h>
#include <boost/asio/io_service.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/asio/write.hpp>
#include <boost/bind/bind.hpp>
#include <functional>
#include <iostream>
#include <sstream>

Serial::Serial()
    : m_open(false), m_io(), m_serial_port(m_io), m_read_queue_size(0) {}

Serial::~Serial() { close(); }

void Serial::open(std::string device, uint32_t baud_rate) {
    if (!m_open) {
        m_io.restart();
        m_serial_port.open(device);
        m_serial_port.set_option(
            boost::asio::serial_port_base::baud_rate(baud_rate));
        m_serial_port.async_read_some(
            boost::asio::buffer(m_read_buffer, READ_BUFFER_SIZE),
            std::bind(&Serial::read_handler, this, std::placeholders::_1,
                      std::placeholders::_2));
        std::thread worker([this]() { m_io.run(); });
        m_worker_thread.swap(worker);
        m_open = true;
    }
}

void Serial::close() {
    if (m_open) {
        m_serial_port.close();
        m_io.stop();
        if (m_worker_thread.joinable()) {
            m_worker_thread.join();
        }
        m_open = false;
    }
}

bool Serial::is_open() { return m_open; }

void Serial::write(char buffer[], size_t size) {
    boost::asio::write(m_serial_port, boost::asio::buffer(buffer, size));
}

size_t Serial::read_some(std::vector<char>& buffer, size_t byte_count) {
    if (m_read_queue_size >= byte_count) {
        buffer.reserve(m_read_queue_size);
        std::lock_guard<std::mutex> l{m_read_queue_mutex};
        std::copy(std::begin(m_read_queue),
                  std::begin(m_read_queue) + m_read_queue_size,
                  std::back_inserter(buffer));
        size_t copied_bytes = m_read_queue_size;
        m_read_queue_size = 0;
        return copied_bytes;
    }
    return 0;
}

void Serial::read_handler(const boost::system::error_code& error,
                          size_t bytes_transferred) {
    std::cout << "reading thread\n" << std::endl;
    if (!error) {
        if (m_read_queue_size + bytes_transferred < READ_QUEUE_SIZE) {
            std::lock_guard<std::mutex> l{m_read_queue_mutex};
            auto begin = std::begin(m_read_buffer);
            auto end = begin + bytes_transferred;
            auto begin_target = std::begin(m_read_queue) + m_read_queue_size;
            auto end_target = std::copy(begin, end, begin_target);
            m_read_queue_size = end_target - std::begin(m_read_queue);
            std::cout << bytes_transferred << " bytes transfered\n"
                      << std::endl;
            std::cout << "read queue size: " << m_read_queue_size << std::endl;
        } else {
            std::cout << "read queue overflow, discarding data\n" << std::endl;
        }
        m_serial_port.async_read_some(
            boost::asio::buffer(m_read_buffer, READ_BUFFER_SIZE),
            std::bind(&Serial::read_handler, this, std::placeholders::_1,
                      std::placeholders::_2));
    } else {
        std::cout << "exit\n" << std::endl;
    }
}
