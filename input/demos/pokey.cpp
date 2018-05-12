// ---------------------------------------------------------------------------
// This shows a simple application that uses an lp3::input::Controller to
// move a face around the screen. It also shows all device keys that being
// pressed.
// ---------------------------------------------------------------------------
#include <boost/format.hpp>
#include <lp3/gfx.hpp>
#include <lp3/input.hpp>
#include <lp3/main.hpp>

namespace core = lp3::core;
namespace gfx = lp3::gfx;
namespace input = lp3::input;
namespace sdl = lp3::sdl;

void set_default_controls(lp3::input::Controls & controls) {
    // Tell the input system that, if it's available, we'd love to use a
    // gamepad.
    input::PreferredButtonMapping game_pad_mapping;
    game_pad_mapping.device = input::PreferredDevice::GAME_PAD;

    // Set the mapping. The names of the keys for each device are in the order
    // of the virtual control buttons we wish to assign.
    game_pad_mapping.set_mapping(
        "Left Analog Up", "Left Analog Down", "Left Analog Left",
        "Left Analog Right", "A", "B", "X");

    // Tell the input system that our second choice is for keyboard controls.
    input::PreferredButtonMapping kb_mapping;
    kb_mapping.device = input::PreferredDevice::KEYBOARD;
    // Ditto.
    kb_mapping.set_mapping(
        "Up", "Down", "Left", "Right", "X", "C", "Z");

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

    const glm::vec2 screen_size(1280, 720);
    const glm::vec2 tile_size(16, 16);
    const glm::vec2 map_size = screen_size / tile_size;

    gfx::Window window("Lp3::Input Example", screen_size);
    glEnable(GL_DEPTH_TEST);
    gfx::Texture texture_text{IMG_Load_RW(media.load("Engine/text.bmp"), 0)};
    gfx::ElementWriter<gfx::TexVert> elements(
        lp3::narrow<std::size_t>((map_size.x * map_size.y + 1) * 4));
    gfx::programs::SimpleTextured program;
    gfx::TileMap tm{ tile_size, map_size };

    auto pokey_quad = elements.add_quad();
    auto quads = tm.create_quads(elements);

    tm.write({20, 1}, "~ INSTRUCTIONS ~", true);
    int tm_y = 2;
    auto write_text = [&](const char * msg, int button_index) {
        // Find out what the virtual button is set to now.
        auto button_config = controls.get_button_configuration(0, button_index);
        auto text = str(boost::format(
            "%s: %s %s") % msg % button_config.first % button_config.second);
        tm.write({ 1, tm_y }, text.c_str(), true);
        ++ tm_y;
    };
    write_text("Up", 0);
    write_text("Down", 1);
    write_text("Left", 2);
    write_text("Right", 3);
    write_text("Face", 4);
    write_text("Snail", 5);
    write_text("Cat", 6);

    glm::vec2 pokey_position = screen_size / glm::vec2(2.0, 2.0);

    auto drawer = [&](const glm::mat4 & previous) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto _2d = gfx::create_2d_screen(previous, screen_size);

        program.use();
        program.set_mvp(_2d);
        program.set_texture(texture_text.gl_id());
        program.draw(elements);
    };

    const glm::vec2 faces[] = {
        {1, 6},   // smiley face
        {12, 6},  // snail
        {13, 6}   // cat
    };
    const glm::vec2 texture_resolution(256.0f, 128.0f);

    glm::vec2 current_avatar = faces[0];

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

        // Must be called each frame
        controls.update();

        // Print out currently pressed keys
        tm.write({ 1, 19 }, "Currently pressed keys:", true);
        int row = 20;
        tm.fill({0, 20}, {map_size.x, map_size.y - 20});  // clear text
        for (const auto & key : controls.get_current_pressed_keys()) {
            auto text = str(boost::format("%s %s") % key.first % key.second);
            tm.write({ 1, row }, text.c_str(), true);
            row += 1;
        }

        input::Control & controller = controls.get_control(0);

        // Move avatar
        pokey_position.y +=
            (controller.analog_state(0) * -1.0f) + controller.analog_state(1);
        pokey_position.x +=
            (controller.analog_state(2) * -1.0f) + controller.analog_state(3);

        // Change faces
        if (controller.state(4)) {
            current_avatar = faces[0];
        } else if (controller.state(5)) {
            current_avatar = faces[1];
        } else if (controller.state(6)) {
            current_avatar = faces[2];
        }

        glm::vec2 half_pokey(8, 8);
        gfx::upright_quad(
            pokey_quad,
            pokey_position - half_pokey,
            pokey_position + half_pokey,
            0.0f,
            (glm::vec2(16, 16) * current_avatar) / texture_resolution,
            (glm::vec2(16, 16) * (current_avatar + glm::vec2(1, 1)))
                / texture_resolution);

        tm.set_quads({ 16.0f, 16.0f }, 0.0f, quads, { 16.0f, 16.0f },
                     texture_text.size());

        window.render(drawer);

        return !quit;
    });
    return 0;
}

LP3_MAIN(_main)
// ~end-doc
