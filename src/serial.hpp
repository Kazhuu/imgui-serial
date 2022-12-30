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
    static std::vector<std::string> get_serial_ports();

    Serial(std::string device, uint32_t baud_rate);
    virtual ~Serial();

    Serial(const Serial&) = delete;
    Serial(Serial&& other) = delete;
    Serial& operator=(const Serial&) = delete;
    Serial& operator=(Serial&&) = delete;

    void write(char buffer[], size_t size);
    /**
     * Read bytes from serial to the given vector the amount of wanted
     * byte_count. Return the amount of read bytes. Returned bytes may be more
     * than the requested bytes.
    */
    size_t read(std::vector<char>& buffer, size_t byte_count);

   private:
    void read_handler(const boost::system::error_code& error,
                      size_t bytes_transferred);

    boost::asio::io_service io;
    boost::asio::serial_port serial_port;
    boost::system::error_code error;
    std::thread worker_thread;
    std::mutex read_queue_mutex;
    std::atomic_size_t read_queue_size;
    char read_queue[READ_QUEUE_SIZE];
    char read_buffer[READ_BUFFER_SIZE];
};
