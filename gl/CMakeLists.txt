# *********************************************************************
# LP3_GL
#       OpenGL stuff
# *********************************************************************
project(lp3_gl CXX)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
include(CMakePackageConfigHelpers)

find_package(glm)
find_package(Lp3_Core 1.0.1)

add_library(gl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gl.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gl/config.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gl/extras.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/gl/gles2.h

    ${CMAKE_CURRENT_SOURCE_DIR}/src/extras.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gles2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gles2._orig_c
)
set_target_properties(gl PROPERTIES OUTPUT_NAME "Lp3_Gl")

target_include_directories(gl
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src)

target_link_libraries(gl PUBLIC lp3::core glm::glm)

# *********************************************************************
# Package / Install Stuff
# *********************************************************************

install(DIRECTORY include/ DESTINATION include)

install(TARGETS gl EXPORT Lp3_Gl-targets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)

add_library(lp3::gl ALIAS gl)
install(EXPORT Lp3_Gl-targets
    FILE Lp3_Gl-targets.cmake
    NAMESPACE lp3::
    DESTINATION lib/cmake/Lp3_Gl
)

file(WRITE "${PROJECT_BINARY_DIR}/Lp3_Gl-config.cmake" "
include(CMakeFindDependencyMacro)
find_dependency(glm)
find_dependency(Lp3_Core)
include(\"\${CMAKE_CURRENT_LIST_DIR}/Lp3_Gl-targets.cmake\")
")


write_basic_package_version_file("${PROJECT_BINARY_DIR}/Lp3_Gl-config-version.cmake"
    VERSION 1.0.1
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${PROJECT_BINARY_DIR}/Lp3_Gl-config.cmake"
    "${PROJECT_BINARY_DIR}/Lp3_Gl-config-version.cmake"
    DESTINATION lib/cmake/Lp3_Gl
)

# *********************************************************************
# Tests and Drivers / Demos
# *********************************************************************

get_directory_property(is_child PARENT_DIRECTORY)
if (NOT is_child)
    set(LP3_GL_FULL True)
endif()

if (LP3_GL_FULL)
    add_executable(glDemo ${CMAKE_CURRENT_SOURCE_DIR}/demos/glDemo.cpp)
    target_link_libraries(glDemo gl ${LP3_LIBS})

    if (LP3_SETUP_FOLDERS)
        set_target_properties (gl PROPERTIES FOLDER gl)
        set_target_properties (glDemo PROPERTIES FOLDER gl)
    endif()
endif()
