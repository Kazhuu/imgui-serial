#include "platform/os_helper.hpp"
#include "logging.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <boost/asio/io_service.hpp>
#include <boost/asio/serial_port.hpp>

std::vector<std::string> OsHelper::get_serial_ports() {
    std::vector<std::string> ports;
    for (int i = 1; i < 99; ++i) {
        std::stringstream device_name;
        device_name << "/dev/ttyUSB" << i - 1;
        try {
            boost::asio::io_service io;
            boost::asio::serial_port serial(io, device_name.str());
            ports.emplace_back(device_name.str());
        } catch (boost::system::system_error& e) {
            LOG_ERROR("Error opening serial port {}: {}", device_name.str(), e.what());
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
            LOG_ERROR("Error opening serial port {}: {}", device_name.str(), e.what());
        }
    }
    return ports;
}
