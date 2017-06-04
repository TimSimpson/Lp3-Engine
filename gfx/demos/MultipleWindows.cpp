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
    gfx::Window window("Super Simple", glm::vec2{ 640, 480 });

    gfx::Window window2("Super Simple Window 2", glm::vec2{ 640, 480 });

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
                window2.handle_events(e.window);
                break;
                // etc, etc
            }
        }
        gfx::SceneNodeFunc do_nothing = [](const glm::mat4 & m) {};
        window.render(do_nothing);
        window2.render(do_nothing);
        return !quit;
    });
}

LP3_MAIN(_main);
