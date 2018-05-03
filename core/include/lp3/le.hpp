// ----------------------------------------------------------------------------
// <lp3/le.hpp>
// ===========================================================================
//  Defines symbols used to determine configurations.
//
//  Note that the tricks below which determine the platform may need to be
//  updated as time goes by.
//
// Configuration Flags
// ----------------------------------------------------------------------------
//
//  These flags will be set depending on the platform and configuration of the
//  build. In practice they're useful mainly to internal LP3 code, though
//  some flags - such as ones dealing with logging options - can be set by
//  consumers of the code.
//
//  LP3_COMPILE_TARGET_WINDOWS
//      True if the target platform is Windows.
//
//  LP3_COMPILE_TARGET_DREAMCAST
//      True if the target platform is the Dreamcast.
//
//  LP3_COMPILE_TARGET_LINUX
//      True if the target platform is Linux.
//
//  LP3_COMPILE_TARGET_PC
//      True if the target platform is a computer.
//
//  LP3_COMPILE_TARGET_EMSCRIPTEN
//      True if the target platform is Javascript.
//
//  LP3_COMPILE_TARGET_POCKETCHIP
//      True if the target platform is the C.H.I.P.
//
//  LP3_COMPILE_WITH_DEBUGGING
//      If true, debug stuff is on, so feel free to take your time.
//
//  LP3_COMPILE_LOG_DISABLE
//      If true, don't use logging even if in debug mode.
//
//  LP3_COMPILE_WITH_PCH
//      If true, try to use precompiled headers.
//
//  LP3_SYMBOL_EXPORT
//      The compiler specific trick to make a library export (only needed for MS).
//
//  LP3_SYMBOL_IMPORT
//      The inverse of the above.
//
//  LP3_COMPILE_DYNAMIC
//      If true, tries to build as dynamic or shared libraries.
// ---------------------------------------------------------------------------/
#ifndef LE_H
#define LE_H
#pragma once

#include <boost/config.hpp>


#ifdef _WIN32
#ifndef LP3_COMPILE_TARGET_WINDOWS
    #define LP3_COMPILE_TARGET_WINDOWS
    // _DEBUG is a Microsoft specific flag for debug builds. Boost
    // seems to add it for any Windows builds.
    #ifdef _DEBUG
        #define LP3_COMPILE_WITH_DEBUGGING
    #endif
#endif
#endif

#ifdef LP3_COMPILE_TARGET_WINDOWS
    #define LP3_COMPILE_TARGET_WINDOWS_THEN(...) __VA_ARGS__
#else
    #define LP3_COMPILE_TARGET_WINDOWS_THEN(...) /* __VA_ARGS__ */
#endif

#ifdef LP3_COMPILE_TARGET_EMSCRIPTEN
  #ifndef NDEBUG
        #define LP3_COMPILE_WITH_DEBUGGING
  #endif
#endif

#ifdef _arch_dreamcast
  #ifndef LP3_COMPILE_TARGET_DREAMCAST
    #define LP3_COMPILE_TARGET_DREAMCAST
  #endif
  // The following is used to turn off the asserts in stdlib. It's
  // GCC specific but that's the only compiler for Dreamcast anyway.
  #ifndef NDEBUG
        #define LP3_COMPILE_WITH_DEBUGGING
  #endif
#endif


#ifndef LP3_COMPILE_TARGET_DREAMCAST
  #ifndef LP3_COMPILE_TARGET_WINDOWS
    #ifndef LP3_COMPILE_TARGET_EMSCRIPTEN
      #ifndef LP3_COMPILE_TARGET_LINUX
        #define LP3_COMPILE_TARGET_LINUX
        #ifndef NDEBUG
          #define LP3_COMPILE_WITH_DEBUGGING
        #endif
      #endif
    #endif
  #endif
#endif

#ifndef LP3_COMPILE_TARGET_PC
  #if defined(LP3_COMPILE_TARGET_LINUX) || defined(LP3_COMPILE_TARGET_WINDOWS)
    #define LP3_COMPILE_TARGET_PC
  #endif
#endif

#ifndef LP3_COMPILE_WITH_DEBUGGING
    #define LP3_COMPILE_LOG_DISABLE
#endif


#ifdef BOOST_TEST_MODULE
  #define LP3_COMPILE_TARGET_TEST
#endif


#define LP3_SYMBOL_EXPORT BOOST_SYMBOL_EXPORT
#define LP3_SYMBOL_IMPORT BOOST_SYMBOL_IMPORT

// LP3_COMPILE_WITH_PCH
#if defined(BOOST_BUILD_PCH_ENABLED)
  #define LP3_COMPILE_WITH_PCH
#endif

// LP3_COMPILE_DYNAMIC
#if defined(BOOST_ALL_DYN_LINK) || defined(BUILD_SHARED_LIBS)
  #define LP3_COMPILE_DYNAMIC
#endif

// ----------------------------------------------------------------------------
// LP3_THROW(t)
// ----------------------------------------------------------------------------
// Given a type, constructs and throws it.
// In release builds for some platforms this may trigger std::abort instead.
// The main utility of this then is to throw specific exception types which
// can be looked for when testing.
//
// Example:
//
// .. code-block:: c++
//
//    LP3_THROW(lp3::Exception)
// ---------------------------------------------------------------------------/

// ----------------------------------------------------------------------------
// LP3_THROW2(t, ...)
// ----------------------------------------------------------------------------
// Identical to LP3_THROW but provides for arguments to be passed to the
// exception type's constructor.
//
// Example:
//
// .. code-block:: c++
//
//    class FileNotFoundError {
//    public:
//        FileNotFoundError(const char * const filename);
//    };
//
//    LP3_THROW2(FileNotFoundError, "somefile.txt")
// ---------------------------------------------------------------------------/
#ifndef LP3_COMPILE_TARGET_DREAMCAST
  #define LP3_THROW(t) { throw t{}; }
  #define LP3_THROW2(t, ...) { throw t(__VA_ARGS__); }
#else
  // Things are much more complicated in Dreamcast land... :(
  #define LP3_THROW_EXCEPTION_TO_STRING(type) #type
  #ifndef LP3_COMPILE_TARGET_TEST
     #define LP3_THROW(t) { \
        printf("\n!!!!\tCRITICAL ERROR:\n!!!!\t\t\t%s\n", LP3_THROW_EXCEPTION_TO_STRING(t)); \
        printf("!!!!\tOn %s, line %d\n\n", __FILE__, __LINE__); \
        abort(); \
     }
     #define LP3_THROW2(t, ...) LP3_THROW(t)
  #else
     #define LP3_THROW(t { \
        Lp3::DreamcastTestAsserts::lastExceptionType=LP3_THROW_EXCEPTION_TO_STRING(t); \
      }
      #define LP3_THROW2(t, ...) LP3_THROW(t)
  #endif
#endif



#endif
