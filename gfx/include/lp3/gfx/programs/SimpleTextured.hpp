#ifndef FILE_LP3_GFX_PROGRAMS_SIMPLE_TEXTURED_HPP
#define FILE_LP3_GFX_PROGRAMS_SIMPLE_TEXTURED_HPP

#include "../config.hpp"
#include <lp3/gl.hpp>
#include "../management.hpp"
#include "../buffers.hpp"

namespace lp3 { namespace gfx { namespace programs {


class
LP3_GFX_API
SimpleTextured {
public:

    SimpleTextured();

    // Draws the screen
    void operator()(const glm::mat4 & previous) const;

    // Index of the position vertex attribute.
    const gl::VertexAttributeIndex a_position{ 0 };

    // Index of texture coordinates.
    const gl::VertexAttributeIndex a_texCoord{ 1 };

    // Name of mvp variable

    // Get variable of u mvp (MUST BE USING PROGRAM).
    gl::UniformVariableLocation get_u_mvp() const;


	// Get variable of s_texture (MUST BE USING PROGRAM).
	gl::UniformVariableLocation get_s_texture() const;

    // Calls glUseProgram
    void use() const;

    // Sets the given buffer to be used for the position and texture
    // cordinates in the program.
    void set_array_buffer(gl::BufferObjectName vertex_pos_object) const;

    // Sets the MVP matrix used by this program.
    void set_mvp(const glm::mat4 & previous) const;

    void set_texture(const gl::TextureID & id) const;

    // Disables array buffer variables.
    void unset_array_buffer() const;

    void draw(const ElementWriter<TexVert> & elements) const;
private:
    gl::ShaderOwner vertex_shader;
    gl::ShaderOwner fragment_shader;
    gl::ProgramOwner program_object;
};

} } }

#endif
