// ----------------------------------------------------------------------------
// main.hpp
// ----------------------------------------------------------------------------
//      Contains a helper macro which defines the main function of a program
//      and calls a method provided by you which accepts a reference to
//      a PlatformLoop. This slightly eases writing portable programs,
//      particularly wrt Emscripten.
//
//      This file should only be included once.
// ----------------------------------------------------------------------------
#ifdef FILE_LP3_MAIN_HPP
    #error "This file should only be included once."
#endif
#define FILE_LP3_MAIN_HPP
#pragma once

#include <lp3/le.hpp>

#if defined(LP3_COMPILE_TARGET_WINDOWS) && defined(LP3_COMPILE_WITH_DEBUGGING)
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#pragma warning(push, 0)       
		#include <crtdbg.h>
	#pragma warning(pop)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include "core/utils.hpp"
#define SDL_MAIN_HANDLED
#include <SDL.h>


#if defined(LP3_COMPILE_TARGET_WINDOWS) && defined(LP3_COMPILE_WITH_DEBUGGING)
	
	#define LP3_MAIN(main_function) \
		int main(int argc, char* argv[]) { \
			_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);	\
			int result; \
			{	\
				lp3::core::PlatformLoop loop(argc, argv); \
				result = main_function(loop); \
			}	\
			if (_CrtDumpMemoryLeaks()) {	\
				__debugbreak();	\
				return 1; \
			}	\
			return result;	\
		}
#else
	#define LP3_MAIN(main_function) \
		int main(int argc, char* argv[]) { \
			lp3::core::PlatformLoop loop(argc, argv); \
			return main_function(loop); \
		}
#endif


