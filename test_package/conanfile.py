import os
import conans

class TestPackage(conans.ConanFile):

    generators = "cmake_find_package"
    settings = "os", "compiler", "build_type", "arch"

    requires = (
    )

    def build(self):
        cmake = conans.CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        pass
