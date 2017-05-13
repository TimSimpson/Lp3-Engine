#include <fstream>
#include <iostream>
#include <memory>
#include <lp3/core.hpp>
#include <lp3/sdl.hpp>
#include <lp3/log.hpp>
#include <lp3/main.hpp>

namespace core = lp3::core;
namespace sdl = lp3::sdl;


int _main(core::PlatformLoop & loop) {
    sdl::SDL2 sdl2(SDL_INIT_VIDEO);

    TTF_Init();

    core::LogSystem log;
    core::MediaManager media;

    SDL_Log("I is a log too!\n");

    LP3_LOG_DEBUG("Hello SDL!");

    sdl::Window window = SDL_CreateWindow(
        "SDL2",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480,
        SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE
    );

    LP3_LOG_DEBUG("Creating renderer...");
    sdl::Renderer renderer
        = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    LP3_LOG_DEBUG("Loading texture...");
    const auto bmp_file = media.path("Engine/Gfx/Earth.bmp");
    sdl::Surface bitmap = SDL_LoadBMP(bmp_file.c_str());

    LP3_LOG_DEBUG("Loading font...")
    const auto font_path = media.path("Engine/Gfx/AGENCYB.ttf");
    auto font = TTF_OpenFont(font_path.c_str(), 28);
    if (nullptr == font) {
        LP3_LOG_ERROR(SDL_GetError());
        LP3_THROW2(core::Exception, "Error loading font!");
    }

    SDL_Color text_color = { 255, 0, 255, 255 };

    auto text = TTF_RenderText_Solid(
        font, "Hello, beautiful\nEarth :)", text_color);

    SDL_BlitSurface(text, nullptr, bitmap, NULL);

    //sdl::Texture tex = SDL_CreateTextureFromSurface(renderer, bitmap);
    sdl::Texture tex = SDL_CreateTextureFromSurface(renderer, text);
    LP3_LOG_DEBUG("Yes, somehow this is new code.");

	LP3_LOG_DEBUG("start itr");
    loop.run([&renderer, &tex]() {
        SDL_Event e;
        bool quit = false;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, tex, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        return !quit;
    });

    return 0;
}

LP3_MAIN(_main)
