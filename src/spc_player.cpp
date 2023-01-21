#include "spc_player.hpp"
#include "logging.hpp"
#include <array>

SpcPlayer::SpcPlayer(Serial& serial) : m_serial(serial), m_reseting(false), m_test_timeout(false) {}

void SpcPlayer::reset() {
    if (m_reseting) {
        return;
    }
    m_reseting = true;
    std::array<char, 1> buffer = {'R'};
    m_serial.write(buffer.data(), buffer.size());
    callback_t callback = [this](const std::vector<char>& buffer, bool timeout) {
        log_debug("read callback called with length: {}, timeout: {}", buffer.size(), timeout);
        if (timeout) {
            log_error("reset timeout");
        } else {
            char result = buffer.at(0);
            if (result == '1') {
                log_info("reset successful");
            } else {
                log_error("reset error, result: {}", result);
                // TODO: Do something with the error state.
            }
        }
        m_reseting = false;
    };
    m_serial.read_async(1, callback, RESET_TIMEOUT_MS);
}

bool SpcPlayer::is_processing() const {
    return m_reseting || m_test_timeout;
}

void SpcPlayer::test_timeout() {
    if (m_test_timeout) {
        return;
    }
    m_test_timeout = true;
    std::array<char, 1> buffer = {'b'};
    m_serial.write(buffer.data(), buffer.size());
    callback_t callback = [this](const std::vector<char>& buffer, bool timeout) {
        log_debug("read callback called with length: {}, timeout: {}", buffer.size(), timeout);
        if (timeout) {
            log_error("timeout happened");
        } else {
            log_info("timeout did not timeout");
        }
        m_test_timeout = false;
    };
    m_serial.read_async(1, callback, 3000);
}
