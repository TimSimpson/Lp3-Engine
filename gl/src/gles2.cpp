#include <algorithm>
#include <vector>
#include <string.h>
#include <stddef.h>

#include <SDL.h>
#define __SDL2__

#ifdef WIN32
    #define strncpy strncpy_s
    #define strcpy strcpy_s
#endif

#include "gles2._orig_c"
