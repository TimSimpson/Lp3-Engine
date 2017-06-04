# Lp3 Sims

[![Build status](https://ci.appveyor.com/api/projects/status/k1v4yvke6if7goft?svg=true)](https://ci.appveyor.com/project/TimSimpson/lp3-sims)
[![Build Status](https://travis-ci.org/TimSimpson/Lp3-Sims.svg?branch=master)](https://travis-ci.org/TimSimpson/Lp3-Sims)

Code for hndling simulations. Contains naive implementations for:

* stateless [coroutines](include/lp3/sims/coroutine.hpp)
* a [game clock](include/lp3/sims/Clock.hpp), which simplifies writing code that runs at the same speed regardless of the performance of the underlying hardware.
* [timers](include/lp3/sims/Clock.hpp) to measure FPS
* a super slow [event system](include/lp3/sims/EventSimulator.hpp) which handles dispatching to arbitrary functions using integral codes as well as causing code to fire at a specific time (this def. shouldn't be used in a tight loop and possibly shouldn't be used at all!)

## Building

This library is built with CMake, and tries to adhere to the some rules described in [The Ultimate Guide to Modern CMake](https://rix0r.nl/blog/2015/08/13/cmake-guide/). Specifically effort is made to avoid trying to find it's own dependencies so that they can be specified by a parent project instead.

For an example of what this looks like please see [the parent project used for CI](standalone/CMakeLists.txt).

### Dependencies / Requirements

* The [Guideline Support Library](https://github.com/Microsoft/GSL), defined as the imported interface library `gsl`.
* [The Boost libraries](http://www.boost.org/) (header only) must be defined as the imported interface library `boost`.
* [Simple Direct Media Layer (SDL) 2](https://www.libsdl.org/download-2.0.php), defined as the imported interface library `lp3_deps::sdl`.
* [SDL 2 image](https://www.libsdl.org/projects/SDL_image/docs/SDL_image.html#SEC_Top), defined as the imported interface library `sdl_image`.
* [Catch](https://github.com/philsquared/Catch), defined as the imported interface library `catch`.
* [Lp3-Core](https://github.com/TimSimpson/Lp3-Core), defined as the imported interface library `Lp3-Core`.