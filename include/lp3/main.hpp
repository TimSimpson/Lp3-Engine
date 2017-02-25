#ifdef FILE_LP3_MAIN_HPP
    #error "This file should only be included once."
#endif
#define FILE_LP3_MAIN_HPP

#include <lp3/le.hpp>

#if defined(LP3_COMPILE_TARGET_WINDOWS)

    #include <windows.h>
    #include "platform.hpp"

    #pragma comment(linker, "/SUBSYSTEM:WINDOWS")

    #define LP3_MAIN(main_function, window_title) \
        int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, \
                           LPSTR lp_cmd_line, int n_cmd_show) { \
            lp3::core::PlatformLoop loop(window_title, h_instance, \
                                         n_cmd_show); \
            return main_function(loop); \
        }

#elif defined(LP3_COMPILE_TARGET_EMSCRIPTEN)

    #include "platform.hpp"

    #define LP3_MAIN(main_function, window_title) \
        int main(int argc, char ** argv) { \
            lp3::core::PlatformLoop loop(argc, argv); \
            return main_function(loop); \
        }

#endif
