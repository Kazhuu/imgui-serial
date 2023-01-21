#ifndef SPC_PLAYER_HPP
#define SPC_PLAYER_HPP

#include "serial.hpp"
#include <atomic>

class SpcPlayer {
public:
    explicit SpcPlayer(Serial& serial);
    void reset();
    void test_timeout();
    [[nodiscard]] bool is_processing() const;
private:
    Serial& m_serial;
    std::atomic_bool m_reseting;
    std::atomic_bool m_test_timeout;
};

#endif
