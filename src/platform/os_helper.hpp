#ifndef OS_HELPER_HPP
#define OS_HELPER_HPP

#include <string>
#include <vector>

class OsHelper {
public:
    static std::vector<std::string> get_serial_ports();
};

#endif
