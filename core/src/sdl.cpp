#define LP3_CORE_API_CREATE
#include <lp3/sdl.hpp>

namespace lp3 { namespace sdl {

LP3_CORE_API
SDL2::SDL2(Uint32 flags) {
    SDL_Init(flags);
    IMG_Init(IMG_INIT_PNG);
}

LP3_CORE_API
SDL2::~SDL2() {
    SDL_Quit();
}

LP3_CORE_API
SdlAssertCalled::SdlAssertCalled()
: Exception("SDL_Assert triggered")
{
}

LP3_CORE_API
SdlAssertToExceptionConverter::SdlAssertToExceptionConverter()
: old_handler(SDL_GetAssertionHandler(nullptr))
{
    #ifndef LP3_COMPILE_TARGET_POCKETCHIP
        auto throw_except = [](const SDL_AssertData *, void *)
        -> SDL_AssertState {
            throw SdlAssertCalled();
        };
    #else
        // Pocket Chip appears to be using an older SDL?
        auto throw_except = [](const SDL_assert_data *, void *)
        -> SDL_assert_state {
            throw SdlAssertCalled();
        };
    #endif
    SDL_SetAssertionHandler(throw_except, nullptr);
}

LP3_CORE_API
SdlAssertToExceptionConverter::~SdlAssertToExceptionConverter() {
    SDL_SetAssertionHandler(old_handler, nullptr);
    SDL_ResetAssertionReport();
}

LP3_CORE_API
RWops::RWops()
:	ops(nullptr)
{}

LP3_CORE_API
RWops::RWops(gsl::owner<SDL_RWops *> _ops)
:	ops(_ops)
{
	SDL_assert(nullptr != _ops);
}

LP3_CORE_API
RWops::~RWops() {
	if (nullptr != ops) {
		auto result = ops->close(ops);
		SDL_assert(0 == result);
	}
	ops = nullptr;
}

LP3_CORE_API
RWops::RWops(RWops && rhs)
: ops(rhs.ops)
{
	rhs.ops = nullptr;
}

LP3_CORE_API
RWops & RWops::operator=(RWops && rvalue) {
	SDL_assert(nullptr == ops);
	if (this->ops != rvalue.ops) {
		this->ops = rvalue.ops;
		rvalue.ops = nullptr;
	}
	return *this;
}


}	}
