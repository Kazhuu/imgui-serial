#ifndef OS_HELPER_HPP
#define OS_HELPER_HPP

#include <vector>
#include <string>

class OsHelper {
public:
    static std::vector<std::string> get_serial_ports();
};

#endif
