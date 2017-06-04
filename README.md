# LP3 Engine vII

TODO: FIX [![Build status](https://ci.appveyor.com/api/projects/status/a3r2jq32f1x6frcv?svg=true)](https://ci.appveyor.com/project/TimSimpson/lp3-core)
TODO: FIX [![Build Status](https://travis-ci.org/TimSimpson/Lp3-Core.svg?branch=master)](https://travis-ci.org/TimSimpson/Lp3-Core)

This is a helper library for writing games which sits atop SDL2, OpenGL, glm, imgui and a few other components.

It is organized as a series of CMake projects designed to work as subprojects of a parent project.

## Building

This library is built with CMake, and tries to adhere to the some rules described in [The Ultimate Guide to Modern CMake](https://rix0r.nl/blog/2015/08/13/cmake-guide/). Specifically effort is made to avoid trying to find it's own dependencies so that they can be specified by a parent project instead.

For an example of what this looks like please see [the parent project used for CI](standalone/CMakeLists.txt).

### Dependencies / Requirements

* The [Guideline Support Library](https://github.com/Microsoft/GSL), defined as the imported interface library `gsl`.
* [The Boost libraries](http://www.boost.org/) (header only) must be defined as the imported interface library `boost`.
* [Simple Direct Media Layer (SDL) 2](https://www.libsdl.org/download-2.0.php), defined as the imported interface library `lp3_deps::sdl`.
* [SDL 2 image](https://www.libsdl.org/projects/SDL_image/docs/SDL_image.html#SEC_Top), defined as the imported interface library `sdl_image`.
* [SDL 2 TTF](https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf.html#SEC_Top), defined as the imported interface library `sdl_ttf`.
* [Catch](https://github.com/philsquared/Catch), defined as the imported interface library `catch`.
* [OpenGL Mathematics](http://glm.g-truc.net/0.9.8/index.html), defined as the imported interface library `glm`.