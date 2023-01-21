#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "imgui_backend.hpp"
#include "ui/file_dialog.hpp"
#include "spc_player.hpp"

namespace ui {

class Application {
public:
    explicit Application(ImGuiBackend& backend);

    void render();

private:
    ImGuiBackend& m_backend;
    bool m_show_demo_window = false;
    std::vector<std::string> m_serial_ports;
    const char* m_current_port_selection = nullptr;
    size_t m_item_current_idx = 0;
    ui::FileDialog m_file_dialog;
    Serial m_serial;
    SpcPlayer m_spc_player;
};

} // namespace ui

#endif
