#ifndef FILE_LP3_GFX_ELEMENTS_HPP
#define FILE_LP3_GFX_ELEMENTS_HPP

#include "config.hpp"
#include <glm/glm.hpp>
#include <gsl/gsl>
#include <lp3/gl.hpp>

namespace lp3 { namespace gfx {


LP3_GFX_API
class TexVert {
public:
    static constexpr std::size_t array_size = 5;

    inline TexVert(GLfloat * _data)
    :   data(_data)
    {
        LP3_ASSERT(nullptr != data);
    }

    inline GLfloat & x() {
        return data[0];
    }

    inline GLfloat & y() {
        return data[1];
    }

    inline GLfloat & z() {
        return data[2];
    }

    inline GLfloat & u() {
        return data[3];
    }

    inline GLfloat & v() {
        return data[4];
    }

    inline void set(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v) {
        this->x() = x;
        this->y() = y;
        this->z() = z;
        this->u() = u;
        this->v() = v;
    }

    inline glm::vec2 pos_to_vec2() {
        return glm::vec2{x(), y()};
    }

    inline void pos_to_vec2(const glm::vec2 & v) {
        x() = v.x;
        y() = v.y;
    }

    inline glm::vec3 pos_to_vec3() {
        return glm::vec3{x(), y(), z()};
    }

    inline void pos_to_vec3(const glm::vec3 & v) {
        x() = v.x;
        y() = v.y;
        z() = v.z;
    }
protected:
    GLfloat * data;
};

LP3_GFX_API
class TexCVert : public TexVert {
public:
    static constexpr std::size_t array_size = 9;

    inline TexCVert(GLfloat * _data)
    :   TexVert(_data)
    {
    }

    inline GLfloat & r() {
        return data[5];
    }

    inline GLfloat & g() {
        return data[6];
    }

    inline GLfloat & b() {
        return data[7];
    }

    inline GLfloat & a() {
        return data[8];
    }

    inline void set(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v) {
        set(x, y, z, u, v, 1.0f, 1.0f, 1.0f, 1.0f);
    }

    inline void set(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v,
                    GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
        this->x() = x;
        this->y() = y;
        this->z() = z;
        this->u() = u;
        this->v() = v;
        this->r() = r;
        this->g() = g;
        this->b() = b;
        this->a() = a;
    }

    inline glm::vec2 pos_to_vec2() {
        return glm::vec2{x(), y()};
    }

    inline void pos_to_vec2(const glm::vec2 & v) {
        x() = v.x;
        y() = v.y;
    }

    inline glm::vec3 pos_to_vec3() {
        return glm::vec3{x(), y(), z()};
    }

    inline void pos_to_vec3(const glm::vec3 & v) {
        x() = v.x;
        y() = v.y;
        z() = v.z;
    }

    inline void set_rgb(const glm::vec4 & v) {
        r() = v.r;
        g() = v.g;
        b() = v.b;
        a() = v.a;
    }
};


LP3_GFX_API
template<typename Vertex>
class Triangle {
public:
    inline Triangle(GLfloat * _data)
    :   data(_data)
    {
        LP3_ASSERT(nullptr != data);
    }

    inline Triangle()
    :   data(nullptr) {
    }

    inline Vertex v1() {        // CCW, so (0.5, 0.0)
        return Vertex{data};
    }

    inline Vertex v2() {        // CCW, so (0.0, 1.0)
        return Vertex{data + Vertex::array_size};
    }

