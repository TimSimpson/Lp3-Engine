import conans as c

import os


class Lp3Engine(c.ConanFile):

    requires = (
        "gsl_microsoft/2.0.0@bincrafters/stable",
        "glm/0.9.8.5@g-truc/stable",
        "sdl2/2.0.8@bincrafters/stable",
        "sdl2_image/2.0.3@bincrafters/stable",
        "sdl2_ttf/2.0.14@bincrafters/stable",
        "catch2/2.4.1@bincrafters/stable"
    )

    generators = "cmake"

    def build(self):
        cmake = c.CMake(self)
        cmake.configure()
        cmake.build()
