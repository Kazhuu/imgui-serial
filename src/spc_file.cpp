#include "spc_file.hpp"
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

SpcFile::SpcFile(const std::string& filename) {
    std::ifstream input(filename, std::ios::binary);
    std::vector<unsigned char> data(std::istreambuf_iterator<char>(input), {});
    file_content.swap(data);
}

std::string SpcFile::get_name() {
    return "";
}

bool SpcFile::is_spc_file() {
    return std::string(file_content.begin(), file_content.begin() + NAME_SIZE) == SPC_HEADER &&
           file_content[0x21] == 26 && file_content[0x22] == 26;
}
