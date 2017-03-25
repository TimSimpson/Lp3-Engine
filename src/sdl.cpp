#define LP3_CORE_API_CREATE
#include <lp3/sdl.hpp>

namespace lp3 { namespace sdl {

SDL2::SDL2(Uint32 flags) {
    SDL_Init(flags);
}

SDL2::~SDL2() {
    SDL_Quit();
}

}	}
