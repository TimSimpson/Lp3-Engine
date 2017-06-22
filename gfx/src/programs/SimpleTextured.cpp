#include <lp3/gfx/programs/SimpleTextured.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <lp3/gl.hpp>

namespace lp3 { namespace gfx { namespace programs {

namespace {

    constexpr char const u_mvp[] = "u_mvp";

    constexpr char const s_texture[] = "s_texture";

    const GLchar * vertex_shader_source = R"GLSL_CODE(
#version 100

uniform mat4 u_mvp;
attribute vec4 a_position;
attribute vec2 a_texCoord;
varying vec2 v_texCoord;

void main()
{
    gl_Position = u_mvp * a_position;
    v_texCoord = a_texCoord;
}
)GLSL_CODE";

	const GLchar * fragment_shader_source = R"GLSL_CODE(
#version 100

precision mediump float;
varying vec2 v_texCoord;
uniform sampler2D s_texture;

void main()
{
	vec4 color = texture2D(s_texture, v_texCoord);

	// Don't render if the alpha channel is zero
	if (color.a == 0.0) {
		discard;
	}
    gl_FragColor = color;
}

)GLSL_CODE";


}

LP3_GFX_API
SimpleTextured::SimpleTextured()
:   vertex_shader(gl::compile_shader(GL_VERTEX_SHADER, vertex_shader_source)),
    fragment_shader(gl::compile_shader(GL_FRAGMENT_SHADER,
                                       fragment_shader_source)),
    program_object(gl::create_program())
{
    gl::attach_shader(program_object, vertex_shader);
    gl::attach_shader(program_object, fragment_shader);

    gl::bind_attrib_location(program_object, a_position, "a_position");
    gl::bind_attrib_location(program_object, a_texCoord, "a_texCoord");

    gl::link_program(program_object);
}

LP3_GFX_API
gl::UniformVariableLocation SimpleTextured::get_u_mvp() const {
    return gl::get_uniform_location(program_object, u_mvp);
}

LP3_GFX_API
gl::UniformVariableLocation SimpleTextured::get_s_texture() const {
	return gl::get_uniform_location(program_object, s_texture);
}

LP3_GFX_API
void SimpleTextured::use() const {
	gl::use_program(program_object);
}

LP3_GFX_API
void SimpleTextured::set_array_buffer(gl::BufferObjectName vertex_pos_object) const {
    // Setup the position attribute to accept a pointer to the buffer.
    gl::bind_buffer(GL_ARRAY_BUFFER, vertex_pos_object);
    gl::vertex_attrib_pointer(
        this->a_position,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(GLfloat),
        0
    );
    // sETS THE TEXTURE STUFF TO THE POS STUFF FOR FUN
    // gl::vertex_attrib_pointer(
    //     this->a_texCoord,
    //     3,
    //     GL_FLOAT,
    //     GL_FALSE,
    //     5 * sizeof(GLfloat),
    //     0
    // );
    gl::vertex_attrib_pointer(
        this->a_texCoord,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(GLfloat),
        (const void *) (3 * sizeof(GLfloat))
    );
    gl::enable_vertex_attrib_array(this->a_position);
    gl::enable_vertex_attrib_array(this->a_texCoord);
}

LP3_GFX_API
void SimpleTextured::set_mvp(const glm::mat4 & mvp) const {
    const gl::UniformVariableLocation u_ortho_index = get_u_mvp();
    gl::uniform_matrix_4fv(u_ortho_index, const_cast<glm::mat4 &>(mvp));
}

LP3_GFX_API
void SimpleTextured::set_texture(const gl::TextureID & id) const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glUniform1i(get_s_texture(), 0);
}

LP3_GFX_API
void SimpleTextured::unset_array_buffer() const {
    gl::disable_vertex_attrib_array(this->a_position);
    gl::disable_vertex_attrib_array(this->a_texCoord);
}

LP3_GFX_API
void SimpleTextured::draw(const ElementWriter<TexVert> & elements) const {
    elements.transfar();
    // Setup the position attribute to accept a pointer to the buffer.
    this->set_array_buffer(elements.vertices_attribute());
    glDrawElements(GL_TRIANGLES, elements.count(), GL_UNSIGNED_SHORT, 0);
    this->unset_array_buffer();
}

}   }   }
