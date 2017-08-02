// ----------------------------------------------------------------------------
// le.hpp
// ----------------------------------------------------------------------------
//  Defines symbols used to determine configurations.
//
//  Note that the tricks below which determine the platform may need to be
//  updated as time goes by.
//
//  Flags:
//
//   LP3_COMPILE_TARGET_WINDOWS
//   LP3_COMPILE_TARGET_DREAMCAST
//   LP3_COMPILE_TARGET_LINUX
//   LP3_COMPILE_TARGET_PC
//   LP3_COMPILE_TARGET_EMSCRIPTEN
//
//   LP3_COMPILE_WITH_DEBUGGING
//     If true, debug stuff is on, so feel free to take your time.
//
//   LP3_COMPILE_LOG_DISABLE
//     If true, don't use logging even if in debug mode.
//
//   LP3_COMPILE_WITH_PCH
//     If true, try to use precompiled headers.
//
//   LP3_SYMBOL_EXPORT
//     The compiler specific trick to make a library export (only needed for MS).
//
//   LP3_SYMBOL_IMPORT
//     The inverse of the above.
//
//   LP3_COMPILE_DYNAMIC
//     If true, tries to build as dynamic or shared libraries.
//
//   LP3_THROW
//     Use this to throw exceptions.
// ---------------------------------------------------------------------------/
#ifndef LE_H
#define LE_H
#pragma once

#include <boost/config.hpp>

#define COMPILE_TARGET_WINDOWS syntax_error
#define COMPILE_TARGET_DREAMCAST syntax_error
#define COMPILE_TARGET_LINUX syntax_error
#define COMPILE_TARGET_PC syntax_error


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

// ----------------------------------------------------------------------------
// LP3_COMPILE_WITH_PCH
// ----------------------------------------------------------------------------
//    Set when using precompiled headers.
// ----------------------------------------------------------------------------
#if defined(BOOST_BUILD_PCH_ENABLED)
  #define LP3_COMPILE_WITH_PCH
#endif

// ----------------------------------------------------------------------------
// LP3_COMPILE_DYNAMIC
// ----------------------------------------------------------------------------
//     If set, libraries will try to compile dynamically (shared).
// ----------------------------------------------------------------------------
#if defined(BOOST_ALL_DYN_LINK) || defined(BUILD_SHARED_LIBS)
  #define LP3_COMPILE_DYNAMIC
#endif

// ----------------------------------------------------------------------------
// LP3_THROW
// ----------------------------------------------------------------------------
// Throws an exception and includes source location.
// ----------------------------------------------------------------------------
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
