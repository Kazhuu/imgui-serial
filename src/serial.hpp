#ifndef SERIAL_HPP
#define SERIAL_HPP

#include "config.hpp"
#include "platform/os_helper.hpp"
#include <array>
#include <atomic>
#include <boost/asio/io_service.hpp>
#include <boost/asio/serial_port.hpp>
#include <string>
#include <thread>
#include <vector>

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
    /**
     * Read bytes from serial to the given vector the amount of wanted
     * byte_count. Return the amount of read bytes. Returned bytes may be more
     * than the requested bytes.
     */
    size_t read_some(std::vector<char>& buffer, size_t byte_count);

private:
    void read_handler(const boost::system::error_code& error, size_t bytes_transferred);

    bool m_open;
    boost::asio::io_service m_io;
    boost::asio::serial_port m_serial_port;
    boost::system::error_code m_error;
    std::thread m_worker_thread;
    std::mutex m_read_queue_mutex;
    std::atomic_size_t m_read_queue_size;
    std::array<char, READ_QUEUE_SIZE> m_read_queue;
    std::array<char, READ_BUFFER_SIZE> m_read_buffer;
};

#endif
