#ifndef LP3_CORE_MEDIA_HPP
#define LP3_CORE_MEDIA_HPP

#include "config.hpp"
#ifdef BOOST_MSVC
    #pragma warning(push, 0)
#endif
#include <gsl/gsl>
#ifdef BOOST_MSVC
    #pragma warning(pop)
#endif
#include <boost/optional.hpp>
#include "../assert.hpp"

namespace lp3 { namespace core {

// ----------------------------------------------------------------------------
// class InputStream
// ----------------------------------------------------------------------------
//     Simple interface for reading in files or other resources.
// ----------------------------------------------------------------------------
class
LP3_CORE_API
InputStream
{
public:
    virtual ~InputStream() = default;

    /** Check this before reading. */
    virtual bool eof() const = 0;

    /** Reads requested number of bytes into the buffer. Returns bytes read. */
    virtual std::size_t read(gsl::span<char> buffer) = 0;

    template<typename T>
    inline std::size_t read(gsl::span<T> buffer) {
        return read(
            gsl::span<char>(reinterpret_cast<char *>(buffer.data()),
                            buffer.size_bytes()));
    }

    template<typename T>
    inline std::size_t read(T * buffer, std::size_t buffer_size) {
        return read(gsl::span<T>(buffer, buffer_size));
    }

    /** Reads current bytes into a simple data structure. */
    template<typename T>
    inline void read_pod(T & data)
    {
        char * as_chars = reinterpret_cast<char *>(&data);
        gsl::span<char, sizeof(T)> buffer(as_chars, sizeof(T));
        #ifdef LP3_COMPILE_WITH_DEBUGGING
            const size_t bytes_read =
        #endif
            read(buffer);
        #ifdef LP3_COMPILE_WITH_DEBUGGING
        LP3_ASSERT_TRUE_MESSAGE(sizeof(T) == bytes_read,
                                "Unable to read all bytes for struct!");
        #endif
    }

    // Go back to the start of the file.
    virtual void reset(size_t position=0) = 0;

    /** Skips ahead the number of bytes. */
    virtual void skip(size_t bytes) = 0;
};


// ----------------------------------------------------------------------------
// class MediaManager
// ----------------------------------------------------------------------------
//     A factory for the stream classes, manages where to find media files.
// ----------------------------------------------------------------------------
class
LP3_CORE_API
MediaManager {
public:
    MediaManager();

    #ifdef LP3_COMPILE_TARGET_PC
        /* On PC, the base directory can be set. */
        MediaManager(const std::string & _base_directory);
    #endif

    /* Opens a file for reading. */
    gsl::owner<InputStream *> load(const gsl::cstring_span<> & file);

	std::unique_ptr<InputStream> load_uptr(const gsl::cstring_span<> & file) {
		std::unique_ptr<InputStream> up(load(file));
		return up;
	}

	/* Returns absolute path to the given file. */
	std::string path(const gsl::cstring_span<> & file) const;
private:
    const std::string base_directory;
};

} }

#endif
