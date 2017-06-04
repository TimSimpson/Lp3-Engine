# *********************************************************************
# LP3_GFX (II)
#       Displays stuff
# *********************************************************************
project(lp3_gfx CXX)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

add_library(Lp3_Gfx
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

    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gfx/nodes/TPaper.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gfx/programs/SimpleTextured.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/buffers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CPaper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/management.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/matrices.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/screen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/text.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/texture.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/nodes/TPaper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/programs/SimpleTextured.cpp
)


target_include_directories(Lp3_Gfx
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src)

target_link_libraries(Lp3_Gfx Lp3_Core Lp3_Gl)

install(TARGETS Lp3_Gfx EXPORT Lp3_Gfx_Config
    ARCHIVE  DESTINATION lib
    LIBRARY  DESTINATION lib
    RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION include)

install(EXPORT Lp3_Gfx_Config DESTINATION share/Lp3_Gfx/cmake)

export(TARGETS Lp3_Gfx FILE Lp3_Gfx_Config.cmake)

if (LP3_GFX_FULL)
    add_executable(Lp3_Gfx_MultipleWindows ${CMAKE_CURRENT_SOURCE_DIR}/demos/MultipleWindows.cpp)
    target_link_libraries(Lp3_Gfx_MultipleWindows Lp3_Gfx)

    add_executable(SimpleGfx ${CMAKE_CURRENT_SOURCE_DIR}/demos/SimpleGfx.cpp)
    target_link_libraries(SimpleGfx Lp3_Gfx)

    add_executable(CPaperDemo ${CMAKE_CURRENT_SOURCE_DIR}/demos/CPaperDemo.cpp)
    target_link_libraries(CPaperDemo Lp3_Gfx)

    add_executable(TextureDemo ${CMAKE_CURRENT_SOURCE_DIR}/demos/TextureDemo.cpp)
    target_link_libraries(TextureDemo Lp3_Gfx Lp3_Sims)

    # This uses sdl2_ttf, which doesn't work well on Emscripten.
    add_executable(font_saver ${CMAKE_CURRENT_SOURCE_DIR}/demos/font_saver.cpp)
    target_link_libraries(font_saver Lp3_Gfx Lp3_Sims lp3_deps::sdl2_ttf)

    add_executable(show_font ${CMAKE_CURRENT_SOURCE_DIR}/demos/show_font.cpp)
    target_link_libraries(show_font Lp3_Gfx Lp3_Sims)

    if (LP3_SETUP_FOLDERS)
        set_target_properties (Lp3_Gfx PROPERTIES FOLDER gfx)
        set_target_properties (CPaperDemo PROPERTIES FOLDER gfx)
        set_target_properties (font_saver PROPERTIES FOLDER gfx)
        set_target_properties (SimpleGfx PROPERTIES FOLDER gfx)
        set_target_properties (TextureDemo PROPERTIES FOLDER gfx)
        set_target_properties (Lp3_Gfx_MultipleWindows PROPERTIES FOLDER gfx)
    endif()
endif()
