#define LP3_CORE_API_CREATE
#include <lp3/core/utils.hpp>

#include <cstdlib>
#include <wchar.h>
#include <SDL.h>

namespace lp3 { namespace core {

LP3_CORE_API
WCharToCharConverter::WCharToCharConverter(wchar_t const *  original)
:   converted(nullptr)
{
    std::size_t totalLength = wcslen(original);
    std::size_t maxBytes = totalLength + 1;
    converted = new char[maxBytes];
    #ifndef LP3_COMPILE_TARGET_DREAMCAST
    try
    {
    #endif
        #ifdef BOOST_MSVC
            #pragma warning(disable : 4996) // Disable warning on wbstombs
        #endif
        const std::size_t convertedCount
            = wcstombs(converted, original, maxBytes);
        // Could not convert wide character string to classic char string.
        SDL_assert(convertedCount == (maxBytes - 1));
    #ifndef LP3_COMPILE_TARGET_DREAMCAST
    }
    catch(...)
    {
        delete[] converted;
        throw; // Rethrows whatever we caught.
    }
    #endif
}

LP3_CORE_API
WCharToCharConverter::~WCharToCharConverter()
{
    delete[] converted;
}

LP3_CORE_API
char const *  WCharToCharConverter::GetCharPtr()
{
    return converted;
}

} }
