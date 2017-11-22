# *********************************************************************
# LP3_GFX (II)
#       Displays stuff
# *********************************************************************
project(lp3_gfx CXX)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
include(CMakePackageConfigHelpers)

find_package(Lp3_Gl 1.0.1)

add_library(gfx
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gfx.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gfx/buffers.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gfx/config.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gfx/colors.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gfx/CPaper.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gfx/elements.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gfx/management.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gfx/matrices.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gfx/screen.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gfx/text.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gfx/tiles.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gfx/nodes/TPaper.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gfx/programs/ColoredTexture.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gfx/programs/SimpleTextured.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/buffers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CPaper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/management.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/matrices.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/screen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/text.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/texture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tiles.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/nodes/TPaper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/programs/ColoredTexture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/programs/SimpleTextured.cpp
)

target_include_directories(gfx
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src)
target_link_libraries(gfx PUBLIC lp3::gl)
set_target_properties(gfx PROPERTIES OUTPUT_NAME "Lp3_Gfx")


# Handle installation, find_package stuff

install(DIRECTORY include/ DESTINATION include)

install(TARGETS gfx EXPORT Lp3_Gfx-targets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)

add_library(lp3::gfx ALIAS gfx)
install(EXPORT Lp3_Gfx-targets
    FILE Lp3_Gfx-targets.cmake
    NAMESPACE lp3::
    DESTINATION lib/cmake/Lp3_Gfx
)

file(WRITE "${PROJECT_BINARY_DIR}/Lp3_Gfx-config.cmake" "
include(CMakeFindDependencyMacro)
find_dependency(Lp3_Gl)
include(\"\${CMAKE_CURRENT_LIST_DIR}/Lp3_Gfx-targets.cmake\")
")


write_basic_package_version_file("${PROJECT_BINARY_DIR}/Lp3_Gfx-config-version.cmake"
    VERSION 1.0.1
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${PROJECT_BINARY_DIR}/Lp3_Gfx-config.cmake"
    "${PROJECT_BINARY_DIR}/Lp3_Gfx-config-version.cmake"
    DESTINATION lib/cmake/Lp3_Gfx
)

# *********************************************************************
# Tests and Drivers / Demos
# *********************************************************************

get_directory_property(is_child PARENT_DIRECTORY)
if (NOT is_child)
    set(LP3_GFX_FULL True)
endif()

if (LP3_GFX_FULL)
    add_executable(Lp3_Gfx_MultipleWindows ${CMAKE_CURRENT_SOURCE_DIR}/demos/MultipleWindows.cpp)
    target_link_libraries(Lp3_Gfx_MultipleWindows gfx)

    add_executable(SimpleGfx ${CMAKE_CURRENT_SOURCE_DIR}/demos/SimpleGfx.cpp)
    target_link_libraries(SimpleGfx gfx)

    add_executable(CPaperDemo ${CMAKE_CURRENT_SOURCE_DIR}/demos/CPaperDemo.cpp)
    target_link_libraries(CPaperDemo gfx)

    add_executable(TextureDemo ${CMAKE_CURRENT_SOURCE_DIR}/demos/TextureDemo.cpp)
    target_link_libraries(TextureDemo gfx lp3::sims)

    add_executable(DrawTiles ${CMAKE_CURRENT_SOURCE_DIR}/demos/DrawTiles.cpp)
    target_link_libraries(DrawTiles gfx lp3::sims)

    # This uses sdl2_ttf, which doesn't work well on Emscripten.
    add_executable(font_saver ${CMAKE_CURRENT_SOURCE_DIR}/demos/font_saver.cpp)
    target_link_libraries(font_saver gfx lp3::sims lp3_deps::sdl2_ttf)

    add_executable(show_font ${CMAKE_CURRENT_SOURCE_DIR}/demos/show_font.cpp)
    target_link_libraries(show_font gfx lp3::sims)

    if (LP3_SETUP_FOLDERS)
        set_target_properties (gfx PROPERTIES FOLDER gfx)
        set_target_properties (CPaperDemo PROPERTIES FOLDER gfx)
        set_target_properties (DrawTiles PROPERTIES FOLDER gfx)
        set_target_properties (font_saver PROPERTIES FOLDER gfx)
        set_target_properties (SimpleGfx PROPERTIES FOLDER gfx)
        set_target_properties (TextureDemo PROPERTIES FOLDER gfx)
        set_target_properties (Lp3_Gfx_MultipleWindows PROPERTIES FOLDER gfx)
    endif()
endif()
