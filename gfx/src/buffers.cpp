#define LP3_GFX_API_CREATE
#include <lp3/gfx/buffers.hpp>
#include <lp3/gl.hpp>
#include <lp3/core.hpp>

namespace gl = lp3::gl;

namespace lp3 { namespace gfx {

LP3_GFX_API
FloatBuffer::FloatBuffer(const std::size_t size, bool _resize_ok)
:   vertices(),
    resize_ok(_resize_ok),
    vertex_pos_object()
{
    // Create the buffer to be used by OpenGL.(GLSL) (Bunny Link)
    vertex_pos_object = gl::gen_buffer();
    // Create the buffer to be used on our side (C++) (Normal Link)
    vertices.reserve(size);
}

LP3_GFX_API
GLfloat * FloatBuffer::claim(const int size) {
    if (!resize_ok && vertices.capacity() < (vertices.size() + size)) {
        LP3_THROW2(lp3::core::Exception, "No more size left in buffer to claim.");
    }
    const auto starting_index = vertices.size();
    vertices.resize(starting_index + size);
    return vertices.data() + starting_index;
}

LP3_GFX_API
void FloatBuffer::reset() {
	const auto s = this->size();
	vertices.clear();
	vertices.reserve(s);
}

LP3_GFX_API
void FloatBuffer::transfar() const {
    // Copy data from client side to graphics land.
    // If the vertices change at all this must happen again.
    gl::bind_buffer(GL_ARRAY_BUFFER, vertex_pos_object);
    gl::buffer_data(
        GL_ARRAY_BUFFER, gsl::span<const GLfloat>(vertices), GL_STATIC_DRAW);
}


LP3_GFX_API
ElementBuffer::ElementBuffer()
:   elements(),
    vertex_index_object()
{
    // Create the buffer to be used by OpenGL.(GLSL) (Bunny Link)
    vertex_index_object = gl::gen_buffer();
}

void ElementBuffer::reset() {
	elements.clear();
}

void ElementBuffer::transfar() const {
    // Copy data from client side to graphics land.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_index_object);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(GLushort) * elements.size(),
                 elements.data(),
                 GL_STATIC_DRAW);
}

} }
