#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "imgui_backend.hpp"
#include "serial.hpp"
#include "ui/file_dialog.hpp"

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
    Serial m_serial;
    ui::FileDialog m_file_dialog;
};

} // namespace ui

#endif
