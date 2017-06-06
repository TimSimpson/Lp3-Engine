// There's probably an easier way to ignore warnings in CMake than this.
#ifndef _MSC_VER
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wuninitialized"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif

#include "../lib/imgui_draw.cpp"

#ifndef _MSC_VER
#pragma GCC diagnostic pop
#endif
