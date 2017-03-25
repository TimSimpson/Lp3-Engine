#ifndef LP3_CORE_UTILS_HPP
#define LP3_CORE_UTILS_HPP

#include <boost/any.hpp>
#include "config.hpp"

namespace lp3 { namespace core {

// Forward declaration of a struct defined in platform.hpp
struct Platform;

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
	friend Platform;
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
	friend Platform;
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
