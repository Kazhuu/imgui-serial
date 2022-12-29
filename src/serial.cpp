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

Serial::Serial(std::string device, uint32_t baud_rate)
    : io(), serial_port(io, device), worker_thread(), read_queue_size(0) {
    serial_port.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    serial_port.async_read_some(
        boost::asio::buffer(read_buffer, READ_BUFFER_SIZE),
        std::bind(&Serial::read_handler, this, std::placeholders::_1,
                  std::placeholders::_2));
    std::thread worker([this]() { io.run(); });
    worker_thread.swap(worker);
}

Serial::~Serial() {
    serial_port.close();
    io.stop();
    worker_thread.join();
}

void Serial::write(char buffer[], size_t size) {
    boost::asio::write(serial_port, boost::asio::buffer(buffer, size));
}

size_t Serial::read(std::vector<char>& buffer, size_t byte_count) {
    if (read_queue_size >= byte_count) {
        buffer.reserve(read_queue_size);
        std::lock_guard<std::mutex> l{read_queue_mutex};
        std::copy(std::begin(read_queue), std::begin(read_queue) + read_queue_size, std::back_inserter(buffer));
        size_t copied_bytes = read_queue_size;
        read_queue_size = 0;
        return copied_bytes;
    }
    return 0;
}

void Serial::read_handler(const boost::system::error_code& error,
                          size_t bytes_transferred) {
    std::cout << "reading thread\n" << std::endl;
    if (!error) {
        if (read_queue_size + bytes_transferred < READ_QUEUE_SIZE) {
            std::lock_guard<std::mutex> l{read_queue_mutex};
            auto begin = std::begin(read_buffer);
            auto end = begin + bytes_transferred;
            auto begin_target = std::begin(read_queue) + read_queue_size;
            auto end_target = std::copy(begin, end, begin_target);
            read_queue_size = end_target - std::begin(read_queue);
            std::cout << bytes_transferred << " bytes transfered\n" << std::endl;
            std::cout << "read queue size: " << read_queue_size << std::endl;
        } else {
            std::cout << "read queue overflow, discarding data\n" << std::endl;
        }
        serial_port.async_read_some(
                boost::asio::buffer(read_buffer, READ_BUFFER_SIZE),
                std::bind(&Serial::read_handler, this, std::placeholders::_1,
                    std::placeholders::_2));
    } else {
        std::cout << "exit\n" << std::endl;
    }
}

std::vector<std::string> Serial::get_serial_ports() {
    std::vector<std::string> ports;
#if defined(_WIN32) || defined(_WIN64)
    for (int i = 1; i < 99; ++i) {
        std::stringstream device_name;
        device_name << "COM" << i;
        try {
            boost::asio::io_service io;
            boost::asio::serial_port serial(io, device_name.str());
            ports.emplace_back(device_name.str());
        } catch (boost::system::system_error& e) {
            std::cout << "Error opening serial port " << device_name.str() << ": "
                      << e.what() << "\n" << std::endl;
        }
    }
#elif defined(__linux__)
    for (int i = 1; i < 99; ++i) {
        std::stringstream device_name;
        device_name << "/dev/ttyUSB" << i - 1;
        try {
            boost::asio::io_service io;
            boost::asio::serial_port serial(io, device_name.str());
            ports.emplace_back(device_name.str());
        } catch (boost::system::system_error& e) {
            std::cout << "Error opening serial port " << device_name.str()
                      << ": " << e.what() << "\n" << std::endl;
        }
    }
    for (int i = 1; i < 99; ++i) {
        std::stringstream device_name;
        device_name << "/dev/ttyACM" << i - 1;
        try {
            boost::asio::io_service io;
            boost::asio::serial_port serial(io, device_name.str());
            ports.emplace_back(device_name.str());
        } catch (boost::system::system_error& e) {
            std::cout << "Error opening serial port " << device_name.str()
                      << ": " << e.what() << "\n" << std::endl;
        }
    }
#else
    assert(!"Cannot read serial ports, not a supported platform.");
#endif
    return ports;
}
