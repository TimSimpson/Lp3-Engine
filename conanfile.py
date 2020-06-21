import conans as c

import os


class Lp3Engine(c.ConanFile):
    name = "lp3-engine"
    version = "1.0.1"
    author = "Tim Simpson"
    url = "https://github.com/TimSimpson/Lp3-Engine"

    settings = "os", "compiler", "build_type", "arch"

    requires = (
        "Lp3-Main/1.0.5@TimSimpson/testing",
        "Lp3-Sdl/1.0.4@TimSimpson/testing",
        "glm/0.9.8.5@g-truc/stable",
        "gsl_microsoft/2.0.0@bincrafters/stable",
        "boost/1.71.0@conan/stable",
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

        # TODO: make project not run tests in packaging mode... maybe?
        "Catch2/2.11.1@catchorg/stable"
    )

    test_requires = [
        # "Catch2/2.11.1@catchorg/stable"
    ]

    default_options = {
        "sdl2_mixer:modplug": False,
        "sdl2_mixer:fluidsynth": False,
        "sdl2_mixer:tinymidi": False,
    }

    @property
    def tests_enabled(self):
        return (
            self.develop
            and (os.environ.get("CONAN_SKIP_TESTS") or "").lower() != 'true'
        )

    def build_requirements(self):
        if self.tests_enabled:
            for tr in self.test_requires:
                self.build_requires(tr)

    generators = "cmake_find_package"

    exports_sources = (
        "src/*", "include/*", "demos/*", "tests/*", "CMakeLists.txt",
        "core/*",
        "gfx/*",
        "gl/*",
        "imgui/*",
        "input/*",
        "mix/*",
        "sims/*",
    )

    def _configed_cmake(self):
        cmake = c.CMake(self)
        cmake.configure(defs={
            "LP3_ENGINE_Build_Tests": self.tests_enabled,
        })
        return cmake

    def build(self):
        cmake = self._configed_cmake()
        cmake.build()

    def package(self):
        cmake = self._configed_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.name = "lp3-engine"

        # TODO: sort out with components, somehow

        # self.cpp_info.components["core"].libs = ["lp3core"]
        # self.cpp_info.components["core"].defines = ["DEFINE_CRYPTO=1"]
        # self.cpp_info.components["core"].requires = ["zlib::zlib"]  # Depends on all components in zlib package

        # self.cpp_info.components["ssl"].names["cmake"] = "SSL"
        # self.cpp_info.components["ssl"].includedirs = ["include/headers_ssl"]
        # self.cpp_info.components["ssl"].libs = ["libssl"]
        # self.cpp_info.components["ssl"].requires = ["crypto",
        #                                             "boost::headers"]  # Depends on
