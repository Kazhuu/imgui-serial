#include "imgui.h"
#include "imgui_backend.hpp"
#include "logging.hpp"
#include "platform/os_helper.hpp"
#include "serial.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <stdio.h>

#define CUSTOM_IMGUIFILEDIALOG_CONFIG "file_dialog_config.hpp"
#include "ImGuiFileDialog.h"

int main(int, char**) {
    Serial serial;
    bool show_demo_window = false;
    std::vector<std::string> serial_ports = OsHelper::get_serial_ports();
    size_t item_current_idx = 0;
    bool done = false;
    const char* current_port_selection = NULL;

    initialize_logging();

    ImGuiBackend backend("SPC Player", [&done]() { done = true; });
    backend.init_imgui();
    backend.register_callback("F1", [&show_demo_window]() { show_demo_window = !show_demo_window; });

    while (!done) {
        if (serial.is_open()) {
            std::vector<char> read_buffer;
            size_t count = serial.read_some(read_buffer, 1);
            if (count > 0) {
                LOG_DEBUG("read: {}, bytes: {}", count, read_buffer.at(0));
            }
        }

        backend.process_keys();
        backend.start_frame();

        // Render components.
        if (show_demo_window) {
            ImGui::ShowDemoWindow();
        }
        if (ImGui::Button("Refresh")) {
            LOG_DEBUG("refresh");
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
                        serial.open(current_port_selection, 38400);
                        char buffer[1] = {'R'};
                        serial.write(buffer, 1);
                        LOG_DEBUG("connect");
                    } catch (boost::system::system_error& e) {
                        LOG_ERROR("error opening serial port: {}", e.what());
                        serial_ports.clear();
                    }
                }
            }
        }
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.10f);
        if (ImGui::BeginCombo("Serial Port", current_port_selection)) {
            for (size_t n = 0; n < serial_ports.size(); ++n) {
                const bool is_selected = (item_current_idx == n);
                if (ImGui::Selectable(serial_ports[n].c_str(), is_selected))
                    item_current_idx = n;

                // Set the initial focus when opening the combo (scrolling +
                // keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
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
                LOG_DEBUG("filename: {}", filename);
                LOG_DEBUG("file_path: ", file_path);
            }
            ImGuiFileDialog::Instance()->Close();
        }

        backend.end_frame();
    }
    return 0;
}
