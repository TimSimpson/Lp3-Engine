// There's probably an easier way to ignore warnings in CMake than this.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#include "../lib/imgui_draw.cpp"
#pragma GCC diagnostic pop

