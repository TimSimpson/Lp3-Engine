#ifndef FILE_LP3_GFX_PROGRAMS_COLORED_TEXTURED_HPP
#define FILE_LP3_GFX_PROGRAMS_COLORED_TEXTURED_HPP

#include "../config.hpp"
#include <lp3/gl.hpp>
#include "../management.hpp"
#include "../buffers.hpp"

namespace lp3 { namespace gfx { namespace programs {

// TODO: Once I actually understand what I'm doing try to cut down on
//       duplication between this and other programs.

class
LP3_GFX_API
ColoredTexture {
public:

    ColoredTexture();

    // Draws the screen
    void operator()(const glm::mat4 & previous) const;

    // Index of the position vertex attribute.
    const gl::VertexAttributeIndex a_position{ 0 };

    // Index of texture coordinates.
    const gl::VertexAttributeIndex a_texCoord{ 1 };

    // Index of color value.
    const gl::VertexAttributeIndex a_color{ 2 };

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

    void draw(const ElementWriter<TexCVert> & elements) const;
private:
    gl::ShaderOwner vertex_shader;
    gl::ShaderOwner fragment_shader;
    gl::ProgramOwner program_object;
};

} } }

#endif
