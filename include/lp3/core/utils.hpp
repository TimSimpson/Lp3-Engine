#ifndef LP3_CORE_UTILS_HPP
#define LP3_CORE_UTILS_HPP
#pragma once

#include <functional>
#include <vector>
#include <boost/any.hpp>
#include <boost/optional.hpp>
#include <gsl/gsl>
#include "config.hpp"

namespace lp3 { namespace core {

// --------------------------------------------------------------------
// get_env_var
// --------------------------------------------------------------------
//	   Returns an environment variable.
// --------------------------------------------------------------------
LP3_CORE_API
boost::optional<std::string> get_env_var(const gsl::cstring_span<> & value);


// --------------------------------------------------------------------
// class PlatformLoop
// --------------------------------------------------------------------
//     Contains code to run the main loop on any given platform.
//	   While it's possible to create this yourself, you should instead
//	   use the macro LP3_MAIN from main.hpp which creates this for you.
// --------------------------------------------------------------------
LP3_CORE_API
class PlatformLoop {
public:
	PlatformLoop(int argc, char ** argv);

    PlatformLoop(const PlatformLoop & other) = delete;
    PlatformLoop & operator=(const PlatformLoop & other) = delete;

    std::vector<std::string> command_line_args() const;

    int run(std::function<bool()> iterate);

private:
    std::vector<std::string> arguments;
};

// --------------------------------------------------------------------
// class PlatformInitArgs
// --------------------------------------------------------------------
//     Contains platform specific arguments passed to main which are
//     type-erased.
// 	   The main goal is to allow the vast majority of code to avoid the
//     dastardly windows.h (or similar native headers). Platform
//     specific code can use platform.hpp to unwrap the native
//     arguments it needs.
// --------------------------------------------------------------------
LP3_CORE_API
class PlatformInitArgs {
protected:
	boost::any hinstance;
	boost::any hwnd;
};

// --------------------------------------------------------------------
// class PlatformMessage
// --------------------------------------------------------------------
//     Contains platform specific arguments passed to the event handler
//     function which are type-erased.
// 	   The main goal is to allow the vast majority of code to avoid the
//     dastardly windows.h (or similar native headers). Platform
//     specific code can use platform.hpp to unwrap the native
//     arguments it needs.
// --------------------------------------------------------------------
LP3_CORE_API
class PlatformMessage {
protected:
	boost::any hwnd;
	boost::any message;
	boost::any w_param;
	boost::any l_param;
};


// --------------------------------------------------------------------
// class WCharToCharConverter
// --------------------------------------------------------------------
//     Converts wchar_t to simple const char *. Owns the converted
//     char buffer.
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
