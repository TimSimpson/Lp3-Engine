#include <sstream>
#include <lp3/gfx.hpp>
#include <lp3/input.hpp>
#include <lp3/sims.hpp>
#include <lp3/main.hpp>

namespace core = lp3::core;
namespace gfx = lp3::gfx;
namespace input = lp3::input;
namespace sdl = lp3::sdl;
namespace sims = lp3::sims;

void set_default_controls(lp3::input::Controls & controls) {
    // Tell the input system that, if it's available, we'd love to use a
    // gamepad.
    input::PreferredButtonMapping game_pad_mapping;
    game_pad_mapping.device = input::PreferredDevice::GAME_PAD;

    game_pad_mapping.set_mapping(
        "Left Analog Up", "Left Analog Down", "Left Analog Left",
        "Left Analog Right", "A");

    // Tell the input system that our second choice is for keyboard controls.
    input::PreferredButtonMapping kb_mapping;
    kb_mapping.device = input::PreferredDevice::KEYBOARD;
    kb_mapping.set_mapping(
        "Up", "Down", "Left", "Right", "Spacebar");

    std::vector<input::PreferredButtonMapping> preferred_mappings = {
        game_pad_mapping, kb_mapping
    };

    controls.set_defaults(0, preferred_mappings);
}

int _main(core::PlatformLoop & loop) {
    core::LogSystem log;
    sdl::SDL2 sdl2(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK| SDL_INIT_GAMECONTROLLER);

    input::Controls controls;

    set_default_controls(controls);

    core::MediaManager media;

    const glm::ivec2 screen_size(640, 480);
    const glm::ivec2 tile_size(16, 16);
    const glm::ivec2 map_size = screen_size / tile_size;

    gfx::Window window("The Adventures of Pokey", screen_size);
    glEnable(GL_DEPTH_TEST);
    gfx::Texture texture_text{IMG_Load_RW(media.load("Engine/text.bmp"), 0)};
    gfx::ElementWriter<gfx::TexVert> elements((map_size.x * map_size.y + 1) * 4);
    gfx::programs::SimpleTextured program;
    gfx::TileMap tm{ tile_size, map_size };

    auto pokey_quad = elements.add_quad();
    auto quads = tm.create_quads(elements);

    tm.write({ 1, 1 }, "The Adventures of Pokey", true);
    glm::vec2 pokey_position(320, 240);

    sims::FrameTimer frame_timer;

    auto drawer = [&](const glm::mat4 & previous) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto _2d = gfx::create_2d_screen(previous, screen_size);

        program.use();
        program.set_mvp(_2d);
        program.set_texture(texture_text.gl_id());
        program.draw(elements);
    };

    return loop.run([&]() {
        bool quit = false;
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_WINDOWEVENT:
                window.handle_events(e.window);
                break;
            }
        }

        controls.update();

        input::Control & controller = controls.get_control(0);
        pokey_position.y +=
            (controller.analog_state(0) * -1.0f) + controller.analog_state(1);
        pokey_position.x +=
            (controller.analog_state(2) * -1.0f) + controller.analog_state(3);

        if (controller.state(4)) {
            quit = true;
        }

        glm::vec2 half_pokey(8, 8);
        gfx::upright_quad(
            pokey_quad,
            pokey_position - half_pokey,
            pokey_position + half_pokey,
            0.0f,
            glm::vec2(16, 6 * 16) / glm::vec2(256.0f, 128.0f),
            glm::vec2(32, 7 * 16) / glm::vec2(256.0f, 128.0f));


        tm.set_quads({ 16.0f, 16.0f }, 0.0f, quads, { 16.0f, 16.0f },
                     texture_text.size());

        window.render(drawer);

        LP3_LOG_DEBUG("FPS: %d\tAvg FPS: %d",
                      frame_timer.get_fps(),
                      frame_timer.get_average_fps());
        frame_timer.next_frame();

        return !quit;
    });
    return 0;
}

LP3_MAIN(_main)
