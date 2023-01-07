#include "imgui_backend.hpp"
#include "logging.hpp"
#include "ui/application.hpp"

int main(int /*argc*/, char** /*varg*/) {
    initialize_logging();

    bool done = false;

    ImGuiBackend backend(APP_NAME, [&done]() { done = true; });
    backend.init_imgui(RESOLUTION_WIDTH, RESOLUTION_HEIGHT);
    ui::Application app(backend);

    while (!done) {
        // if (serial.is_open()) {
        // std::vector<char> read_buffer;
        // size_t count = serial.read_some(read_buffer, 1);
        // if (count > 0) {
        // log_debug("read: {}, bytes: {}", count, read_buffer.at(0));
        //}
        //}

        backend.process_keys();
        backend.start_frame();

        app.render();

        backend.end_frame();
    }
    return 0;
}
