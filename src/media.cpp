#define LP3_CORE_API_CREATE
#include <lp3/core/media.hpp>

#ifdef LP3_COMPILE_TARGET_DREAMCAST
    #include <kos.h>
#endif
#ifdef LP3_COMPILE_TARGET_WINDOWS
    #include <cstdlib>  // getenv
#endif

#include <memory>
#include <lp3/assert.hpp>
#include <lp3/log.hpp>
#include <lp3/core/Exception.hpp>
#include <boost/format.hpp>
#include <fstream>


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
                LP3_THROW(core::Exception,
					str(boost::format("Error opening file %s!") % file_path));
            }
        }

		virtual ~InputFileStream() = default;

        virtual bool eof() const {
            return _eof;
        }

        virtual std::size_t read(gsl::span<char> buffer) {
            file.read(buffer.data(), buffer.length());
            LP3_ASSERT_FALSE_MESSAGE(file.bad(),
                "Bad bit set! Stream integrity compromised.");
            _eof = file.eof();

            // Check for overflow on 32 bit machines
            LP3_ASSERT_EQUAL(
                static_cast<std::streamsize>(
                    static_cast<std::size_t>(file.gcount())),
                file.gcount());

            return static_cast<std::size_t>(file.gcount());
        }

        virtual void reset(std::size_t position=0) {
            this->_eof = seek(file, position, std::ios::beg);
        }

        virtual void skip(std::size_t bytes) {
            this->_eof = seek(file, bytes, std::ios::cur);
        }
    private:
        bool _eof;
        std::ifstream file;

        template<typename T>
        bool seek(std::ifstream & file,
				  const std::size_t position,
				  const T seekdir) {
            LP3_ASSERT_FALSE_MESSAGE(file.bad(), "Can't reset, file is bad!");
            LP3_ASSERT_TRUE_MESSAGE(file.is_open(), "Can't reset, file not open!");
            file.clear(); // Clear eof flags.
            file.seekg(position, seekdir);
            return file.eof();
        }
    };

#ifdef LP3_COMPILE_TARGET_PC
    std::string get_env_media_path() {
#ifdef LP3_COMPILE_TARGET_WINDOWS
		char * env_var_value;
		std::size_t length;
		auto result = _dupenv_s(&env_var_value, &length, "LP3_ROOT_PATH");
		LP3_ASSERT_TRUE_MESSAGE(0 == result, "Error calling _dupenv_s");
		std::unique_ptr<char> delete_env_var(env_var_value);
#else
		const char * const env_var_value = getenv("LP3_ROOT_PATH");
#endif
		LP3_ASSERT_FALSE_MESSAGE(nullptr == env_var_value,
			                     "Couldn't get LP3_ROOT_PATH!");
		return std::string{ env_var_value };
    }
#endif

}


namespace lp3 { namespace core {

#ifdef LP3_COMPILE_TARGET_WINDOWS

MediaManager::MediaManager(const std::string & base_directory)
:   base_directory(base_directory)
{}

MediaManager::MediaManager()
:   base_directory(get_env_media_path())
{}

#elif defined(LP3_COMPILE_TARGET_EMSCRIPTEN)

MediaManager::MediaManager()
:   base_directory("/")
{}

#else

#ifndef LP3_DREAMCAST_MEDIA_PATH
	#error "Please define LP3_DREAMCAST_MEDIA_PATH"
#endif

MediaManager::MediaManager()
:   base_directory(LP3_DREAMCAST_MEDIA_PATH)
{}

#endif

gsl::owner<InputStream *> MediaManager::load(const gsl::cstring_span<> & file) {
    std::string full_path =
		str(boost::format("%s/%s") % base_directory % file.data());
	return new InputFileStream(full_path.c_str());
}


} }  // end lp3::core
