# *********************************************************************
# LP3_MIX
#       Convenience stuff for SDL Mixer
# *********************************************************************
project(lp3_mix CXX)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
include(CMakePackageConfigHelpers)

find_package(lp3-main REQUIRED)
find_package(lp3-sdl REQUIRED)
find_package(lp3-sims REQUIRED)

if(NOT "${CMAKE_SYSTEM_NAME}" MATCHES "Emscripten")
    find_package(sdl2_mixer REQUIRED)
else()
    add_library(sdl2_mixer::sdl2_mixer INTERFACE IMPORTED)
    target_compile_options(sdl2_mixer::sdl2_mixer INTERFACE "SHELL:-s -s USE_SDL_MIXER=2")
    target_link_options(sdl2_mixer::sd2_mixer INTERFACE "SHELL:-s -s USE_SDL_MIXER=2")
endif()

add_library(mix
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/mix.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/mix/config.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/mix.cpp
)
# Mandate the use of at least C++17 by everything that uses this
target_compile_features(mix PUBLIC cxx_std_17)
set_target_properties(mix PROPERTIES OUTPUT_NAME "Lp3_Mix")

target_include_directories(mix
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src)

target_link_libraries(mix PUBLIC sdl2_mixer::sdl2_mixer lp3::core )

# *********************************************************************
# Package / Install Stuff
# *********************************************************************

install(DIRECTORY include/ DESTINATION include)

install(TARGETS mix EXPORT Lp3_Mix-targets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)

add_library(lp3::mix ALIAS mix)
install(EXPORT Lp3_Mix-targets
    FILE Lp3_Mix-targets.cmake
    NAMESPACE lp3::
    DESTINATION lib/cmake/Lp3_Mix
)

file(WRITE "${PROJECT_BINARY_DIR}/Lp3_Mix-config.cmake" "
include(CMakeFindDependencyMacro)
include(\"\${CMAKE_CURRENT_LIST_DIR}/Lp3_Mix-targets.cmake\")
")

write_basic_package_version_file("${PROJECT_BINARY_DIR}/Lp3_Mix-config-version.cmake"
    VERSION 1.0.1
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${PROJECT_BINARY_DIR}/Lp3_Mix-config.cmake"
    "${PROJECT_BINARY_DIR}/Lp3_Mix-config-version.cmake"
    DESTINATION lib/cmake/Lp3_Mix
)

# *********************************************************************
# Tests and Drivers / Demos
# *********************************************************************

get_directory_property(is_child PARENT_DIRECTORY)
if (NOT is_child)
    set(LP3_MIX_FULL True)
endif()

if (LP3_MIX_FULL)
    add_executable(mix_demo WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/demos/mix_demo.cpp)
    target_link_libraries(mix_demo mix lp3::gfx lp3::input lp3::sims lp3::imgui lp3::main)

    if (LP3_SETUP_FOLDERS)
        set_target_properties (mix PROPERTIES FOLDER mix)
        set_target_properties (mix_demo PROPERTIES FOLDER mix)
    endif()
endif()
