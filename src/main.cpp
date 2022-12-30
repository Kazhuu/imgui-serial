#include "platform/os_helper.hpp"
#include "serial.hpp"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include <SDL_opengl.h>
#include <boost/asio.hpp>

int main(int, char**)
{
    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Decide GL+GLSL versions.
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create window with graphics context.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("ImGui Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style.
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends.
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    //Serial serial("/dev/ttyACM0", 38400);
    bool show_demo_window = false;
    std::vector<std::string> serial_ports = OsHelper::get_serial_ports();
    size_t item_current_idx = 0;
    bool done = false;
    while (!done)
    {
        // TODO: Move this elsewhere.
        //std::vector<char> read_buffer;
        //size_t count = serial.read(read_buffer, 1);
        // if (count > 0) {
        //     std::cout << "read: " << count << " bytes: " << read_buffer.at(0) << "\n" << std::endl;
        // }

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT
                    && event.window.event == SDL_WINDOWEVENT_CLOSE
                    && event.window.windowID == SDL_GetWindowID(window))
                done = true;
            if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_F1:
                        show_demo_window = !show_demo_window;
                        break;
                    default:
                        break;
                }
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window) {
            ImGui::ShowDemoWindow();
        } else {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);

            ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
            ImGui::Begin("IMGUI Test", NULL, flags);

            // Render components.
            const char* current_port_selection = NULL;
            if (!serial_ports.empty()) {
                current_port_selection = serial_ports[item_current_idx].c_str();
            }
            if (ImGui::Button("Refresh")) {
                std::cout << "refresh" << std::endl;
                serial_ports = OsHelper::get_serial_ports();
            }
            ImGui::SameLine();
            if (ImGui::Button("Connect")) {
                //char buffer[1] = {'R'};
                //serial.write(buffer, 1);
                std::cout << "connect" << std::endl;
            }
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.33f);
            if (ImGui::BeginCombo("Serial Port", current_port_selection))
            {
                for (size_t n = 0; n < serial_ports.size(); ++n)
                {
                    const bool is_selected = (item_current_idx == n);
                    if (ImGui::Selectable(serial_ports[n].c_str(), is_selected))
                        item_current_idx = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
