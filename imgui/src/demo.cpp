// ~begin-doc
// This code is stolen from [redblobgames](https://github.com/redblobgames/imgui)'s
// fork of ImGui (specifically,
// [this file](https://github.com/redblobgames/imgui/blob/master/examples/sdl_emscripten_example/main.cpp)),
// with mild updates to get the code to compile with my stuff.

// ImGui SDL2 binding with OpenGL ES 2, compatible with WebGL when compiled with Emscripten
// https://github.com/ocornut/imgui


// ImGui - standalone example application for SDL2 + OpenGL ES 2 + Emscripten

#include <lp3/imgui.hpp>
#include <imgui.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <lp3/main.hpp>


namespace core = lp3::core;
namespace gfx = lp3::gfx;
namespace sdl = lp3::sdl;


int _main(lp3::main::PlatformLoop & loop) {
    core::LogSystem log;
    sdl::SDL2 sdl2(SDL_INIT_VIDEO);

    LP3_LOG_DEBUG("Initializing Window");
	gfx::Window window("Lp3 ImGui Demo", glm::vec2{ 1280, 720 });

    //// TODO: Could these move to after the window is created?
    //SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    //SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    //SDL_DisplayMode current;
    //SDL_GetCurrentDisplayMode(0, &current);

    /*sdl::GLContext gl_context = SDL_GL_CreateContext(window);
    if (SDL_GL_MakeCurrent(window, gl_context) < 0) {
        LP3_LOG_ERROR("Error creating GL %s", SDL_GetError());
    }
    lp3::gl::initialize();*/

    LP3_LOG_DEBUG("Initializing ImGuiApp");
    lp3::imgui::ImGuiApp imgui(window);

    bool g_show_test_window = true;
    bool g_show_another_window = false;

    return loop.run([&]() {
        const ImVec4 g_clear_color = ImColor(114, 144, 154);
        bool g_done = false;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            imgui.process_event(event);
            if (event.type == SDL_QUIT)
                g_done = true;
        }
        imgui.new_frame(window);

        // 1. Show a simple window
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
        {
            static float f = 0.0f;
            ImGui::Text("Hello, world!");
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&g_clear_color);
            if (ImGui::Button("Test Window")) g_show_test_window ^= 1;
            if (ImGui::Button("Another Window")) g_show_another_window ^= 1;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        // 2. Show another simple window, this time using an explicit Begin/End pair
        if (g_show_another_window)
        {
            ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Another Window", &g_show_another_window);
            ImGui::Text("Hello");
            ImGui::End();
        }

        // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
        if (g_show_test_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
            ImGui::ShowDemoWindow(&g_show_test_window);
        }

        // Rendering
        glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClearColor(g_clear_color.x, g_clear_color.y, g_clear_color.z, g_clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

		window.render([](const glm::mat4 &) {
			ImGui::Render();
		});

        return !g_done;
    });
}

LP3_MAIN(_main)
// ~end-doc
