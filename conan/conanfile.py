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

        "gsl_microsoft/2.0.0@bincrafters/stable",
        "glm/0.9.8.5@g-truc/stable",
        "sdl2/2.0.9@bincrafters/stable",
        "zlib/1.2.11",
        "libpng/1.6.37",
        "sdl2_image/2.0.5@bincrafters/stable",
        "sdl2_ttf/2.0.15@bincrafters/stable",
        "Catch2/2.11.1@catchorg/stable"
    )

    generators = "cmake_find_package"

    def build(self):
        cmake = c.CMake(self)
        cmake.configure()
        cmake.build()
