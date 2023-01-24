#include "spc_file.hpp"
#include <string>
#include <fstream>
#include <iterator>
#include <vector>

SpcFile::SpcFile(const std::string& filepath) {
    std::ifstream input(filepath, std::ios::binary);
    std::vector<unsigned char> data(std::istreambuf_iterator<char>(input), {});
    file_content.swap(data);
}
