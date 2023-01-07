#include "logging.hpp"
#include "platform/os_helper.hpp"
#include <boost/asio/io_service.hpp>
#include <boost/asio/serial_port.hpp>
#include <iostream>
#include <string>
#include <vector>

const int PORT_COUNT = 99;

std::vector<std::string> OsHelper::get_serial_ports() {
    std::vector<std::string> ports;
    for (int i = 1; i < PORT_COUNT; ++i) {
        std::stringstream device_name;
        device_name << "/dev/ttyUSB" << i - 1;
        try {
            boost::asio::io_service io_service;
            boost::asio::serial_port serial(io_service, device_name.str());
            ports.emplace_back(device_name.str());
        } catch (boost::system::system_error& e) {
            log_error("Error opening serial port {}: {}", device_name.str(), e.what());
        }
    }
    for (int i = 1; i < PORT_COUNT; ++i) {
        std::stringstream device_name;
        device_name << "/dev/ttyACM" << i - 1;
        try {
            boost::asio::io_service io_service;
            boost::asio::serial_port serial(io_service, device_name.str());
            ports.emplace_back(device_name.str());
        } catch (boost::system::system_error& e) {
            log_error("Error opening serial port {}: {}", device_name.str(), e.what());
        }
    }
    return ports;
}
