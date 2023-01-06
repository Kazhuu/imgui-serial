#ifndef IMGUI_BACKEND_HPP
#define IMGUI_BACKEND_HPP

#include <SDL.h>
#include <functional>
#include <map>
#include <string>

class ImGuiBackend {
public:
    ImGuiBackend(std::string app_name, std::function<void()> quit_callback);
    ~ImGuiBackend();
    void init_imgui();
    void start_frame();
    void end_frame();
    void process_keys();
    void register_callback(std::string key, std::function<void()> callback);

private:
    void execute_callback(std::string key);

    SDL_Window* _window;
    SDL_GLContext _gl_context;
    std::string _app_name;
    std::function<void()> _quit_callback;
    std::map<std::string, std::function<void()>> _callbacks;
};

#endif
