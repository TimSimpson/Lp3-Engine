# *********************************************************************
# LP3_INPUT
#       Handles gamepads, joysticks, etc
# *********************************************************************
project(lp3_input CXX)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
include(CMakePackageConfigHelpers)

find_package(Lp3_Core 1.0.1)

add_library(input
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

target_include_directories(input
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src)

target_link_libraries(input PUBLIC lp3::core)

# *********************************************************************
# Package / Install Stuff
# *********************************************************************

install(DIRECTORY include/ DESTINATION include)

install(TARGETS input EXPORT Lp3_Input-targets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)

add_library(lp3::input ALIAS input)
install(EXPORT Lp3_Input-targets
    FILE Lp3_Input-targets.cmake
    NAMESPACE lp3::
    DESTINATION lib/cmake/Lp3_Input
)

file(WRITE "${PROJECT_BINARY_DIR}/Lp3_Input-config.cmake" "
include(CMakeFindDependencyMacro)
find_package(Lp3_Core 1.0.1)
include(\"\${CMAKE_CURRENT_LIST_DIR}/Lp3_Input-targets.cmake\")
")


write_basic_package_version_file("${PROJECT_BINARY_DIR}/Lp3_Input-config-version.cmake"
    VERSION 1.0.1
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${PROJECT_BINARY_DIR}/Lp3_Input-config.cmake"
    "${PROJECT_BINARY_DIR}/Lp3_Input-config-version.cmake"
    DESTINATION lib/cmake/Lp3_Input
)

# *********************************************************************
# Tests and Drivers / Demos
# *********************************************************************

get_directory_property(is_child PARENT_DIRECTORY)
if (NOT is_child)
    set(LP3_INPUT_FULL True)
endif()

if (LP3_INPUT_FULL)
    find_package(Lp3_Sims 1.0.1)
    find_package(Lp3_ImGui 1.0.1)
    find_package(Lp3_Gfx 1.0.1)

    add_executable(view_controls ${CMAKE_CURRENT_SOURCE_DIR}/demos/view_controls.cpp)
    target_link_libraries(view_controls input lp3::gfx lp3::sims lp3::imgui)

    add_executable(pokey ${CMAKE_CURRENT_SOURCE_DIR}/demos/pokey.cpp)
    target_link_libraries(pokey input lp3::gfx lp3::sims lp3::imgui)

    if (LP3_SETUP_FOLDERS)
        set_target_properties (input PROPERTIES FOLDER input)
        set_target_properties (view_controls PROPERTIES FOLDER input)
        set_target_properties (pokey PROPERTIES FOLDER input)
    endif()
endif()
