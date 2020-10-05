// --------------------------------------------------------------------
// This example shows how to use lp3::gl and lp3::sdl to create a
// simple app.
// --------------------------------------------------------------------
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

int _main(lp3::main::PlatformLoop & loop) {
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
	gl::initialize();

    // Create and compile the vertex shader
    const GLchar* vertex_shader_source = R"GLSL_CODE(
#version 100

attribute vec4 v_position;
void main()
{
   gl_Position = v_position;
}

    )GLSL_CODE";

    gl::ShaderOwner vertex_shader
        = gl::compile_shader(GL_VERTEX_SHADER, vertex_shader_source);

    // Create and compile the fragment shader
    const GLchar* fragment_source = R"GLSL_CODE(
#version 100

#ifdef GL_ES
precision mediump float;
#endif

void main()
{
  gl_FragColor = vec4 (1.0, 0.0, 0.0, 1.0 );
}

        )GLSL_CODE";
	gl::ShaderOwner fragment_shader
        = lp3::gl::compile_shader(GL_FRAGMENT_SHADER, fragment_source);

    // Link the vertex and fragment shader into a shader program
	gl::ProgramOwner program_object = gl::create_program();
    gl::attach_shader(program_object, vertex_shader);
    gl::attach_shader(program_object, fragment_shader);

	const gl::VertexAttributeIndex v_position{ 0 };

    gl::bind_attrib_location(program_object, v_position, "v_position");

	lp3::gl::link_program(program_object);

	gl::BufferObjectName vertex_pos_object = gl::gen_buffer();

    GLfloat vertices[] = { 0.0f,  0.5f, 0.0f,
                          -0.5f, -0.5f, 0.5f,
                           0.5f, -0.5f, 0.0f };
    gl::bind_buffer(GL_ARRAY_BUFFER, vertex_pos_object);
	gl::buffer_data(GL_ARRAY_BUFFER, gsl::span<const GLfloat>(vertices),
		            GL_STATIC_DRAW);

	loop.run([&window, &program_object, &vertex_pos_object, &v_position]() {
        SDL_Event e;
        bool quit = false;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        gl::use_program(program_object);

        gl::bind_buffer(GL_ARRAY_BUFFER, vertex_pos_object);
        gl::vertex_attrib_pointer(v_position, 3, GL_FLOAT, 0, 0, 0);
        gl::enable_vertex_attrib_array(v_position);
        gl::draw_arrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window);
        return !quit;
    });

    return 0;
}

LP3_MAIN(_main)

// ~end-doc
