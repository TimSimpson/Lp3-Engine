#include <fstream>
#include <iostream>
#include <memory>
#include <lp3/core.hpp>
#include <lp3/gl.hpp>
#include <lp3/sdl.hpp>
#include <lp3/log.hpp>
#include <lp3/main.hpp>

namespace core = lp3::core;
namespace gl = lp3::gl;
namespace sdl = lp3::sdl;

int _main(core::PlatformLoop & loop) {
    sdl::SDL2 sdl2(SDL_INIT_VIDEO);
    core::LogSystem log;
    core::MediaManager media;

    sdl::Window window = SDL_CreateWindow(
        "OpenGL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480,
        SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE
    );
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetSwapInterval(0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    sdl::GLContext gl_context = SDL_GL_CreateContext(window);
    if (SDL_GL_MakeCurrent(window, gl_context) < 0) {
        LP3_LOG_ERROR("Error creating GL %s", SDL_GetError());
    }
    LP3_ASSERT_TRUE(GLEW_OK == glewInit());


    // Create and compile the vertex shader
    const GLchar* vertex_shader_source =
        "attribute vec4 vPosition;    \n"
        "void main()                  \n"
        "{                            \n"
        "   gl_Position = vPosition;  \n"
        "}                            \n";
    gl::ShaderOwner vertex_shader
        = gl::compile_shader(GL_VERTEX_SHADER, vertex_shader_source);

    // Create and compile the fragment shader
    const GLchar* fragment_source =
        "precision mediump float;\n"
        "void main()                                 \n"
        "{                                           \n"
        "  gl_FragColor = vec4 (1.0, 0.0, 0.0, 1.0 );\n"
        "}                                           \n";
    gl::ShaderOwner fragment_shader
        = gl::compile_shader(GL_FRAGMENT_SHADER, fragment_source);

    // Link the vertex and fragment shader into a shader program
    gl::ProgramOwner program_object = glCreateProgram();
    glAttachShader(program_object, vertex_shader);
    glAttachShader(program_object, fragment_shader);

    glBindAttribLocation(program_object, 0, "vPosition");

    gl::link_program(program_object);

    GLuint vertex_pos_object;
    glGenBuffers(1, &vertex_pos_object);

    GLfloat vertices[] = { 0.0f,  0.5f, 0.0f,
                          -0.5f, -0.5f, 0.5f,
                           0.5f, -0.5f, 0.0f };
    glBindBuffer(GL_ARRAY_BUFFER, vertex_pos_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    loop.run([&window, &program_object, &vertex_pos_object]() {
        SDL_Event e;
        bool quit = false;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program_object);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_pos_object);
        glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window);
        return !quit;
    });

    return 0;
}

LP3_MAIN(_main, "Sdl2WithGlew")
