# *********************************************************************
# LP3_GL
#       OpenGL stuff
# *********************************************************************
project(lp3_gl CXX)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

find_package(Lp3_Core 1.0.1)
find_package(glm)

add_library(Lp3_Gl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gl.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gl/config.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gl/extras.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gl/gles2.h

    ${CMAKE_CURRENT_SOURCE_DIR}/src/extras.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gles2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gles2._orig_c
)


target_include_directories(Lp3_Gl
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src)

target_link_libraries(Lp3_Gl lp3::core glm)

install(TARGETS Lp3_Gl EXPORT Lp3_Gl_Config
    ARCHIVE  DESTINATION lib
    LIBRARY  DESTINATION lib
    RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION include)

install(EXPORT Lp3_Gl_Config DESTINATION share/Lp3_Gl/cmake)

export(TARGETS Lp3_Gl FILE Lp3_Gl_Config.cmake)

if (LP3_GL_FULL)
    add_executable(glDemo ${CMAKE_CURRENT_SOURCE_DIR}/demos/glDemo.cpp)
    target_link_libraries(glDemo Lp3_Gl ${LP3_LIBS})

    if (LP3_SETUP_FOLDERS)
        set_target_properties (Lp3_Gl PROPERTIES FOLDER gl)
        set_target_properties (glDemo PROPERTIES FOLDER gl)
    endif()
endif()
