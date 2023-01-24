#ifndef SPC_FILE_HPP
#define SPC_FILE_HPP

#include <string>
#include <vector>
#include <array>

class SpcFile {
public:

    explicit SpcFile(const std::string& filename);
    std::string get_name();
    bool is_spc_file();

private:

    const char* SPC_HEADER = "SNES-SPC700 Sound File Data v0.30";
    static const size_t NAME_SIZE = 33;

    std::vector<unsigned char> file_content;
};

#endif
