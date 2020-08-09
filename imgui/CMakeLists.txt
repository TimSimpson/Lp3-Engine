# *********************************************************************
# LP3_IMGUI
#       Simple library which builds ImGUI
# *********************************************************************
project(lp3_imgui CXX)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
include(CMakePackageConfigHelpers)

find_package(lp3-main REQUIRED)
find_package(Lp3_Gfx 1.0.1)

add_library(imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/imgui.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImGuiApp.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imconfig.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_draw_wrapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui_internal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imstb_rectpack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imstb_textedit.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imstb_truetype.h
)
set_target_properties(imgui PROPERTIES OUTPUT_NAME "Lp3_ImGui")

target_include_directories(imgui
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src)

target_link_libraries(imgui PUBLIC lp3::gfx)

# *********************************************************************
# Package / Install Stuff
# *********************************************************************

install(DIRECTORY include/ DESTINATION include)
install(FILES lib/imgui.h lib/imconfig.h DESTINATION include)

install(TARGETS imgui EXPORT Lp3_ImGui-targets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)

add_library(lp3::imgui ALIAS imgui)
install(EXPORT Lp3_ImGui-targets
    FILE Lp3_ImGui-targets.cmake
    NAMESPACE lp3::
    DESTINATION lib/cmake/Lp3_ImGui
)

file(WRITE "${PROJECT_BINARY_DIR}/Lp3_ImGui-config.cmake" "
include(CMakeFindDependencyMacro)
find_package(Lp3_Core 1.0.1)
include(\"\${CMAKE_CURRENT_LIST_DIR}/Lp3_ImGui-targets.cmake\")
")


write_basic_package_version_file("${PROJECT_BINARY_DIR}/Lp3_ImGui-config-version.cmake"
    VERSION 1.0.1
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${PROJECT_BINARY_DIR}/Lp3_ImGui-config.cmake"
    "${PROJECT_BINARY_DIR}/Lp3_ImGui-config-version.cmake"
    DESTINATION lib/cmake/Lp3_ImGui
)

# *********************************************************************
# Tests and Drivers / Demos
# *********************************************************************

get_directory_property(is_child PARENT_DIRECTORY)
if (NOT is_child)
    set(LP3_IMGUI_FULL True)
endif()

if (LP3_IMGUI_FULL)
    add_executable(ImGuiDemo WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/demo.cpp)
    target_link_libraries(ImGuiDemo imgui ${LP3_LIBS} lp3::main)

    if (LP3_SETUP_FOLDERS)
        set_target_properties (imgui PROPERTIES FOLDER imgui)
        set_target_properties (ImGuiDemo PROPERTIES FOLDER imgui)
    endif()
endif()
