#include <sstream>
#include <boost/format.hpp>
#include <lp3/core.hpp>
#include <lp3/gfx.hpp>
#include <lp3/imgui.hpp>
#include <lp3/input.hpp>
#include <lp3/sims.hpp>
#include <lp3/main.hpp>

namespace core = lp3::core;
namespace gfx = lp3::gfx;
namespace input = lp3::input;
namespace sdl = lp3::sdl;
namespace sims = lp3::sims;

int _main(lp3::main::PlatformLoop & loop) {
    core::LogSystem log;
    sdl::SDL2 sdl2(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK| SDL_INIT_GAMECONTROLLER);

    LP3_LOG_DEBUG("Setting up input.");
	input::Controls controls;
    core::MediaManager media;

    gfx::Window window("Lp3 Input", glm::vec2{ 427, 240 });

    LP3_LOG_DEBUG("Initializing ImGUI");
    lp3::imgui::ImGuiApp imgui_app(window);

    const glm::ivec2 res2d{ 427, 240 };
    gfx::programs::SimpleTextured program;

    LP3_LOG_DEBUG("Setting up Earth texture stuff");
    gfx::Texture earth_texture(
        SDL_LoadBMP_RW(media.load("Engine/Gfx/Earth.bmp"), 0));
    gfx::ElementWriter<gfx::TexVert> earth_elements{ 4 };
    gfx::upright_quad(earth_elements.add_quad(), glm::vec2(53, 0),
                      glm::vec2(320, 240),
                      0.0f, glm::vec2(0, 0), glm::vec2(1.0, 1.0));

    LP3_LOG_DEBUG("Loading the font");
    gfx::Font font{media.load("Engine/apple_kid.fnt")};
    gfx::ElementWriter<gfx::TexVert> font_elements{ 4000 };
    auto font_quads = font_elements.add_quads(80 * 7);

    std::string big_text{ "LP3 INPUT * * * LP3 INPUT * * * LP3 INPUT * * * LP3 INPUT * * * LP3 INPUT * * * LP3 INPUT * * * LP3 INPUT * * * LP3 INPUT * * * LP3 INPUT * * * LP3 INPUT * * * LP3 INPUT * * * LP3 INPUT * * * LP3 INPUT * * * LP3 INPUT * * * LP3 INPUT * * * LP3 INPUT * * * " };
    std::size_t text_i = 0;

    auto node = [&](const glm::mat4 & previous) {
        program.use();
        auto _2d = gfx::create_2d_screen(previous, res2d);
        program.set_mvp(_2d);
        program.set_texture(earth_texture.gl_id());
        program.draw(earth_elements);

        program.set_texture(font.texture().gl_id());
        program.draw(font_elements);
        //tpaper(previous);
		ImGui::Render();
    };

    sims::FrameTimer frame_timer;

    std::size_t letter_count = 0;   // Last letters written, represents how many
                            // quads we must clear.

    LP3_LOG_INFO("Enumerate JoySticks, not all of which are game controllers.\n"
                 "It seems like SDL JoySticks == DirectInput devices, while \n"
                 "SDL Game Controllers == XInputDevices.");
    std::vector<gsl::owner<SDL_Joystick *>> joys;

    //int r = SDL_GameControllerAddMapping(
    //  "830560200000000000005049445649,iBuffalo SNES Pad,a:b1,b:b2,y:b3,"
    //  "x:b0,start:b9,guide:b12,back:b8,dpup:h0.1,dpleft:h0.8,dpdown:h0.4,"
    //  "dpright:h0.2,leftshoulder:b4,rightshoulder:b5,leftstick:b10,"
    //  "rightstick:b11,leftx:a0,lefty:a1,rightx:a2,righty:a3,lefttrigger:b6,"
    //  "righttrigger:b7");
    //if (-1 == r) {
    //  LP3_LOG_ERROR("Error adding mapping: %s", SDL_GetError());
    //} else if (0 == r) {
    //  LP3_LOG_WARNING("Hmm, an existing mapping UPDATED?!");
    //} else {
    //  LP3_LOG_INFO("Cool man.");
    //}
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        const char * js_name = SDL_JoystickNameForIndex(i);
        LP3_LOG_VAR(js_name)
        LP3_LOG_INFO("Joystick Name=%s", (js_name ? js_name : "nullptr"));
        char guid[32];
        static_assert(sizeof(guid) == 32, "?!");
        SDL_JoystickGetGUIDString(SDL_JoystickGetDeviceGUID(i),
                                  guid, sizeof(guid));
        LP3_LOG_INFO("        guid =%s", guid);
        SDL_Joystick * joy = SDL_JoystickOpen(i);
        if (joy) {
            joys.push_back(joy);
            LP3_LOG_INFO("    Axises  : %d", SDL_JoystickNumAxes(joy));
            LP3_LOG_INFO("    Buttons : %d", SDL_JoystickNumButtons(joy));
            LP3_LOG_INFO("    Balls : %d", SDL_JoystickNumBalls(joy));
        }
        else {
            joys.push_back(nullptr);
            LP3_LOG_INFO("    ~~~~ x_x Couldn't open this Joystick!");
        }

        const char * gc_name = SDL_GameControllerNameForIndex(i);
        LP3_LOG_VAR(gc_name)
        LP3_LOG_INFO("    Game Controller Name=%s", (gc_name ? gc_name : "nullptr"));
        LP3_LOG_INFO("    SDL_IsGameController(%i)=%s", i, (SDL_IsGameController(i) ? "true" : "false"));

    }

    // TODO: Close joysticks - see https://wiki.libsdl.org/SDL_JoystickOpen

    // const ImVec4 g_clear_color = ImColor(114, 144, 154);

    return loop.run([&]() {
        frame_timer.next_frame();
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            imgui_app.process_event(e);
            switch (e.type) {
            case SDL_QUIT:
                return false;
            case SDL_WINDOWEVENT:
                window.handle_events(e.window);
                break;
            case SDL_CONTROLLERDEVICEADDED:
                LP3_LOG_INFO("A controller device was added.");
                break;
        case SDL_CONTROLLERDEVICEREMOVED:
                LP3_LOG_INFO("A controller device was removed.");// %d", (int)e.cdevice);
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                LP3_LOG_INFO("Button down!");// %d", (int)e.cbutton);
                break;
            case SDL_CONTROLLERBUTTONUP:
                LP3_LOG_INFO("Button up!");// %d", (int)e.cbutton);
                break;
            case SDL_CONTROLLERAXISMOTION:
                LP3_LOG_INFO("Axis time baby.");// %d", (int)e.caxis);
                break;
            default:
                break;
            }
        }

		controls.update();


        imgui_app.new_frame(window);

		ImGui::Begin("Debug");
		ImGui::Text("Hello, world!");
		ImGui::End();

		auto keys = controls.get_current_pressed_keys();

		std::stringstream ss;
		for (const auto & key : keys) {
			ss << key.first << "=" << key.second << "\n";
		}
		std::string key_text = ss.str();
		ImGui::Begin("Debug 2");
    	ImGui::Text("%s", key_text.c_str());
		ImGui::End();

        for (std::size_t i = 0; i < letter_count; ++i) {
            gfx::upright_quad(font_quads[i], glm::vec2(0, 0), glm::vec2(0, 0), 0.0f, glm::vec2(0.0f, 0.0), glm::vec2(0, 0));
        }

        std::string text = big_text.substr(text_i) + big_text.substr(0, text_i);
        std::string fps = str(boost::format("FPS: %d\n%s") % frame_timer.get_fps() % text);
        ++text_i;
        if (text_i >= big_text.length()) {
            text_i = 0;
        }
        letter_count =
            gfx::write_string(font_quads, font, glm::vec2(10, 10), 0.0f, 427.0f, fps);
        window.render(node);

        return true;
    });
    return 0;
}

LP3_MAIN(_main)
