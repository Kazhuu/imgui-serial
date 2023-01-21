#include "serial.hpp"
#include "logging.hpp"
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <boost/bind/bind.hpp>
#include <fcntl.h>
#include <functional>
#include <mutex>

Serial::Serial()
    : m_open(false), m_serial_port(m_io), m_timer(m_io), m_work_guard(make_work_guard(m_io)), m_read_queue_size(0),
      m_read_buffer() {}

Serial::~Serial() noexcept {
    close();
}

void Serial::open(const std::string& device, uint32_t baud_rate) {
    if (!m_open) {
        m_io.restart();
        m_serial_port.open(device);
        m_serial_port.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
        std::thread worker([this]() { m_io.run(); });
        m_worker_thread.swap(worker);
        m_open = true;
        log_info("opened serial port {}", device);
    }
}

void Serial::close() {
    if (m_open) {
        try {
            log_debug("closing serial port");
            m_serial_port.close();
            log_info("successful closed serial port");
        } catch (boost::system::system_error& e) {
            log_error("error closing the serial port");
        }
        m_io.stop();
        if (m_worker_thread.joinable()) {
            m_worker_thread.join();
            log_debug("successful joined worker thread");
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

void Serial::read_async(size_t requested_bytes, callback_t& callback, unsigned int duration_ms) {
    log_debug("read_async requested to read {} bytes", requested_bytes);
    {
        std::lock_guard<std::mutex> l{m_callback_mutex};
        m_user_read_callback = callback;
    }

    boost::asio::async_read(m_serial_port, boost::asio::buffer(m_read_buffer, requested_bytes),
                            [this](auto&& error, auto&& bytes_transferred) {
                                read_handler(std::forward<decltype(error)>(error),
                                             std::forward<decltype(bytes_transferred)>(bytes_transferred));
                            });
    m_timer.expires_from_now(boost::posix_time::milliseconds(duration_ms));
    m_timer.async_wait([this](const boost::system::error_code& error) { timeout_handler(error); });
}

void Serial::read_handler(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
        log_debug("read_handler not errors, got {} bytes", bytes_transferred);
        execute_callback(bytes_transferred, false);
    } else {
        log_debug("reading_handler error: {}", error.what());
    }
}

void Serial::timeout_handler(const boost::system::error_code& error) {
    log_error("timeout handler called: {}", error.what());
    if (!error) {
        log_error("timer expired: {}", error.what());
        execute_callback(0, true);
    } else {
        log_error("timeout_handler error: {}", error.what());
    }
}

void Serial::execute_callback(size_t bytes_transferred, bool timeout) {
    callback_t callback_copy;
    {
        std::lock_guard<std::mutex> l{m_callback_mutex};
        callback_copy = m_user_read_callback;
        m_user_read_callback = nullptr;
    }
    if (callback_copy) {
        std::vector<char> local_buffer;
        if (bytes_transferred != 0) {
            local_buffer.reserve(bytes_transferred);
            {
                std::copy(std::begin(m_read_buffer), std::begin(m_read_buffer) + bytes_transferred,
                        std::back_inserter(local_buffer));
            }
        }
        log_debug("calling user provided callback with buffer size {}", local_buffer.size());
        callback_copy(local_buffer, timeout);
    }
}
