#include "spc_file.hpp"
#include <algorithm>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

SpcFile::SpcFile(const std::string& filename) {
    std::ifstream input(filename, std::ios::binary);
    std::vector<unsigned char> data(std::istreambuf_iterator<char>(input), {});
    m_file_content.swap(data);
    if (!is_spc_file()) {
        throw std::invalid_argument("invalid SPC file");
    }
}

std::string SpcFile::get_title() {
    auto begin = m_file_content.begin() + TITLE_BEGIN;
    return {begin, begin + TITLE_SIZE};
}

std::string SpcFile::get_game() {
    auto begin = m_file_content.begin() + GAME_BEGIN;
    return {begin, begin + GAME_SIZE};
}

std::string SpcFile::get_comments() {
    auto begin = m_file_content.begin() + COMMENT_BEGIN;
    return {begin, begin + COMMENT_SIZE};
}

unsigned int SpcFile::get_length() {
    auto begin = m_file_content.begin() + LENGTH_BEGIN;
    const std::string length = std::string(begin, begin + LENGTH_SIZE);
    return (unsigned int)(std::stoul(length));
}

unsigned char SpcFile::get_a_register() {
    return m_file_content[A_REGISTER];
}

unsigned char SpcFile::get_x_register() {
    return m_file_content[X_REGISTER];
}

unsigned char SpcFile::get_y_register() {
    return m_file_content[Y_REGISTER];
}

unsigned char SpcFile::get_program_status_word() {
    return m_file_content[PROGRAM_STATUS_WORD];
}

unsigned char SpcFile::get_stack_pointer() {
    return m_file_content[STACK_POINTER];
}

unsigned short SpcFile::get_program_counter() {
    return (unsigned short)((m_file_content[PROGRAM_COUNTER_LOW] << 8) | m_file_content[PROGRAM_COUNTER_HIGH]);
}

std::array<unsigned char, DSP_REGISTER_SIZE> SpcFile::get_dsp_registers() {
    std::array<unsigned char, DSP_REGISTER_SIZE> registers{};
    std::copy_n(m_file_content.begin() + DSP_REGISTERS_BEGIN, DSP_REGISTER_SIZE, registers.begin());
    return registers;
}

std::array<unsigned char, RAM_FIRST_PAGE_SIZE> SpcFile::get_first_page_ram() {
    std::array<unsigned char, RAM_FIRST_PAGE_SIZE> ram{};
    std::copy_n(m_file_content.begin() + FIRST_PAGE_BEGIN, RAM_FIRST_PAGE_SIZE, ram.begin());
    return ram;
}

std::array<unsigned char, RAM_SECOND_PAGE_SIZE> SpcFile::get_second_page_ram() {
    std::array<unsigned char, RAM_SECOND_PAGE_SIZE> ram{};
    std::copy_n(m_file_content.begin() + SECOND_PAGE_BEGIN, RAM_SECOND_PAGE_SIZE, ram.begin());
    return ram;
}

std::array<unsigned char, REST_OF_RAM_SIZE> SpcFile::get_rest_of_ram() {
    std::array<unsigned char, REST_OF_RAM_SIZE> ram{};
    std::copy_n(m_file_content.begin() + REST_OF_RAM_BEGIN, REST_OF_RAM_SIZE, ram.begin());
    return ram;
}

bool SpcFile::is_spc_file() {
    return std::string(m_file_content.begin(), m_file_content.begin() + HEADER_NAME_SIZE) == SPC_HEADER &&
           m_file_content[HEADER_CHECK_BEGIN] == HEADER_CHECK && m_file_content[HEADER_CHECK_BEGIN + 1] == HEADER_CHECK;
}

bool SpcFile::has_id666_metadata() {
    return m_file_content[METADATA] == HAS_METADATA;
}
