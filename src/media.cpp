#define LP3_CORE_API_CREATE
#include <lp3/core/media.hpp>

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

namespace core = lp3::core;


namespace {

    class InputFileStream : public core::InputStream
    {
    public:
        InputFileStream(const char * file_path)
        :   _eof(false),
            file() {
			LP3_LOG_DEBUG("Opening file %s...", file_path);
            file.open(file_path, std::ios::in | std::ios::binary);
            if (file.bad() || file.fail())
            {
                LP3_LOG_ERROR("Error opening file %s!", file_path);
                LP3_THROW2(core::Exception,
					str(boost::format("Error opening file %s!") % file_path));
            }
        }

		~InputFileStream() override = default;

        bool eof() const override {
            return _eof;
        }

        std::size_t read(gsl::span<char> buffer) override {
            file.read(buffer.data(), buffer.length());
            // Bad bit set! Stream integrity compromised.
            SDL_assert(!file.bad());
            _eof = file.eof();

            // Check for overflow on 32 bit machines
            SDL_assert(
                static_cast<std::streamsize>(
                    static_cast<std::size_t>(file.gcount()))
                == file.gcount());

            return static_cast<std::size_t>(file.gcount());
        }

        void reset(std::size_t position=0) override {
            this->_eof = seek(file, position, std::ios::beg);
        }

        void skip(std::size_t bytes) override {
            this->_eof = seek(file, bytes, std::ios::cur);
        }
    private:
        bool _eof;
        std::ifstream file;

        template<typename T>
        static bool seek(std::ifstream & file,
				         const std::size_t position,
				         const T seekdir) {
            SDL_assert(!file.bad()); // Can't reset, file is bad!
            SDL_assert(file.is_open()); // Can't reset, file not open!
            file.clear(); // Clear eof flags.
            file.seekg(position, seekdir);
            return file.eof();
        }
    };

    std::string get_env_media_path() {
        return lp3::core::get_env_var("LP3_ROOT_PATH").get_value_or("");
    }

}


namespace lp3 { namespace core {

#ifdef LP3_COMPILE_TARGET_PC

MediaManager::MediaManager(const std::string & _base_directory)
:   base_directory(_base_directory)
{}

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

gsl::owner<InputStream *> MediaManager::load(const gsl::cstring_span<> & file) {
    std::string full_path =
		str(boost::format("%s/%s") % base_directory % file.data());
	return new InputFileStream(full_path.c_str());
}

std::string MediaManager::path(const gsl::cstring_span<> & file) const {
	return str(boost::format("%s/%s") % base_directory % file.data());
}

} }  // end lp3::core
