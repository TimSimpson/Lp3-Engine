#ifndef FILE_LP3_GFX_BUFFERS_HPP
#define FILE_LP3_GFX_BUFFERS_HPP

#include "config.hpp"
#include <lp3/gl.hpp>
#include "elements.hpp"

namespace lp3 { namespace gfx {

// No init allocator - allows vector::resize and other functions to avoid
// default initialization of new items.
// I nabbed this from an answer Casey gave on stackoverflow.com which was
// linked to by cppreference.com.
template<typename T, typename A=std::allocator<T>>
class default_init_allocator :   public std::allocator<T> {
    typedef std::allocator_traits<A> a_t;
public:
    template<typename U> struct rebind {
        using other = default_init_allocator<U, typename a_t::template rebind_alloc<U>>;
    };
    using A::A;
    template<typename U>
    void construct(U * ptr) noexcept(std::is_nothrow_default_constructible<U>::value) {
        ::new(static_cast<void *>(ptr)) U;
    }

    template<typename U, typename... Args>
    void construct(U * ptr, Args &&... args) {
        a_t::construct(static_cast<A&>(*this), ptr, std::forward<Args>(args)...);
    }
};


LP3_GFX_API
class FloatBuffer {
public:
    FloatBuffer(const std::size_t size, bool resize_ok=false);

    // Returns a pointer to the start of count number of contiguous floats
    // for the number of vertices requested * vertex_fmt_size
    GLfloat * claim(const int vertex_count);

    // Handle to buffer object- GLSL code.will use this for an attribute.
    gl::BufferObjectName attribute() const {
        return vertex_pos_object;
    }

	// Resets the buffer so it can be reused.
	void reset();

    std::size_t size() const {
        return vertices.size();
    }
    // Copy our data to the graphics card.
    // This is const because we don't actually modify data on our side...
    // though maybe thats not the right idea conceptually.
    void transfar() const;

private:
    std::vector<GLfloat, default_init_allocator<GLfloat>> vertices;
    bool resize_ok;
    gl::BufferObjectName vertex_pos_object;
};


LP3_GFX_API
class ElementBuffer {
public:
    ElementBuffer();

    inline void push_back(GLushort value) {
        elements.push_back(value);
    }

	// Clears all elements
	void reset(); 

    inline GLsizei size() const {
        return GLsizei(elements.size());
    }

    // Copy our data to the graphics card.
    void transfar() const;

private:
    std::vector<GLushort> elements;
    gl::BufferObjectName vertex_index_object;  // todo: rename
};


LP3_GFX_API
template<typename Vertex>
class ElementWriter {
public:
    ElementWriter(std::size_t count)
    :   vertices(Vertex::array_size * count)
    {}

    Triangle<Vertex> add_triangle() {
        const auto starting_index = vertices.size();
        const GLushort element_starting_index
            = static_cast<GLushort>(starting_index / Vertex::array_size);

        GLfloat * data = vertices.claim(3 * TexVert::array_size);

        Triangle<Vertex> triangle{data};

        elements.push_back(element_starting_index + 0);
        elements.push_back(element_starting_index + 1);
        elements.push_back(element_starting_index + 2);

        return triangle;
    }

    Quad<Vertex> add_quad() {
        const auto starting_index = vertices.size();
        const GLushort element_starting_index
            = static_cast<GLushort>(starting_index / Vertex::array_size);

        GLfloat * data = vertices.claim(4 * TexVert::array_size);

        Quad<Vertex> quad{data};

        elements.push_back(element_starting_index + 0);
        elements.push_back(element_starting_index + 1);
        elements.push_back(element_starting_index + 2);

        elements.push_back(element_starting_index + 0);
        elements.push_back(element_starting_index + 2);
        elements.push_back(element_starting_index + 3);

        return quad;
    }

	QuadArray<Vertex> add_quads(const int count) {
		const auto starting_index = vertices.size();
		const GLushort element_starting_index
			= static_cast<GLushort>(starting_index / Vertex::array_size);

		GLfloat * data = vertices.claim(4 * TexVert::array_size * count);
		for (int i = 0; i < count; ++i) {
			const GLushort start_at = element_starting_index + (i * 4);
			elements.push_back(start_at + 0);
			elements.push_back(start_at + 1);
			elements.push_back(start_at + 2);

			elements.push_back(start_at + 0);
			elements.push_back(start_at + 2);
			elements.push_back(start_at + 3);
		}

		QuadArray<Vertex> array(data, count);
		return array;
	}

	// Reset the buffers in order to write again.
	void reset() {
		vertices.reset();
		elements.reset();
	}

    void transfar() const {
        vertices.transfar();
        elements.transfar();
    }

    gl::BufferObjectName vertices_attribute() const {
        return vertices.attribute();
    }

    inline GLsizei count() const {
        return elements.size();
    }
private:
    FloatBuffer vertices;
    ElementBuffer elements;
};
} }

#endif
