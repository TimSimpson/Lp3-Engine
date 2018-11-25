import conans as c

import os


def _change(filename, find, replace):
    new_lines = []
    with open(filename, 'r') as f:
        for line in f.readlines():
            new_lines.append(line.replace(find, replace))

    with open(filename, 'w') as f:
        f.write(''.join(new_lines))


def standardize_conan_packages():
    _change("Findcatch2.cmake", "catch2::catch2", "Catch2::Catch2")


class Lp3Engine(c.ConanFile):

    requires = (
        "gsl_microsoft/2.0.0@bincrafters/stable",
        "glm/0.9.8.5@g-truc/stable",
        "sdl2/2.0.8@bincrafters/stable",
        "sdl2_image/2.0.3@bincrafters/stable",
        "sdl2_ttf/2.0.14@bincrafters/stable",
        "catch2/2.4.1@bincrafters/stable"
    )

    generators = "cmake_paths"  # "cmake_find_package"

    def build(self):
        # standardize_conan_packages()

        cmake = c.CMake(self)
        cmake.configure(source_folder=os.path.join(self.source_folder, "core"))
        cmake.build()
