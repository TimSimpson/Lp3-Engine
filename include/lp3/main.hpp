#ifdef FILE_LP3_MAIN_HPP
    #error "This file should only be included once."
#endif
#define FILE_LP3_MAIN_HPP

#include <lp3/le.hpp>
#include "platform.hpp"
#define SDL_MAIN_HANDLED
#include <SDL.h>


// #pragma comment(lib, "SDL2main.lib")

#define LP3_MAIN(main_function) \
    int main(int argc, char* argv[]) { \
        lp3::core::PlatformLoop loop(argc, argv); \
        return main_function(loop); \
    }

