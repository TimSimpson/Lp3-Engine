#include <iostream>
#include <lp3/core.hpp>
#include <lp3/sdl.hpp>
#include <lp3/main.hpp>

namespace core = lp3::core;
namespace sdl = lp3::sdl;

struct Window {
    sdl::Window window;
    sdl::Renderer renderer;

    Window(const char * title)
    :   window(SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            320,
            240,
            SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE
        )),
        renderer(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED))
    {
    }

    void do_events() {
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }
};

int _main(core::PlatformLoop & loop) {
    sdl::SDL2 sdl2(SDL_INIT_VIDEO);

    core::LogSystem log;

    LP3_LOG_DEBUG("Hi!");

    Window win1{"Window 1"};
    Window win2{"Window 2"};
    Window win3{"Window 3"};

    loop.run([&win1, &win2, &win3]() {
		bool quit = false;
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}
        win1.do_events();
        win2.do_events();
        win3.do_events();
		return !quit;
    });

    return 0;
}

LP3_MAIN(_main)
