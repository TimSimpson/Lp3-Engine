#ifndef FILE_LP3_SDL_HPP
#define FILE_LP3_SDL_HPP

#include "core/config.hpp"
#include "core/Exception.hpp"
#include "log.hpp"
#include <gsl/gsl>
#include <SDL.h>


namespace lp3 { namespace sdl {

LP3_CORE_API
struct SDL2 {
    SDL2(Uint32 flags);
    ~SDL2();
};

/*
	Template which checks to see if a resource is nullptr on creation and
	calls a deleter function on destruction.
 */
template<typename ResourceTypePtr, void(*Deleter)(ResourceTypePtr)>
class SdlAutoDeletedResource {
public:
	SdlAutoDeletedResource(ResourceTypePtr _ptr)
    : ptr(_ptr)
    {
        if (!ptr) {
            LP3_LOG_ERROR(SDL_GetError());
            LP3_THROW2(lp3::core::Exception, "Couldn't create resource");
        }
    }

    SdlAutoDeletedResource(SdlAutoDeletedResource && rvalue)
    :  ptr(rvalue.ptr)
    {
        rvalue.ptr = 0;
    }

    ~SdlAutoDeletedResource() {
        if (ptr) {
            Deleter(ptr);
        }
    }

	SdlAutoDeletedResource & operator=(const SdlAutoDeletedResource & other)
		= delete;

    operator ResourceTypePtr () {
        return ptr;
    }
private:
    gsl::owner<ResourceTypePtr> ptr;
};

using Window = SdlAutoDeletedResource<SDL_Window *, SDL_DestroyWindow>;
using Renderer = SdlAutoDeletedResource<SDL_Renderer *, SDL_DestroyRenderer>;
using Surface = SdlAutoDeletedResource<SDL_Surface *, SDL_FreeSurface>;
using Texture = SdlAutoDeletedResource<SDL_Texture *, SDL_DestroyTexture>;
using GLContext = SdlAutoDeletedResource<SDL_GLContext, SDL_GL_DeleteContext>;

} }

#endif
