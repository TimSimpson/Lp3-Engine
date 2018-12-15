#define LP3_CORE_API_CREATE
#include <lp3/core/utils.hpp>
#include <lp3/assert.hpp>

#ifdef LP3_COMPILE_TARGET_DREAMCAST
    #include <kos.h>
#endif
#ifdef LP3_COMPILE_TARGET_PC
    #include <cstdlib>  // getenv
#endif

#include <memory>
#include <boost/format.hpp>
#include <fstream>
#include <SDL.h>

#include <lp3/log.hpp>
#include <lp3/core/Exception.hpp>
#include <lp3/core/utils.hpp>


namespace lp3 { namespace core {

namespace {
	std::string get_env_media_path() {
		return lp3::core::get_env_var("LP3_ROOT_PATH").value_or("./");
	}
}


MediaManager::MediaManager(const std::string & _base_directory)
:   base_directory(_base_directory)
{}

#ifdef LP3_COMPILE_TARGET_PC

MediaManager::MediaManager()
:   base_directory(get_env_media_path())
{}

#elif defined(LP3_COMPILE_TARGET_EMSCRIPTEN)

MediaManager::MediaManager()
:   base_directory("/")
{}

#elif defined(LP3_COMPILE_TARGET_DREAMCAST)

#ifndef LP3_DREAMCAST_MEDIA_PATH
	#error "Please define LP3_DREAMCAST_MEDIA_PATH"
#endif

MediaManager::MediaManager()
:   base_directory(LP3_DREAMCAST_MEDIA_PATH)
{}

#else

    #error "I don't know how to handle this platform!"

#endif

LP3_CORE_API
sdl::RWops MediaManager::load(const gsl::cstring_span<> & file) {
    std::string full_path =
		str(boost::format("%s/%s") % base_directory % file.data());
	SDL_RWops * ptr = SDL_RWFromFile(full_path.c_str(), "rb");
	if (!ptr) {
		LP3_LOG_ERROR("Error opening file %s.", full_path);
		LP3_LOG_ERROR("SDL error %s.", SDL_GetError());
		LP3_THROW(lp3::core::Exception);
	}
	return sdl::RWops{ptr};
}

LP3_CORE_API
std::string MediaManager::path(const gsl::cstring_span<> & file) const {
	return str(boost::format("%s/%s") % base_directory % file.data());
}

LP3_CORE_API
sdl::RWops MediaManager::save(const gsl::cstring_span<> & file) {
	std::string full_path =
		str(boost::format("%s/%s") % base_directory % file.data());
	SDL_RWops * ptr = SDL_RWFromFile(full_path.c_str(), "wb");
	if (!ptr) {
		LP3_LOG_ERROR("Error opening save file %s.", full_path);
		LP3_THROW(lp3::core::Exception);
	}
	return sdl::RWops{ptr};
}

LP3_CORE_API
MediaManager MediaManager::sub_directory(const gsl::cstring_span<> & sub_d) {
	if (base_directory == "/") {
		MediaManager subm(sub_d.data());
		return subm;
	} else {
		MediaManager subm(
			str(boost::format("%s/%s") % base_directory % sub_d.data()));
		return subm;
	}
}

} }  // end lp3::core
