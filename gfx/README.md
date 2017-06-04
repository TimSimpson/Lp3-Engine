# Lp3 Gfx

[![Build status](https://ci.appveyor.com/api/projects/status/yv28ei69ehsvmpg4?svg=true)](https://ci.appveyor.com/project/TimSimpson/lp3-gfx)
[![Build Status](https://travis-ci.org/TimSimpson/Lp3-Gfx.svg?branch=master)](https://travis-ci.org/TimSimpson/Lp3-Gfx)

Limited helper utilities for OpenGL and SDL2.

Features:

* Helper tool to capture a font with SDL2_TTF and save it as a special file, which can be loaded back and drawn to the screen using textures in OpenGL.
* World's worst shader code

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
* [Lp3-Core](https://github.com/TimSimpson/Lp3-Core), defined as the imported interface library `Lp3-Core`.
* [Lp3-Sims](https://github.com/TimSimpson/Lp3-Sims), defined as the imported interface library `Lp3-Sims`.
* [Lp3-Gl](https://github.com/TimSimpson/Lp3-Gl), defined as the imported interface library `Lp3-Gl`.