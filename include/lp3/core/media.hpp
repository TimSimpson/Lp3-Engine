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
#include "../sdl.hpp"


namespace lp3 { namespace core {


// ----------------------------------------------------------------------------
// class MediaManager
// ----------------------------------------------------------------------------
//     A factory for the stream classes, manages where to find media files.
//	   Currently this just forwards to SDL's calls to open a file; the premise
//	   is that in the future it could be something more complex, such as a 
//     file which is a collection of resources.
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

    /* Opens a resource for reading. */
	sdl::RWops load(const gsl::cstring_span<> & file);

	/* Returns absolute path to the given file. */
	std::string path(const gsl::cstring_span<> & file) const;

	/* Opens a resource for writing. */
	sdl::RWops save(const gsl::cstring_span<> & file);
private:
    const std::string base_directory;
};

} }

#endif
