// ----------------------------------------------------------------------------
// SDL C++ RAII Wrappers and Helpers
// ============================================================================
//      Lp3 contains RAII enabled types for working with the SDL2 library.
//
//      The expectation is that code will use the C functions provided by
//      the SDL library but use the types found here to manage the resources
//      it creates. Some rules of thumb about these types:
//
//          * The majority of these wrapped types have implicit conversions to
//            the pointers they wrap, allowing them to be used when calling
//            SDL functions.
//          * However these types "own" a resource (in the gsl::owner sense)
//            and should not be copied. Instead references should be passed.
//          * There is however a default constructor for these which sets the
//            underlying pointer to nullptr. These resources can be assigned
//            to from a not null pointer; however after that they can never be
//            assigned to again (honoring the Moonlight Graham principle).
//            Additionally there are asserts to ensure the non-default
//            constructor is not passed nullptr, making it possible to directly
//            pass the result of SDL calls that return null on failures.
//
// ~see-file "../../demos/Sdl2Simple.cpp"
// ---------------------------------------------------------------------------/
// ~end-doc summary

#ifndef FILE_LP3_SDL_HPP
#define FILE_LP3_SDL_HPP
#pragma once

// Disable SDL's
#define SDL_MAIN_HANDLED
#include <optional>
#include <gsl/gsl>
#include "core/config.hpp"
#include "core/Exception.hpp"
#include "log.hpp"
#include <SDL.h>
#include <SDL_image.h>


namespace lp3::sdl {

// ----------------------------------------------------------------------------
// struct SDL2
// ----------------------------------------------------------------------------
//      Create one of these in your main method to initialize the SDL.
//      The destructor will deinitialize the SDL.
// ----------------------------------------------------------------------------
LP3_CORE_API
struct SDL2 {
    SDL2(Uint32 flags);
    ~SDL2();
};
// -/

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

	SdlAutoDeletedResource()
		: ptr(nullptr)
	{
	}

    SdlAutoDeletedResource(SdlAutoDeletedResource && rvalue)
    :  ptr(rvalue.ptr)
    {
        rvalue.ptr = nullptr;
    }

    ~SdlAutoDeletedResource() {
        if (ptr) {
            Deleter(ptr);
        }
    }

	SdlAutoDeletedResource & operator=(SdlAutoDeletedResource && rvalue)
	{
		SDL_assert(nullptr == ptr);
		if (this->ptr != rvalue.ptr) {
			this->ptr = rvalue.ptr;
			rvalue.ptr = nullptr;
		}
		return *this;
	}
	SdlAutoDeletedResource & operator=(const SdlAutoDeletedResource & other)
		= delete;


    operator ResourceTypePtr () {
		SDL_assert(nullptr != ptr);
        return ptr;
    }
private:
    gsl::owner<ResourceTypePtr> ptr;
};

// ----------------------------------------------------------------------------
// Sdl Auto Deleted Resource types
// ----------------------------------------------------------------------------
//      The following types automatically delete SDL resources of the same
//      name when they go out of scope.
// ----------------------------------------------------------------------------
using Window = SdlAutoDeletedResource<SDL_Window *, SDL_DestroyWindow>;
using Renderer = SdlAutoDeletedResource<SDL_Renderer *, SDL_DestroyRenderer>;
using Surface = SdlAutoDeletedResource<SDL_Surface *, SDL_FreeSurface>;
using Texture = SdlAutoDeletedResource<SDL_Texture *, SDL_DestroyTexture>;
using GLContext = SdlAutoDeletedResource<SDL_GLContext, SDL_GL_DeleteContext>;
// ~end-doc


// Use these two classes to make the SDL_assert calls throw exceptions
// instead to test that code properly triggers them.
// NOTE: Currently this breaks the default behavior of SDL_assert afterwards
//       (asserts causes failures but it won't trigger a breakpoint) even
//       though it looks correct according to the docs, so only use it in tests.
LP3_CORE_API
class SdlAssertCalled : public lp3::core::Exception {
public:
	SdlAssertCalled();
};

LP3_CORE_API
class SdlAssertToExceptionConverter {
public:
	SdlAssertToExceptionConverter();

	~SdlAssertToExceptionConverter();

private:
    SDL_AssertionHandler old_handler;
};

LP3_CORE_API
inline void close_rwops(SDL_RWops * ops) {
	SDL_assert(nullptr != ops);
	auto result = ops->close(ops);
	SDL_assert(0 == result);
}

// ----------------------------------------------------------------------------
// RWops
// ----------------------------------------------------------------------------
//      Wraps SDL_RWops which controls all file and other resource I/O.
//
//      Unlike the majority of types this handles more than just resource
//      lifetime. SDL_RWops is a strange struct that contains function
//      pointers, all of which accept the struct itself as their first
//      argument. All of these functions are mapped to class methods here.
//
//      A few extra functions are added which read and write directly into
//      POD object references.
//
//      lp3::core::MediaManager's `load` and `save` methods return instances
//      of RWops.
// ----------------------------------------------------------------------------
LP3_CORE_API
class RWops {
public:
	RWops();

	RWops(gsl::owner<SDL_RWops *> ops);

	~RWops();

	RWops(RWops && rhs);

	RWops & operator=(RWops && rvalue);

	RWops & operator=(const RWops & other) = delete;

	// Implicitly convert to SDL_RWops
	inline operator SDL_RWops * () {
		SDL_assert(nullptr != ops);
		return ops;
	}

	inline std::size_t read(void * dst, std::size_t object_size,
		                    std::size_t object_count = 1) {
		SDL_assert(nullptr != ops);
		// object_size is the size of an object to be read-
		// object_count is the number of objects to read. If things work
		// exactly as expected object_count is what's returned.
		return ops->read(ops, dst, object_size, object_count);
	}

    template<typename T>
    inline void read(T & dst) {
        static_assert(std::is_pod<T>::value, "Type must be POD.");
        const auto result = read(reinterpret_cast<char *>(&dst), sizeof(T));
        SDL_assert(1 == result);
    }

	template<typename T>
	inline std::optional<T> read_optional() {
		static_assert(std::is_pod<T>::value, "Type must be POD.");
		char data[sizeof(T)];
		const auto result = read(data, sizeof(T));
		if (1 == result) {
            // This is less terse than it could be to get around a
            // strict-aliasing related warning in GCC in release mode.
            T * const copy_ptr = reinterpret_cast<T *>(data);
            T copy = *copy_ptr;
            return copy;
		} else {
			return std::nullopt;
		}
	}

	inline std::int64_t seek(std::int64_t offset, int whence=RW_SEEK_CUR) {
		SDL_assert(nullptr != ops);
		return ops->seek(ops, offset, whence);
	}

	inline std::int64_t seek_from_beginning(std::int64_t offset) {
		return seek(offset, RW_SEEK_SET);
	}

	inline std::int64_t seek_from_end(std::int64_t offset) {
		return seek(offset, RW_SEEK_END);
	}

	inline std::int64_t size() {
		SDL_assert(nullptr != ops);
		return ops->size(ops);
	}

	inline std::size_t write(const void * src, std::size_t object_count,
							 std::size_t object_size = 1) {
		SDL_assert(nullptr != ops);
		return ops->write(ops, src, object_count, object_size);
	}

    template<typename T>
    inline void write(const T & n) {
        static_assert(std::is_pod<T>::value, "Type must be POD.");
        const auto result = write(reinterpret_cast<const char *>(&n), sizeof(T));
        SDL_assert(1 == result);
    }
private:
	gsl::owner<SDL_RWops *> ops;
};

// ~end-doc

}

#endif
