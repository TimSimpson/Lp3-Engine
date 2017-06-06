#include <lp3/gfx/nodes/TPaper.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <lp3/gl.hpp>

namespace lp3 { namespace gfx {


LP3_GFX_API
TPaper::TPaper(const glm::ivec2 _resolution, gl::TextureID _texture,
               const std::size_t vertex_count)
:   program(),
	resolution(_resolution),
	texture(_texture),
    _elements(vertex_count * TexVert::array_size)
{
}

LP3_GFX_API
void TPaper::operator()(const glm::mat4 & previous) const {
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

    // This turns it into a "TPaper" coordinate system which I've been used to
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

	program.use();
    program.set_mvp(ortho);
    program.set_texture(this->texture);

	// glActiveTexture(GL_TEXTURE0);
	// glBindTexture(GL_TEXTURE_2D, this->texture);

	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// //glEnable(GL_ALPHA_TEST);
	// //glAlphaFunc(GL_GREATER, 0.0f);

	// glUniform1i(program.get_s_texture(), 0);

    program.draw(_elements);
	// //vertices.transfar();
	// _elements.transfar();
	// // Setup the position attribute to accept a pointer to the buffer.
	// program.set_array_buffer(_elements.vertices_attribute());

	// //elements.transfar();
	// // glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, vertex_index_object);
	// // glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * elements.size(), elements.data(), GL_STATIC_DRAW );

	// //gl::draw_arrays(GL_TRIANGLES, 0, 3); //vertices.size() / 5);
	// glDrawElements(GL_TRIANGLES, _elements.count(), GL_UNSIGNED_SHORT, 0);
}


}   }
