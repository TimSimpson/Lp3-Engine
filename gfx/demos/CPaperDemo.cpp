#include <lp3/core.hpp>
#include <lp3/gfx.hpp>
#include <lp3/main.hpp>
#include <lp3/sdl.hpp>

namespace core = lp3::core;
namespace gl = lp3::gl;
namespace gfx = lp3::gfx;
namespace sdl = lp3::sdl;


int _main(core::PlatformLoop & loop) {
    sdl::SDL2 sdl2(SDL_INIT_VIDEO);
    core::LogSystem log;
    core::MediaManager media;

    // Creates SDL window and GL Context and calls lp3::gl::initialize()
    gfx::Window window("Construction Paper", glm::vec2{ 640, 480 });

	// Make the render list. This will store what to render and do it later
	// when passed to CPaper.
	gfx::CPaper::RendererList r;

	const GLfloat z = -0.0f;
	r.triangle(gfx::red, glm::vec2(-1.0f, -1.0f), glm::vec2(1.0, 1.0), z);
    r.triangle(gfx::yellow, glm::vec2(0.0f, 0.0f), glm::vec2(1.0, 1.0), z);
    r.triangle(gfx::green, glm::vec2(1.0f, 1.0f), glm::vec2(2.0, 2.0), z);

    for (int i = 1; i < 50; ++i) {
		r.triangle(
			gfx::blue,
			glm::vec2(1.0f * i, 1.0f * i),
			glm::vec2((1.0f * i) + 1.0f, (1.0f * i) + 1.0f),
			z
		);
    }

	// Now make our construction paper with a resolution of 64 by 64.
	gfx::CPaper cpaper(glm::ivec2{ 32, 32 });
	// Copy in the render list
	cpaper.set_render_list(r);

    return loop.run([&]() {
        SDL_Event e;
        bool quit = false;
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
        window.render(std::ref(cpaper));
        return !quit;
    });
}

LP3_MAIN(_main)
