#include "ui/file_dialog.hpp"
#include "imgui.h"
#include "logging.hpp"
#include "string"
#include "config.hpp"
#include "spc_file.hpp"

#define CUSTOM_IMGUIFILEDIALOG_CONFIG "file_dialog_config.hpp" // NOLINT
#include "ImGuiFileDialog.h"

namespace ui {
void FileDialog::render() {
    ImGui::NewLine();

    if (ImGui::Button("Open File Dialog")) {
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".spc", ".");
    }
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 min_size(viewport->WorkSize.x * FILE_DIALOG_SIZE, viewport->WorkSize.y * FILE_DIALOG_SIZE);
    ImVec2 max_size(viewport->WorkSize);
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse, min_size, max_size)) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filename = ImGuiFileDialog::Instance()->GetFilePathName();
            std::string file_path = ImGuiFileDialog::Instance()->GetCurrentPath();
            log_debug("filename: {}", filename);
            log_debug("file_path: ", file_path);

            SpcFile spc_file(filename);
            log_info("opened file {} from game {}, length {} in seconds", spc_file.get_title(), spc_file.get_game(), spc_file.get_length());
            log_info("song comments: {}", spc_file.get_comments());
        }
        ImGuiFileDialog::Instance()->Close();
    }
}
} // namespace ui
