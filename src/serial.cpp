#include "serial.hpp"
#include <boost/asio/io_service.hpp>
#include <boost/asio/serial_port.hpp>
#include <fcntl.h>
#include <iostream>
#include <sstream>

std::vector<std::string> Serial::getSerialPorts() {
    std::vector<std::string> ports;
    for (int i=1;i<99;i++)
    {
        std::stringstream device_name;
#if defined (_WIN32) || defined( _WIN64)
        device_name << "COM" << i;
#elif __linux__
        device_name << "/dev/ttyUSB" << i-1;
#else
        assert false;
#endif
        auto device = device_name.str();
        try {
            boost::asio::io_service io;
            boost::asio::serial_port serial(io, device);
            ports.emplace_back(device);
        } catch (boost::system::system_error &e) {
            std::cout << "Error opening serial port " << device << ": " << e .what() << std::endl;
        }
    }
    return ports;
}
