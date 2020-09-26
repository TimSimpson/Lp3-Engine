import conans as c

import os


class Lp3Engine(c.ConanFile):
    name = "lp3-engine"
    version = "1.0.2"
    author = "Tim Simpson"
    url = "https://border-town.com/projects/Lp3-Engine/index.html"
    description = "Game Stuff"

    url = "https://github.com/TimSimpson/Lp3-Engine"

    settings = "os", "compiler", "build_type", "arch"

    options = {
        "shared": [True, False],
        "use_basic_sdl2": [True, False],
    }
    default_options = {
        "shared": False,
        "use_basic_sdl2": False,
    }


    requires = (
        "lp3-main/1.0.7@TimSimpson/testing",
        "lp3-sdl/1.0.5@TimSimpson/testing",
        "lp3-sims/1.0.1@TimSimpson/testing",
        "glm/0.9.8.5@g-truc/stable",
        "gsl_microsoft/2.0.0@bincrafters/stable",
        "boost/1.71.0@conan/stable",

        # This stuff was brought in exclusively to resolve issues in
        # transitive deps... yikes.
        # "bzip2/1.0.8",
        # "zlib/1.2.11",
        # "libpng/1.6.37",
        # "openssl/1.1.1g",
        # "glib/2.64.0@bincrafters/stable",
    )

    test_requires = [
        "Catch2/2.11.1@catchorg/stable"
    ]


    def get_sdl2_dependencies(self):
        if not self.options.use_basic_sdl2:
            return (
                "sdl2/2.0.9@bincrafters/stable",
                "sdl2_image/2.0.5@bincrafters/stable",
                "sdl2_ttf/2.0.15@bincrafters/stable",
                "sdl2_mixer/2.0.4@bincrafters/stable",
                # This stuff was brought in exclusively to resolve issues in
                # transitive deps... yikes.
                "bzip2/1.0.8",
                "zlib/1.2.11",
                "libpng/1.6.37",
                # "openssl/1.1.1g",
                # "glib/2.64.0@bincrafters/stable",
            )
        else:
            return (
                "sdl2/b_2.0.9@TimSimpson/testing",
                "sdl2_image/b_2.0.5@TimSimpson/testing",
                "sdl2_ttf/b_2.0.15@TimSimpson/testing",
                "sdl2_mixer/b_2.0.4@TimSimpson/testing",
            )

    @property
    def tests_enabled(self):
        return (
            self.develop
            and (os.environ.get("CONAN_SKIP_TESTS") or "").lower() != 'true'
        )

    def requirements(self):
        for d in self.get_sdl2_dependencies():
            self.requires(d)
        if self.tests_enabled:
            for tr in self.test_requires:
                self.requires(tr)

    generators = "cmake_find_package", "virtualrunenv"

    exports_sources = (
        "src/*", "include/*", "demos/*", "tests/*", "CMakeLists.txt", "Lp3EngineTargets.cmake",
        "core/*",
        "gfx/*",
        "gl/*",
        "imgui/*",
        "input/*",
        "mix/*",
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

    def _set_lib(self, pkg_name, lib_file_name, requires=None):
        _set_for_cmake(self.cpp_info.components[pkg_name].names, pkg_name)
        self.cpp_info.components[pkg_name].libs = [ lib_file_name ]
        if requires:
            self.cpp_info.components[pkg_name].requires = requires


    def package_info(self):
        self.cpp_info.name = "lp3-engine"
        # self.cpp_info.requires = [ 'lp3-sims::sims' ]
        _set_for_cmake(self.cpp_info.filenames, "lp3-engine")
        _set_for_cmake(self.cpp_info.names, "lp3")
        self._set_lib('core', 'Lp3_Core', [
            'lp3-main::main',
            'lp3-sdl::sdl',
            'gsl_microsoft::gsl_microsoft',
            'boost::boost',
            'sdl2::sdl2',
            'sdl2_image::sdl2_image',
        ])
        self._set_lib('gl', 'Lp3_Gl', [ 'core', 'glm::glm' ])
        self._set_lib('gfx', 'Lp3_Gfx', [ 'core', 'gl', 'glm::glm', 'sdl2_ttf::sdl2_ttf'])
        self._set_lib('imgui', 'Lp3_ImGui', [ 'gfx'] )
        self._set_lib('input', 'Lp3_Input', [ 'core' ])
        self._set_lib('mix', 'Lp3_Mix', [ 'core', 'sdl2_mixer::sdl2_mixer'])

        self.cpp_info.components['sims'].requires = [ 'lp3-sims::sims' ]

        # I don't understand why Conan requires things to use _every_ package
        # I'm adding as a requirement even when I'm only putting it there
        # because I have to solve a conflict between two dependencies which
        # both require different versions... ugh.
        # Anyway, that's what this next bit of code is doing.
        if not self.options.use_basic_sdl2:
            self.cpp_info.components['fake'].requires = [
                'zlib::zlib',
                'bzip2::bzip2',
                'libpng::libpng'
            ]

        # _set_for_cmake(self.cpp_info.components['core'].names, "core")
        # self.cpp_info.components['core'].libs = [ "lp3-main" ]

        # self.cpp_info.name = "lp3-engine"
        # self.cpp_info.libs = [
        #     "Lp3_Core",
        #     "Lp3_Gl",
        #     "Lp3_Gfx",
        #     "Lp3_ImGui",
        #     "Lp3_Input",
        #     "Lp3_Mix",
        # ]
        # TODO: sort out with components, somehow
        # self.cpp_info.requires = [ "Lp3-Main" ]
        # self.cpp_info.components["core"].libs = ["Lp3_Core"]
        # self.cpp_info.components["core"].requires = ["Lp3-Main"]
        # self.cpp_info.components["core"].requires = ["Boost::boost"]
        # self.cpp_info.components["core"].requires = ["sdl2::image"]

def _set_for_cmake(attr, value):
    for generator in ['cmake_find_package', 'cmake_find_package_multi']:
        attr[generator] = value

