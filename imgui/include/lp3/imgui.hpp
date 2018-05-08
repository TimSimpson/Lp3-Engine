#ifndef FILE_LP3_IMGUI_HPP
#define FILE_LP3_IMGUI_HPP
#pragma once

#include <lp3/gfx.hpp>
#include <lp3/imgui/config.hpp>
#include <imgui.h>

// TODO: change to something nicer.

// Use if you want to reset your rendering device without losing ImGui state.
LP3_IMGUI_API void        ImGui_ImplSdl_InvalidateDeviceObjects();
LP3_IMGUI_API bool        ImGui_ImplSdl_CreateDeviceObjects();

namespace lp3 { namespace imgui {

// ---------------------------------------------------------------------------
// class ImGuiApp
// ---------------------------------------------------------------------------
//     Handles global start up / shut down of ImGui stuff.
// ---------------------------------------------------------------------------
LP3_IMGUI_API
class ImGuiApp {
public:
    ImGuiApp(::lp3::gfx::Window & window);
    ~ImGuiApp();

    void operator() (const glm::mat4 & previous);

    static void new_frame(::lp3::gfx::Window & window);
    static bool process_event(SDL_Event & event);

};
// ~end-doc

// ---------------------------------------------------------------------------
// make_imvec2
// ---------------------------------------------------------------------------
//     Translates a glm or other style vector to ImVec2.
// ---------------------------------------------------------------------------
template<typename Vec>
ImVec2 make_imvec2(const Vec & v) {
    return ImVec2(lp3::narrow<float>(v.x), lp3::narrow<float>(v.y));
}
// ~end-doc

}   }

#endif
