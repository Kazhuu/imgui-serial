#ifndef SERIAL_HPP
#define SERIAL_HPP

#include "config.hpp"
#include "platform/os_helper.hpp"
#include <array>
#include <atomic>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/date_time/time_duration.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <string>
#include <thread>
#include <vector>

using callback_t = std::function<void(std::vector<char>&, bool)>;

class Serial {
public:
    Serial();
    virtual ~Serial() noexcept;

    Serial(const Serial&) = delete;
    Serial& operator=(const Serial&) = delete;
    Serial(Serial&& other) = delete;
    Serial& operator=(Serial&&) = delete;

    void open(const std::string& device, uint32_t baud_rate);
    void close();
    [[nodiscard]] bool is_open() const;
    void write(char* buffer, size_t size);
    void read_async(size_t requested_bytes, callback_t& callback, unsigned int duration_ms);

private:
    void read_handler(const boost::system::error_code& error, size_t bytes_transferred);
    void timeout_handler(const boost::system::error_code& error);
    void execute_callback(size_t bytes_transferred, bool timeout);

    using work_guard_t = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;

    bool m_open;
    boost::asio::io_service m_io;
    boost::asio::serial_port m_serial_port;
    boost::asio::deadline_timer m_timer;
    work_guard_t m_work_guard;
    std::thread m_worker_thread;
    std::atomic_size_t m_read_queue_size;
    std::array<char, READ_BUFFER_SIZE> m_read_buffer;
    std::mutex m_callback_mutex;
    callback_t m_user_read_callback;
};

#endif
