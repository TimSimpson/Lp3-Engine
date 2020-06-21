import conans as c

import os


class Lp3Engine(c.ConanFile):
    name = "Lp3-Engine"
    version = "1.0.1"
    author = "Tim Simpson"
    url = "https://github.com/TimSimpson/Lp3-Engine"

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}

    requires = (
        "Lp3-Main/1.0.5@TimSimpson/testing",
        "Lp3-Sdl/1.0.4@TimSimpson/testing",

        "Catch2/2.11.1@catchorg/stable",
        "glm/0.9.8.5@g-truc/stable",
        "gsl_microsoft/2.0.0@bincrafters/stable",
        "sdl2/2.0.9@bincrafters/stable",
        "sdl2_image/2.0.5@bincrafters/stable",
        "sdl2_ttf/2.0.15@bincrafters/stable",
        "sdl2_mixer/2.0.4@bincrafters/stable",

        # This stuff was brought in exclusively to resolve issues in
        # transitive deps... yikes.
        "bzip2/1.0.8",
        "zlib/1.2.11",
        "libpng/1.6.37",
        "openssl/1.1.1g",
        "glib/2.64.0@bincrafters/stable",
        # "libmodplug/0.8.9.0@bincrafters/stable",
    )

    default_options = {
        "sdl2_mixer:modplug": False,
        "sdl2_mixer:fluidsynth": False,
        "sdl2_mixer:tinymidi": False,
    }

    generators = "cmake_find_package"


    def build(self):
        cmake = c.CMake(self)
        cmake.configure()
        cmake.build()
