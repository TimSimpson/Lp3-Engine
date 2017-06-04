# *********************************************************************
# LP3_IMGUI
#       Simple library which builds ImGUI
# *********************************************************************
project(lp3_imgui CXX)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

add_library(Lp3_ImGui
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/imgui.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImGuiApp.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui_internal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imconfig.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/stb_rect_pack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/stb_textedit.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/stb_truetype.h
)


target_include_directories(Lp3_ImGui
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src)

target_link_libraries(Lp3_ImGui Lp3_Core Lp3_Gfx)

install(TARGETS Lp3_ImGui EXPORT Lp3_ImGui_Config
    ARCHIVE  DESTINATION lib
    LIBRARY  DESTINATION lib
    RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION include)

install(EXPORT Lp3_ImGui_Config DESTINATION share/Lp3_ImGui/cmake)

export(TARGETS Lp3_ImGui FILE Lp3_ImGui_Config.cmake)

if (LP3_IMGUI_FULL)
    add_executable(ImGuiDemo ${CMAKE_CURRENT_SOURCE_DIR}/src/demo.cpp)
    target_link_libraries(ImGuiDemo Lp3_ImGui ${LP3_LIBS})

    if (LP3_SETUP_FOLDERS)
        set_target_properties (Lp3_ImGui PROPERTIES FOLDER imgui)
        set_target_properties (ImGuiDemo PROPERTIES FOLDER imgui)
    endif()
endif()
