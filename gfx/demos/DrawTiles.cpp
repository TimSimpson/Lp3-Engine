// ----------------------------------------------------------------------------
// This example shows how a TileMap can be used to write some text to the
// screen.
// ----------------------------------------------------------------------------
#include <lp3/core.hpp>
#include <lp3/gfx.hpp>
#include <lp3/main.hpp>
#include <lp3/sdl.hpp>
#include <lp3/sims.hpp>

namespace core = lp3::core;
namespace gl = lp3::gl;
namespace gfx = lp3::gfx;
namespace sdl = lp3::sdl;
namespace sims = lp3::sims;

int _main(core::PlatformLoop & loop) {
    sdl::SDL2 sdl2(SDL_INIT_VIDEO);
    core::LogSystem log;
    core::MediaManager media;

    // Creates SDL window and GL Context and calls lp3::gl::initialize()
    gfx::Window window("Draw Tiles", glm::vec2{ 640, 480 });

    glEnable(GL_DEPTH_TEST);

    gfx::Texture texture_text{IMG_Load_RW(media.load("Engine/text.bmp"), 0)};

    gfx::ElementWriter<gfx::TexVert> elements{ (640/16 * 480/16) * 4 };

    gfx::programs::SimpleTextured program;

    // Now make our construction paper with a resolution of 64 by 64.
    const glm::ivec2 res2d(640, 480);

    // Make the render list. This will store what to render and do it later
    sims::FrameTimer frame_timer;

	gfx::TileMap tm{ {16.0f, 16.0f}, {256.0f / 16.0f, 128.0f / 16.05f} };

	tm.write({ 0, 0 },
		"And ye, I say,\n"
		"don't doubt me,\n"
		"for I shall rise in the night\n"
		"to drag away your dreams.", true);

	tm.write({ 15.0f, 6.0f }, 97);

	auto quads = tm.create_quads(elements);

    auto drawer = [&](const glm::mat4 & previous) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto _2d = gfx::create_2d_screen(previous, res2d);

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
		tm.set_quads({ 16.0f, 16.0f }, 0.0f, quads, { 16.0f, 16.0f },
			         texture_text.size());

        window.render(drawer);

        LP3_LOG_DEBUG("FPS: %d\tAvg FPS: %d",
                      frame_timer.get_fps(),
                      frame_timer.get_average_fps());
        frame_timer.next_frame();

        return !quit;
    });
}

LP3_MAIN(_main)
// ~end-doc
