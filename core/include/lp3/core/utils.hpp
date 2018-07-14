#ifndef LP3_CORE_UTILS_HPP
#define LP3_CORE_UTILS_HPP
#pragma once

#include <functional>
#include <vector>
#include <boost/any.hpp>
#include <boost/optional.hpp>
#include <gsl/gsl>
#include "config.hpp"
#include "../sdl.hpp"


namespace lp3 { namespace core {


// --------------------------------------------------------------------
// class PlatformLoop
// --------------------------------------------------------------------
//     Contains code to run the main loop on any given platform.
//     While it's possible to create this yourself, you should instead
//     use the macro LP3_MAIN from <lp3/main.hpp> which creates this
//     for you.
// --------------------------------------------------------------------
class
LP3_CORE_API
PlatformLoop {
public:
    PlatformLoop();

    PlatformLoop(int argc, char ** argv);

    PlatformLoop(const PlatformLoop & other) = delete;
    PlatformLoop & operator=(const PlatformLoop & other) = delete;

    std::vector<std::string> command_line_args() const;

    int run(std::function<bool()> iterate);

private:
    std::vector<std::string> arguments;
};

// --------------------------------------------------------------------
// get_env_var
// --------------------------------------------------------------------
//     Returns an environment variable.
// --------------------------------------------------------------------
LP3_CORE_API
boost::optional<std::string> get_env_var(const gsl::cstring_span<> & value);

// ----------------------------------------------------------------------------
// class MediaManager
// ----------------------------------------------------------------------------
//     A factory for the stream classes, manages where to find media files.
//     Currently this just forwards to SDL's calls to open a file; the premise
//     is that in the future it could be something more complex, such as a
//     file which is a collection of resources.
// ----------------------------------------------------------------------------
class
LP3_CORE_API
MediaManager {
public:
    MediaManager();

#ifndef LP3_COMPILE_TARGET_PC
private:  // Private, except on the PC where a base directory can be set.
#endif

    MediaManager(const std::string & _base_directory);

public:
    /* Opens a resource for reading. */
	sdl::RWops load(const gsl::cstring_span<> & file);

	/* Returns absolute path to the given file. */
	std::string path(const gsl::cstring_span<> & file) const;

	/* Opens a resource for writing. */
	sdl::RWops save(const gsl::cstring_span<> & file);

    /* Opens a new Media Manager in a sub directory. */
    MediaManager sub_directory(const gsl::cstring_span<> & sub_directory);

private:
    const std::string base_directory;
};

// ~see-file "../../../tests/media_tests.cpp"

// --------------------------------------------------------------------
// class WCharToCharConverter
// --------------------------------------------------------------------
//     Converts wchar_t to simple const char *. Owns the converted
//     char buffer. Good for some bits of Windows code.
// --------------------------------------------------------------------
class
LP3_CORE_API
WCharToCharConverter
{
public:
	WCharToCharConverter(wchar_t const *  original);
	~WCharToCharConverter();
	char const *  GetCharPtr();
private:
	char *  converted;
};


}	}

#endif
