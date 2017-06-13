#define LP3_GFX_API_CREATE
#include <lp3/gfx/CPaper.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <lp3/gl.hpp>

namespace lp3 { namespace gfx {

namespace {

    const GLchar * vertex_shader_source = R"GLSL_CODE(
#version 100

uniform mat4 u_ortho;
attribute vec4 a_position;
attribute vec4 a_color;
varying vec4 v_color;

void main()
{
    v_color = a_color;
    gl_Position = u_ortho * a_position;
}
)GLSL_CODE";

    const GLchar * fragment_shader_source = R"GLSL_CODE(
#version 100

precision mediump float;

varying vec4 v_color;
void main()
{
    gl_FragColor = v_color;
}

)GLSL_CODE";


}

LP3_GFX_API
CPaper::CPaper(const glm::ivec2 _resolution)
:   resolution(_resolution),
    vertex_shader(gl::compile_shader(GL_VERTEX_SHADER, vertex_shader_source)),
    fragment_shader(gl::compile_shader(GL_FRAGMENT_SHADER,
                                       fragment_shader_source)),
    program_object(gl::create_program()),
    list()
{
    gl::attach_shader(program_object, vertex_shader);
    gl::attach_shader(program_object, fragment_shader);

    gl::bind_attrib_location(program_object, a_color, "a_color");
    gl::bind_attrib_location(program_object, a_position, "a_position");

    gl::link_program(program_object);
}

LP3_GFX_API
void CPaper::operator()(const glm::mat4 & previous) const {
    // TODO: should I make positive Z near instead? Near is currently -1, not 1.
    // The smart correct way to do this would appear to be the following
    // commented out line, but that doesn't work.
    //glm::mat4 ortho = glm::ortho(0, resolution.x, resolution.y, 0, -1, 1);

    // Reminder: OpenGL's normal coordinate system is like this:

    // (-1.0,  1.0)  |   (1.0,  1.0)
    //               |
    // --------------|--------------
    //               |
    // (-1.0, -1.0)  |   (1.0, -1.0)

    // This turns it into a "CPaper" coordinate system which I've been used to
    // since time immemorial (width and height are the resolution):

    // (0.0, 0.0)      |   (width,  0.0)
    //                 |
    // ----------------|----------------
    //                 |
    // ( 0.0, height)  | (width, height)

    // The matrix has to map the screen cordinates we want *back* to OpenGL's
    // system. Step one is to scale the width and height of what we're doing to
    // essentially shrink it, and reverse the Y. Next we move the center to
    // the upper left corner.

    const glm::mat4 scale = glm::scale(
        previous, glm::vec3(2.0f/resolution.x, -2.0f/resolution.y, 1.0f));
    const glm::mat4 translate = glm::translate(
        scale, glm::vec3(-0.5f * resolution.x, -0.5f * resolution.y, 0.0f));
    glm::mat4 ortho = translate;

    // {
    //  auto orig = glm::vec4(0.0, 1.0, 0.0, 1.0);
    //  auto thing = ortho * orig;
    //  LP3_LOG_INFO("original x=(%d -> %d), y=(%d -> %d), z=(%d -> %d)",
    //               orig.x, thing.x, orig.y, thing.y, orig.z, thing.z)
    // }
    // {
    //  auto orig = glm::vec4(10.0, 10.0, 10.0, 1.0);
    //  auto thing = ortho * orig;
    //  LP3_LOG_INFO("original x=(%d -> %d), y=(%d -> %d), z=(%d -> %d)",
    //      orig.x, thing.x, orig.y, thing.y, orig.z, thing.z)
    // }
    // {
    //  auto orig = glm::vec4(1920.0, 1080.0, 0.0, 1.0);
    //  auto thing = ortho * orig;
    //  LP3_LOG_INFO("original x=(%d -> %d), y=(%d -> %d), z=(%d -> %d)",
    //      orig.x, thing.x, orig.y, thing.y, orig.z, thing.z)
    // }

    LP3_GL_ASSERT_NO_ERRORS();

    gl::use_program(program_object);

    const gl::UniformVariableLocation u_ortho_index
        = gl::get_uniform_location(program_object, "u_ortho");
    gl::uniform_matrix_4fv(u_ortho_index, ortho);

	list.render(*this);
}

LP3_GFX_API
void CPaper::set_render_list(const CPaper::RendererList & new_list) {
	this->list = new_list;
}


LP3_GFX_API
CPaper::RendererList::RendererList()
:	colors(),
	vertices(),
	vertex_pos_object()
{
	// Create the buffer to be used by OpenGL.
	vertex_pos_object = gl::gen_buffer();

}

LP3_GFX_API
void CPaper::RendererList::render(const CPaper & cpaper) const {
	// Copy data from client side to graphics land.
	// If the vertices change at all this must happen again.
	gl::bind_buffer(GL_ARRAY_BUFFER, vertex_pos_object);
	gl::buffer_data(
		GL_ARRAY_BUFFER, gsl::span<const GLfloat>(vertices), GL_STATIC_DRAW);

	// Setup the position attribute to accept a pointer to the buffer.
	gl::bind_buffer(GL_ARRAY_BUFFER, vertex_pos_object);
	gl::vertex_attrib_pointer(cpaper.a_position, 3, GL_FLOAT, 0, 0, 0);
	gl::enable_vertex_attrib_array(cpaper.a_position);

	for (std::size_t i = 0; i < colors.size(); ++i) {
		GLfloat color[4];
		color[0] = colors[i].r;
		color[1] = colors[i].g;
		color[2] = colors[i].b;
		color[3] = colors[i].a;
		gl::vertex_attrib_4fv(cpaper.a_color, color);
		gl::draw_arrays(GL_TRIANGLES, lp3::narrow<GLint>(i * 3), 3);
	}
}

LP3_GFX_API
void CPaper::RendererList::triangle(glm::vec4 color, glm::vec2 a, glm::vec2 b,
	                                GLfloat z) {
	colors.push_back(color);
	glm::vec2 size = b - a;
	glm::vec3 v1(a.x + (size.x * 0.5f), a.y, z);
	glm::vec3 v2(a.x, b.y, z);
	glm::vec3 v3(b.x, b.y, z);

	vertices.push_back(v1.x);
	vertices.push_back(v1.y);
	vertices.push_back(v1.z);

	vertices.push_back(v2.x);
	vertices.push_back(v2.y);
	vertices.push_back(v2.z);

	vertices.push_back(v3.x);
	vertices.push_back(v3.y);
	vertices.push_back(v3.z);
}



}   }
