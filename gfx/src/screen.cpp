#include <lp3/gfx/screen.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <lp3/gl.hpp>

namespace lp3 { namespace gfx {

namespace {

	const GLchar * vertex_shader_source = R"GLSL_CODE(
uniform mat4 u_ortho;
attribute vec4 a_position;
attribute vec4 a_color;
varying vec4 v_color;

void main()
{
    v_color = a_color;
    gl_Position = u_ortho * a_position;
	//gl_Position = a_position;
}
)GLSL_CODE";

	const GLchar * fragment_shader_source = R"GLSL_CODE(
#version 100

#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_color;
void main()
{
    gl_FragColor = v_color;
}

)GLSL_CODE";


}

LP3_GFX_API
Screen::Screen(const glm::ivec2 _resolution)
:	resolution(_resolution),
	vertex_shader(gl::compile_shader(GL_VERTEX_SHADER, vertex_shader_source)),
	fragment_shader(gl::compile_shader(GL_FRAGMENT_SHADER,
		                               fragment_shader_source)),
	program_object(gl::create_program()),
	object()
{
	gl::attach_shader(program_object, vertex_shader);
	gl::attach_shader(program_object, fragment_shader);

	gl::bind_attrib_location(program_object, a_color, "a_color");
	gl::bind_attrib_location(program_object, a_position, "a_position");

	gl::link_program(program_object);
}

LP3_GFX_API
void Screen::operator()(const glm::mat4 & previous) const {
	// TODO: should I make positive Z near instead? Near is currently -1, not 1.
	// The smart correct way to do this would appear to be the following
	// commented out line, but that doesn't work.
	//glm::mat4 ortho = glm::ortho(0, resolution.x, resolution.y, 0, -1, 1);

	// Reminder: OpenGL's normal cordinate system is like this:

	// (-1.0,  1.0)  |   (1.0,  1.0)
	//               |
	// --------------|--------------
	//               |
	// (-1.0, -1.0)  |   (1.0, -1.0)

	// This turns it into a "Screen" cordinate system which I've been used to
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
	// 	auto orig = glm::vec4(0.0, 1.0, 0.0, 1.0);
	// 	auto thing = ortho * orig;
	// 	LP3_LOG_INFO("original x=(%d -> %d), y=(%d -> %d), z=(%d -> %d)",
	// 		         orig.x, thing.x, orig.y, thing.y, orig.z, thing.z)
	// }
	// {
	// 	auto orig = glm::vec4(10.0, 10.0, 10.0, 1.0);
	// 	auto thing = ortho * orig;
	// 	LP3_LOG_INFO("original x=(%d -> %d), y=(%d -> %d), z=(%d -> %d)",
	// 		orig.x, thing.x, orig.y, thing.y, orig.z, thing.z)
	// }
	// {
	// 	auto orig = glm::vec4(1920.0, 1080.0, 0.0, 1.0);
	// 	auto thing = ortho * orig;
	// 	LP3_LOG_INFO("original x=(%d -> %d), y=(%d -> %d), z=(%d -> %d)",
	// 		orig.x, thing.x, orig.y, thing.y, orig.z, thing.z)
	// }

	LP3_GL_ASSERT_NO_ERRORS();

	gl::use_program(program_object);

	const gl::UniformVariableLocation u_ortho_index
		= gl::get_uniform_location(program_object, "u_ortho");
	gl::uniform_matrix_4fv(u_ortho_index, ortho);

	if (object) {
		object(*this);
	}
	//gl::bind_buffer(GL_ARRAY_BUFFER, a)
}

LP3_GFX_API
void Screen::set_object(ScreenObject obj) {
	this->object = obj;
}

}	}
