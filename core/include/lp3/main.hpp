// ----------------------------------------------------------------------------
// main.hpp
// ----------------------------------------------------------------------------
//      Contains a helper macro which defines the main function of a program
//      and calls a method provided by you which accepts a reference to
//      a PlatformLoop. This slightly eases writing portable programs,
//      particularly wrt Emscripten.
//
//      This file should only be included once.
// ----------------------------------------------------------------------------
#ifdef FILE_LP3_MAIN_HPP
    #error "This file should only be included once."
#endif
#define FILE_LP3_MAIN_HPP
#pragma once

#include <lp3/le.hpp>
#include "core/utils.hpp"
#define SDL_MAIN_HANDLED
#include <SDL.h>



#define LP3_MAIN(main_function) \
    int main(int argc, char* argv[]) { \
        lp3::core::PlatformLoop loop(argc, argv); \
        return main_function(loop); \
    }

