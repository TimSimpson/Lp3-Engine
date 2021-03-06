// ~begin-doc
#include <sstream>
#include <lp3/core.hpp>
#include <lp3/gfx.hpp>
#include <lp3/imgui.hpp>
#include <lp3/input.hpp>
#include <lp3/mix.hpp>
#include <lp3/sims.hpp>
#include <lp3/main.hpp>

namespace core = lp3::core;
namespace gfx = lp3::gfx;
namespace input = lp3::input;
namespace mix = lp3::mix;
namespace sdl = lp3::sdl;
namespace sims = lp3::sims;


int _main(core::PlatformLoop & loop) {
    core::LogSystem log;
    sdl::SDL2 sdl2(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK| SDL_INIT_GAMECONTROLLER);

    input::Controls controls;
    core::MediaManager media;

    const glm::ivec2 res2d{ 427, 240 };
    gfx::Window window("Lp3 Mix", res2d);

	mix::Mixer mixer(22050, MIX_DEFAULT_FORMAT, 2, 4096);

    const auto music_path = media.path("Engine/Sfx/c.wav");
    mix::Music music = Mix_LoadMUS(music_path.c_str());

	const auto ogg_path = media.path("Engine/Sfx/test.ogg");
	mix::Music ogg = Mix_LoadMUS(ogg_path.c_str());

    auto hello_wav = media.path("Engine/Sfx/Hello.wav");
    mix::Chunk hello_chunk = Mix_LoadWAV(hello_wav.c_str());

    lp3::imgui::ImGuiApp imgui_app(window);

    gfx::programs::SimpleTextured program;

    auto node = [&](const glm::mat4 &) {
        ImGui::Render();
    };

    sims::FrameTimer frame_timer;

    if(!mix::Music::is_playing()) { //Play the music
		music.play();
    }

    {
        auto r = Mix_FadeInChannelTimed(-1, hello_chunk, -1, 2000, 8000);
        LP3_ASSERT(r != -1);
    }


	const std::int64_t ms_per_update = 1000 / 60;  //16 ms for 60 fps
	sims::GameClock clock(ms_per_update);
	std::int64_t time_to_play = 0000;
	int play_state = 0;

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
            default:
                break;
            }
        }

		controls.update();

		clock.run_updates([&time_to_play, &ogg, &play_state](std::int64_t ms) {
			time_to_play += ms;
			if (play_state == 0) {
				if (time_to_play > 1000) {
					Mix_FadeOutMusic(1000);
					ogg.fade_in(1000);
					play_state = 1;
					time_to_play = 0;
				}
			}
			else if (play_state == 1) {
				if (time_to_play > 1000) {
					play_state = 2;
				}
			}
		});

        imgui_app.new_frame(window);

        ImGui::Begin("Listen to the Beautiful Music");
        std::string zero(500, '0');
        ImGui::Text("%s", zero.c_str());
        ImGui::End();

        auto keys = controls.get_current_pressed_keys();

        window.render(node);

        return true;
    });
    return 0;
}

LP3_MAIN(_main)
// ~end-doc
