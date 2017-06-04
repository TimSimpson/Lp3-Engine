# *********************************************************************
# LP3_INPUT
#       Handles gamepads, joysticks, etc
# *********************************************************************
project(lp3_input CXX)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

add_library(Lp3_Input
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/input.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/input/config.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Device.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DeviceSource.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/input.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DeviceSource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SdlGameController.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SdlJoystick.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SdlKeyboard.cpp
)

target_include_directories(Lp3_Input
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src)

target_link_libraries(Lp3_Input Lp3_Core)

install(TARGETS Lp3_Input EXPORT Lp3_Input_Config
    ARCHIVE  DESTINATION lib
    LIBRARY  DESTINATION lib
    RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION include)

install(EXPORT Lp3_Input_Config DESTINATION share/Lp3_Input/cmake)

export(TARGETS Lp3_Input FILE Lp3_Input_Config.cmake)

if (LP3_INPUT_FULL)
    add_executable(view_controls ${CMAKE_CURRENT_SOURCE_DIR}/demos/view_controls.cpp)
    target_link_libraries(view_controls Lp3_Input Lp3_Gfx Lp3_Sims Lp3_ImGui)

    if (LP3_SETUP_FOLDERS)
        set_target_properties (Lp3_Input PROPERTIES FOLDER input)
        set_target_properties (view_controls PROPERTIES FOLDER input)
    endif()
endif()
