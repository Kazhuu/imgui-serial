#include "imgui.h"
#include "imgui_backend.hpp"
#include "logging.hpp"
#include "platform/os_helper.hpp"
#include "serial.hpp"
#include <boost/asio.hpp>
#include <cstdio>
#include <iostream>

#define CUSTOM_IMGUIFILEDIALOG_CONFIG "file_dialog_config.hpp" // NOLINT
#include "ImGuiFileDialog.h"

const int BAUD_RATE = 38400;
const float COMPONENT_WIDTH = 0.10F;
const int RESOLUTION_WIDTH = 1280;
const int RESOLUTION_HEIGHT = 720;

int main(int /*argc*/, char** /*varg*/) { // NOLINT
    initialize_logging();

    Serial serial;
    bool show_demo_window = false;
    std::vector<std::string> serial_ports = OsHelper::get_serial_ports();
    size_t item_current_idx = 0;
    bool done = false;
    const char* current_port_selection = nullptr;

    ImGuiBackend backend("SPC Player", [&done]() { done = true; });
    backend.init_imgui(RESOLUTION_WIDTH, RESOLUTION_HEIGHT);
    backend.register_callback("F1", [&show_demo_window]() { show_demo_window = !show_demo_window; });

    while (!done) {
        if (serial.is_open()) {
            std::vector<char> read_buffer;
            size_t count = serial.read_some(read_buffer, 1);
            if (count > 0) {
                log_debug("read: {}, bytes: {}", count, read_buffer.at(0));
            }
        }

        backend.process_keys();
        backend.start_frame();

        // Render components.
        if (show_demo_window) {
            ImGui::ShowDemoWindow();
        }
        if (ImGui::Button("Refresh")) {
            log_debug("refresh");
            serial_ports = OsHelper::get_serial_ports();
        }
        ImGui::SameLine();
        if (!serial_ports.empty()) {
            current_port_selection = serial_ports[item_current_idx].c_str();
            if (serial.is_open()) {
                if (ImGui::Button("Disconnect")) {
                    serial.close();
                }
            } else {
                if (ImGui::Button("Connect")) {
                    try {
                        serial.open(current_port_selection, BAUD_RATE);
                        std::array<char, 1> buffer = {'R'};
                        serial.write(buffer.data(), 1);
                        log_debug("connect");
                    } catch (boost::system::system_error& e) {
                        log_error("error opening serial port: {}", e.what());
                        serial_ports.clear();
                    }
                }
            }
        }
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * COMPONENT_WIDTH);
        if (ImGui::BeginCombo("Serial Port", current_port_selection)) {
            for (size_t n = 0; n < serial_ports.size(); ++n) {
                const bool is_selected = (item_current_idx == n);
                if (ImGui::Selectable(serial_ports[n].c_str(), is_selected)) {
                    item_current_idx = n;
                }
                // Set the initial focus when opening the combo (scrolling +
                // keyboard navigation focus)
                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        ImGui::NewLine();

        if (ImGui::Button("Open File Dialog")) {
            ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".spc", ".");
        }
        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string filename = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string file_path = ImGuiFileDialog::Instance()->GetCurrentPath();
                log_debug("filename: {}", filename);
                log_debug("file_path: ", file_path);
            }
            ImGuiFileDialog::Instance()->Close();
        }

        backend.end_frame();
    }
    return 0;
}
