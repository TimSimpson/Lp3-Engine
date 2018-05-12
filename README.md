# LP3 Engine vII

[![Build Status](https://travis-ci.org/TimSimpson/Lp3-Engine.svg?branch=master)](https://travis-ci.org/TimSimpson/Lp3-Engine)
[![Build status](https://ci.appveyor.com/api/projects/status/hro698ynw2t40014?svg=true)](https://ci.appveyor.com/project/TimSimpson/lp3-engine)

This is a series of helper libraries for writing games in C++. There are a million of those, so why create one of those?

Check out these amazing list of features:

* Written by me.
* Bare minimum functionality layered on top of the SDL2, OpenGL, and imgui. It's mostly just wrapper classes to manage resources with RAII and throw exceptions in the event of errors.
* Some focus on Emscripten support. In particular, the OpenGL stuff uses generated bindings to OpenGL ES 2, which is useful if you want to make sure your code works with Emscripten.
* Many libraries require no other dependencies beyond the SDL2 for logging and asserts. So the input and mixer libraries could theoretically be used with code using Direct3D.
* Not a framework. Tries hard to avoid Byzantine initialization rituals.
* A very rudimentary logging system for debug builds only that's compatable with [Squirrel Console](https://github.com/TimSimpson/SquirrelConsole) on Windows. Otherwise it just calls SDL2's logging facilities.
* Portable facilities for writing the `main` function and application loop in Emscripten, Windows and Linux.
* A class for deterministically running game logic in the main loop.
* Simple stateless coroutines that use macros and are implemented as switch statement based state machines but are easier to write and read.
* A super rudimentary event system.
* `input` namespace that makes it very easy to write a game with highly configurable controls.


## Using from CMake

### Typical Usage with `find_package`

This repo follows modern CMake standards. If you're a crazy person who installs dependencies globally just run cmake install against the CMakeLists.txt in the root of this repository to install all of the libraries. Of course, this will fail if you haven't installed the various dependencies...

But, if you're an elegant and sophisticated person you probably use [cget](http://cget.readthedocs.io) and maybe [cenv](https://github.com/TimSimpson/cenv), so just install it like this:

    cget install TimSimpson/Lp3-Engine@master

... and all the dependencies specified in the requirements.txt file will be installed automatically.

>> Note: Boost is also required. It's not in the requirements file, because it's big, and I don't use it with cget yet. So... whoops.

Once you've got the libraries installed, you can use them from your own project by calling `find_package(Lp3_LibraryName)` and then referencing the target `lp3::library_name` (as in `target_link_libraries(your_lib PUBLIC lp3::library_name)` ).


### Integrated Builds with Super Projects

If you're the kind of person who likes super repos that pull in Git modules and stuff, you can use these libraries too! Simply include the various CMakeLists.txt in the directory corresponding to the library you want, and use [Daniel Pfeifer's `find_package` macro trick](https://www.youtube.com/watch?v=bsXLMQ6WgIk) to avoid pulling in the installed library as a dependency. See [the `standalone` super project](`standalone/CMakeLists.txt`) (used by this repo in CI) for an example.


## Library Overview

* lp3::core - Includes logging and assert macros used by other projects as well as SDL2 wrappers. Everything else depends on this, but it's pretty tiny.
* lp3::sims - Classes for writing game logic. Features a game clock class for running game code deterministically and primitive but powerful macro based coroutines.
* lp3::input - Has code for a virtual controller whose buttons can be remapped to different physical buttons. In other words you can just start using this and then easily write code in some config menu in your game that allows for the controls to be remapped.
* lp3::gl - Offers OpenGL bindings and very basic RAII classes.
* lp3::gfx - Offers some basic stuff on top of OpenGL. Mostly for my own benefit, there's some basic shader programs and stuff in here, as well as helpers like code to keep the aspect ratio on Windows when they're being resized. If you're well-versed in OpenGl your needs are best met elsewhere.
* lp3::imgui - Very simple wrappings for ImGui. It's main strength is just bringing in a CMake project for it.
* lp3::mix - Simple wrapper for SDL2::mixer.


### Dependencies / Requirements

Again, [cget](http://cget.readthedocs.io) installs all this stuff automatically if you just run `cget install -f requirements.txt` (note: except for Boost, which you'll download yourself. Don't worry, Lp3 only uses the header only libraries).

* The [Guideline Support Library](https://github.com/Microsoft/GSL), defined as the imported interface library `gsl`.
* [The Boost libraries](http://www.boost.org/) (header only) must be defined as the imported interface library `boost`.
* [Simple Direct Media Layer (SDL) 2](https://www.libsdl.org/download-2.0.php), defined as the imported interface library `lp3_deps::sdl`.
* [SDL 2 image](https://www.libsdl.org/projects/SDL_image/docs/SDL_image.html#SEC_Top), defined as the imported interface library `sdl_image`.
* [SDL 2 TTF](https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf.html#SEC_Top), defined as the imported interface library `sdl_ttf`.
* [SDL mixer (version 1 or 2)](https://www.libsdl.org/projects/SDL_mixer/), defined as the imported interface library `sdlx_mixer`.
* [Catch](https://github.com/philsquared/Catch), defined as the imported interface library `catch`.
* [OpenGL Mathematics](http://glm.g-truc.net/0.9.8/index.html), defined as the imported interface library `glm`.
