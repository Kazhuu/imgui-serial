#include "imgui_backend.hpp"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "logging.hpp"
#include <SDL.h>
#include <SDL_opengl.h>
#include <utility>

ImGuiBackend::ImGuiBackend(std::string app_name, std::function<void()> quit_callback)
    : _window(nullptr), _gl_context(), _app_name(std::move(app_name)), _quit_callback(std::move(quit_callback)) {}

ImGuiBackend::~ImGuiBackend() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(_gl_context);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void ImGuiBackend::init_imgui(int width, int height) {
    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling
    // issues on a minority of Windows systems, depending on whether
    // SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest
    // version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        log_error("Error: {}", SDL_GetError());
        std::terminate();
    }
    // Decide GL+GLSL versions.
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create window with graphics context.
    const int DOUBLE_BUFFER = 1;
    const int DEPTH_BUFFER_SIZE = 24;
    const int STENCIL_BUFFER_SIZE = 8;
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, DOUBLE_BUFFER);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, DEPTH_BUFFER_SIZE);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, STENCIL_BUFFER_SIZE);
    auto window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    _window = SDL_CreateWindow(_app_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
                               window_flags);
    _gl_context = SDL_GL_CreateContext(_window);
    SDL_GL_MakeCurrent(_window, _gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Dear ImGui style.
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends.
    ImGui_ImplSDL2_InitForOpenGL(_window, _gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void ImGuiBackend::start_frame() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus;
    ImGui::Begin(_app_name.c_str(), nullptr, flags);
}

void ImGuiBackend::end_frame() {
    ImGui::End();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(_window);
}

void ImGuiBackend::process_keys() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) {
            _quit_callback();
        }
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
            event.window.windowID == SDL_GetWindowID(_window)) {
            _quit_callback();
        }
        if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
            case SDLK_F1:
                execute_callback("F1");
                break;
            default:
                break;
            }
        }
    }
}

void ImGuiBackend::register_callback(const std::string& key, const std::function<void()>& callback) {
    _callbacks.insert({key, callback});
}

void ImGuiBackend::execute_callback(const std::string& key) {
    _callbacks.at(key)();
}
