#define LP3_CORE_API_CREATE
#include <lp3/sdl.hpp>

namespace lp3 { namespace sdl {

SDL2::SDL2(Uint32 flags) {
    SDL_Init(flags);
}

SDL2::~SDL2() {
    SDL_Quit();
}


SdlAssertCalled::SdlAssertCalled()
: Exception("SDL_Assert triggered")
{
}


SdlAssertToExceptionConverter::SdlAssertToExceptionConverter()
: old_handler(SDL_GetAssertionHandler(nullptr))
{
    auto throw_except = [](const SDL_AssertData *, void *) -> SDL_AssertState {
        throw SdlAssertCalled();
    };
    SDL_SetAssertionHandler(throw_except, nullptr);
}

SdlAssertToExceptionConverter::~SdlAssertToExceptionConverter() {
    SDL_SetAssertionHandler(old_handler, nullptr);
    SDL_ResetAssertionReport();
}

}	}
