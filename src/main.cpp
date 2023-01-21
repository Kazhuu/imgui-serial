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
        backend.process_keys();
        backend.start_frame();
        app.render();
        backend.end_frame();
    }
    return 0;
}
