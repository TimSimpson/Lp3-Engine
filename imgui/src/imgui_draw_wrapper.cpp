// There's probably an easier way to ignore warnings in CMake than this.
#ifndef _MSC_VER
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunknown-pragmas"
    #pragma GCC diagnostic ignored "-Wuninitialized"
    #ifndef __clang__
        #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
    #endif
#endif

#include "../lib/imgui_draw.cpp"

#ifndef _MSC_VER
    #pragma GCC diagnostic pop
#endif
