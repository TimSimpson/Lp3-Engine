# *********************************************************************
# LP3_MIX
#       Convenience stuff for SDL Mixer
# *********************************************************************
project(lp3_mix CXX)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

add_library(Lp3_Mix
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/mix.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/mix/config.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/mix.cpp
)

target_include_directories(Lp3_Mix
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src)

target_link_libraries(Lp3_Mix Lp3_Core lp3_deps::sdlx_mixer)

install(TARGETS Lp3_Mix EXPORT Lp3_Mix_Config
    ARCHIVE  DESTINATION lib
    LIBRARY  DESTINATION lib
    RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION include)

install(EXPORT Lp3_Mix_Config DESTINATION share/Lp3_Mix/cmake)

export(TARGETS Lp3_Mix FILE Lp3_Mix_Config.cmake)

add_executable(mix_demo ${CMAKE_CURRENT_SOURCE_DIR}/demos/mix_demo.cpp)
target_link_libraries(mix_demo Lp3_Mix Lp3_Gfx Lp3_Input Lp3_Sims Lp3_ImGui)

if (LP3_SETUP_FOLDERS)
    set_target_properties (Lp3_Mix PROPERTIES FOLDER mix)
    set_target_properties (mix_demo PROPERTIES FOLDER mix)
endif()
