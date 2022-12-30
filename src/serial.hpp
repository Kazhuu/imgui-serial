#ifndef SERIAL_HPP
#define SERIAL_HPP

#include "platform/os_helper.hpp"
#include <boost/asio/io_service.hpp>
#include <boost/asio/serial_port.hpp>
#include <string>
#include <thread>
#include <vector>
#include <atomic>

class Serial {
   public:
    static const size_t READ_BUFFER_SIZE = 500;
    static const size_t READ_QUEUE_SIZE = 1000;

    Serial();
    virtual ~Serial();

    Serial(const Serial&) = delete;
    Serial& operator=(const Serial&) = delete;
    Serial(Serial&& other) = delete;
    Serial& operator=(Serial&&) = delete;

    void open(std::string device, uint32_t baud_rate);
    void close();
    bool is_open();
    void write(char buffer[], size_t size);
    /**
     * Read bytes from serial to the given vector the amount of wanted
     * byte_count. Return the amount of read bytes. Returned bytes may be more
     * than the requested bytes.
    */
    size_t read_some(std::vector<char>& buffer, size_t byte_count);

   private:
    void read_handler(const boost::system::error_code& error,
                      size_t bytes_transferred);

    bool m_open;
    boost::asio::io_service m_io;
    boost::asio::serial_port m_serial_port;
    boost::system::error_code m_error;
    std::thread m_worker_thread;
    std::mutex m_read_queue_mutex;
    std::atomic_size_t m_read_queue_size;
    char m_read_queue[READ_QUEUE_SIZE];
    char m_read_buffer[READ_BUFFER_SIZE];
};

#endif
