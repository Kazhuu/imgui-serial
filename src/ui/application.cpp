#include "ui/application.hpp"
#include "imgui.h"
#include "logging.hpp"

namespace ui {

Application::Application(ImGuiBackend& backend) : m_backend(backend) {
    m_backend.register_callback("F1", [this]() { m_show_demo_window = !m_show_demo_window; });
    m_serial_ports = OsHelper::get_serial_ports();
}

void Application::render() {

    if (m_show_demo_window) {
        ImGui::ShowDemoWindow();
    }
    if (ImGui::Button("Refresh")) {
        log_debug("refresh");
        m_serial_ports = OsHelper::get_serial_ports();
    }
    ImGui::SameLine();
    if (!m_serial_ports.empty()) {
        m_current_port_selection = m_serial_ports[m_item_current_idx].c_str();
        if (m_serial.is_open()) {
            if (ImGui::Button("Disconnect")) {
                m_serial.close();
            }
        } else {
            if (ImGui::Button("Connect")) {
                try {
                    m_serial.open(m_current_port_selection, BAUD_RATE);
                    std::array<char, 1> buffer = {'R'};
                    m_serial.write(buffer.data(), 1);
                    log_debug("connect");
                } catch (boost::system::system_error& e) {
                    log_error("error opening serial port: {}", e.what());
                    m_serial_ports.clear();
                }
            }
        }
    }
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * COMPONENT_WIDTH);
    if (ImGui::BeginCombo("Serial Port", m_current_port_selection)) {
        for (size_t n = 0; n < m_serial_ports.size(); ++n) {
            const bool is_selected = (m_item_current_idx == n);
            if (ImGui::Selectable(m_serial_ports[n].c_str(), is_selected)) {
                m_item_current_idx = n;
            }
            // Set the initial focus when opening the combo (scrolling +
            // keyboard navigation focus)
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    m_file_dialog.render();
}

} // namespace ui
