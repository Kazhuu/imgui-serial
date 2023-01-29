#ifndef SPC_FILE_HPP
#define SPC_FILE_HPP

#include <array>
#include <string>
#include <vector>

const char* const SPC_HEADER = "SNES-SPC700 Sound File Data v0.30";
const size_t HEADER_NAME_SIZE = 33;

const size_t METADATA = 0x23;
const size_t HAS_METADATA = 0x1a;

const size_t TITLE_BEGIN = 0x2e;
const size_t TITLE_SIZE = 32;

const size_t GAME_BEGIN = 0x4e;
const size_t GAME_SIZE = 32;

const size_t COMMENT_BEGIN = 0x7e;
const size_t COMMENT_SIZE = 32;

const size_t LENGTH_BEGIN = 0xa9;
const size_t LENGTH_SIZE = 3;

const size_t HEADER_CHECK_BEGIN = 0x21;
const size_t HEADER_CHECK = 26;

const size_t A_REGISTER = 0x27;
const size_t X_REGISTER = 0x28;
const size_t Y_REGISTER = 0x29;
const size_t PROGRAM_STATUS_WORD = 0x2a;
const size_t STACK_POINTER = 0x2b;
const size_t PROGRAM_COUNTER_LOW = 0x25;
const size_t PROGRAM_COUNTER_HIGH = 0x26;

const size_t DSP_REGISTERS_BEGIN = 0x10100;
const size_t DSP_REGISTER_SIZE = 128;

const size_t FIRST_PAGE_BEGIN = 0x100;
const size_t RAM_FIRST_PAGE_SIZE = 256;

const size_t SECOND_PAGE_BEGIN = 0x200;
const size_t RAM_SECOND_PAGE_SIZE = 256;

const size_t REST_OF_RAM_BEGIN = 0x300;
const size_t REST_OF_RAM_SIZE = 64'960;

class SpcFile {
public:

    explicit SpcFile(const std::string& filename);
    std::string get_title();
    std::string get_game();
    std::string get_comments();
    unsigned int get_length();
    unsigned char get_a_register();
    unsigned char get_x_register();
    unsigned char get_y_register();
    unsigned char get_program_status_word();
    unsigned char get_stack_pointer();
    unsigned short get_program_counter();
    std::array<unsigned char, DSP_REGISTER_SIZE> get_dsp_registers();
    std::array<unsigned char, RAM_FIRST_PAGE_SIZE> get_first_page_ram();
    std::array<unsigned char, RAM_SECOND_PAGE_SIZE> get_second_page_ram();
    std::array<unsigned char, REST_OF_RAM_SIZE> get_rest_of_ram();

private:
    bool is_spc_file();
    bool has_id666_metadata();

    std::vector<unsigned char> m_file_content;
};

#endif
