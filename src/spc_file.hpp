#ifndef SPC_FILE_HPP
#define SPC_FILE_HPP

#include <string>
#include <vector>
#include <array>

class SpcFile {
public:
    explicit SpcFile(const std::string& filepath);

private:
    std::vector<unsigned char> file_content;
};

#endif
