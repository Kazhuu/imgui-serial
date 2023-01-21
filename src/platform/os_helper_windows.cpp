#include "config.hpp"
#include "logging.hpp"
#include "platform/os_helper.hpp"
#include <boost/asio/io_service.hpp>
#include <boost/asio/serial_port.hpp>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> OsHelper::get_serial_ports() {
    std::vector<std::string> ports;
    for (int i = 1; i < PORT_COUNT; ++i) {
        std::stringstream device_name;
        device_name << "COM" << i;
        try {
            boost::asio::io_service io_service;
            boost::asio::serial_port serial(io_service, device_name.str());
            ports.emplace_back(device_name.str());
            log_info("Found serial port {}", device_name.str());
        } catch (boost::system::system_error& e) {
            //log_debug("Error opening serial port {}: {}", device_name.str(), e.what());
        }
    }
    return ports;
}