    inline Vertex v3() {        // CCW, so (01.0, 1.0)
        return Vertex{data + (Vertex::array_size * 2)};
    }

private:
    GLfloat * data;
};

template<typename T>
void upright_triangle(T & triangle, glm::vec2 a, glm::vec2 b, GLfloat z,
                      glm::vec2 u, glm::vec2 v) {
    glm::vec2 size = b - a;
    glm::vec2 t_size = v - u;
    triangle.v1().set(a.x + (size.x * 0.5f), a.y, z,
                      u.x + (t_size.x * 0.5f), u.y);
    triangle.v2().set(a.x, b.y, z, u.x, v.y);
    triangle.v3().set(b.x, b.y, z, v.x, v.y);
}

template<typename T>
void upright_triangle(T && triangle, glm::vec2 a, glm::vec2 b, GLfloat z,
                      glm::vec2 u, glm::vec2 v) {
    upright_triangle(triangle, a, b, z, u, v);
}

LP3_GFX_API
template<typename Vertex>
class Quad {
public:
    inline Quad(GLfloat * _data)
    :   data(_data)
    {
        LP3_ASSERT(nullptr != data);
    }

    inline Quad()
    :   data(nullptr) {
    }
    inline Vertex ul() {        // CCW, so (0.0, 0.0)
        return Vertex{data};
    }

    inline Vertex dl() {        // CCW, so (0.0, 1.0)
        return Vertex{data + Vertex::array_size};
    }

    inline Vertex dr() {        // CCW, so (1.0, 1.0)
        return Vertex{data + (Vertex::array_size * 2)};
    }

    inline Vertex ur() {        // CCW, so (1.0, 0.0)
        return Vertex{data + (Vertex::array_size * 3)};
    }

private:
    GLfloat * data;
};


// --------------------------------------------------------------------
// QuadArray
// --------------------------------------------------------------------
//		References GLfloat data in such a way that it can be treated
//		as an array of quads.
//		NOTE: This is a view- it does now own the memory it points to!
// --------------------------------------------------------------------
LP3_GFX_API
template<typename Vertex>
class QuadArray {
public:
	QuadArray()
	:	count(0),
		data(nullptr)
	{}

	QuadArray(GLfloat * _data, std::size_t _count)
	:	count(_count),
		data(_data)
	{}

	QuadArray(const QuadArray & other)
	:   count(other.count),
		data(other.data)
	{}

    QuadArray(QuadArray && other)
    :   count(other.count),
        data(other.data)
    {}

	QuadArray & operator=(QuadArray & rhs) {
		this->count = rhs.count;
		this->data = rhs.data;
	}

	Quad<Vertex> operator[](const std::size_t index) {
		LP3_ASSERT(index <= count);
		return Quad<Vertex>(data + (index * Vertex::array_size * 4));
	}

	inline const std::size_t size() const {
		return count;
	}

	QuadArray<Vertex> subset(const std::size_t index, const int _count = -1) {
		LP3_ASSERT(index < this->count);
		const std::size_t c
			= _count < 0
				? (this->count - lp3::narrow<std::size_t>(index))
				: _count;
		return QuadArray(data + (index * Vertex::array_size * 4), c);
	}
private:
	const std::size_t count;
	GLfloat * const data;
};


// Given the upper left and lower right vertices of a right angled quad, sets
// the other vertices.
template<typename Quad>
void upright_quad(Quad && quad, const glm::vec2 & a, const glm::vec2 & b,
                  const GLfloat z, const glm::vec2 & ta, const glm::vec2 & tb) {
    quad.ul().set(a.x, a.y, z, ta.x, ta.y);
    quad.dl().set(a.x, b.y, z, ta.x, tb.y);
    quad.dr().set(b.x, b.y, z, tb.x, tb.y);
    quad.ur().set(b.x, a.y, z, tb.x, ta.y);
}

template<typename Quad>
void upright_quad(Quad && quad, const glm::vec2 & a, const glm::vec2 & b,
                  const GLfloat z, const glm::vec2 & ta, const glm::vec2 & tb,
                  const glm::vec4 & color) {
    quad.ul().set(a.x, a.y, z, ta.x, ta.y, color.r, color.g, color.b, color.a);
    quad.dl().set(a.x, b.y, z, ta.x, tb.y, color.r, color.g, color.b, color.a);
    quad.dr().set(b.x, b.y, z, tb.x, tb.y, color.r, color.g, color.b, color.a);
    quad.ur().set(b.x, a.y, z, tb.x, ta.y, color.r, color.g, color.b, color.a);
}

} }

#endif
