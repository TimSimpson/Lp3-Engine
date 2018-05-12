#include <sstream>
#include <boost/format.hpp>
#include <lp3/core.hpp>
#include <lp3/gfx.hpp>
#include <lp3/sims.hpp>
#include <lp3/main.hpp>

namespace core = lp3::core;
namespace gfx = lp3::gfx;
namespace sdl = lp3::sdl;
namespace sims = lp3::sims;

int _main(core::PlatformLoop & loop) {
    core::LogSystem log;
    sdl::SDL2 sdl2(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK| SDL_INIT_GAMECONTROLLER);

    LP3_LOG_DEBUG("Setting up input.");
    core::MediaManager media;

    gfx::Window window("Lp3 Input", glm::vec2{ 427, 240 });

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

    std::string big_text{
        "pICKLE jUICE bROS!!\n"
        "* ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
        "* abcdefghijklmnopqrstuvwxyz\n"
        "* 0123456789\n"
        "* !@#$%^&*()\n"
    };
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
    };

    sims::FrameTimer frame_timer;

    std::size_t letter_count = 0;   // Last letters written, represents how many
                                    // quads we must clear.

    return loop.run([&]() {
        frame_timer.next_frame();
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
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
