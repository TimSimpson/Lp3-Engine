#pragma once
#ifndef FILE_LP3_GL_EXTRAS_HPP
#define FILE_LP3_GL_EXTRAS_HPP

#include <gsl/span>
#include <glm/glm.hpp>
/// #include <glm/fwd.hpp>
#include "config.hpp"
#include <lp3/core.hpp>
#include "gles2.h"
// #include <GL/glew.h>


namespace lp3 { namespace gl {

// Must be called after a GL context is created.
// Throws an exception if initialization fails. The other motivation for this
// function to make it possible to go back to GLEW if that becomes necessary.
LP3_GL_API
void initialize();

// Converts an error enumeration to a string.
LP3_GL_API
gsl::not_null<gsl::czstring<>> error_to_string(const GLenum);

// Throws an exception with glGetError in the string.
LP3_GL_API
void throw_gl_error();

inline bool no_errors() {
	return GL_NO_ERROR == glGetError();
}

#ifdef LP3_COMPILE_WITH_DEBUGGING
#define LP3_GL_ASSERT_NO_ERRORS() { \
	GLenum e = glGetError(); \
	if (GL_NO_ERROR != e) { \
		LP3_LOG_ERROR(::lp3::gl::error_to_string(e)); \
		SDL_assert(false); \
	} \
}

#else
	#define LP3_GL_ASSERT_NO_ERRORS() /* */
#endif

// Template which checks to see if a resource is nullptr on creation and
//  calls a deleter function on destruction.
template<typename ResourceType, void(*Deleter)(ResourceType)>
class GlOwner {
public:
    explicit GlOwner(GLuint _value)
        : value(ResourceType{ _value })
    {
        if (!value) {
            throw_gl_error();
        }
    }

	GlOwner()
		: value(ResourceType{}) {
	}

    GlOwner(GlOwner && other)
        : value(other.value)
    {
		other.value = ResourceType{};
    }

    ~GlOwner() {
        if (value) {
            Deleter(value);
        }
    }

	GlOwner & operator=(GlOwner && other) {
		if (this->value != other.value) {
			if (this->value) {
				Deleter(value);
			}
			this->value = other.value;
			other.value = ResourceType{};
		}
		return *this;
	}

    GlOwner & operator=(const GlOwner & other)
        = delete;

    inline operator ResourceType () const {
        return value;
    }

	inline operator GLuint() {
        return value.value;
    }
private:
    ResourceType value;
};

// Creates an alias type for GLuint. When only these alias types are used
// the compiler can flag errors where arguments get mixed up, say when
// passing a "Shader" where a "Program" is expected.
#define LP3_GL_WRAP_GLUINT_TYPE(NAME) \
    struct NAME { \
        GLuint value; \
        NAME() \
        : value() { \
        }\
		inline explicit NAME(GLuint _glInt) \
        : value(_glInt) { \
        }\
        inline operator GLuint() const { \
			return value; \
        } \
    };

#define LP3_GL_SUBCLASS(NAME, PARENT) \
	struct NAME : public PARENT { \
		NAME() \
		:	PARENT() \
		{} \
		explicit NAME(GLuint _gl_int) \
		:	PARENT(_gl_int) \
		{}	\
	};

// Creates an owner class.
#define LP3_GL_WRAP_GLUINT_TYPE_OWNER(NAME2, DELETER_FUNCTION) \
    inline void NAME2 ## _destroy(NAME2 val) { DELETER_FUNCTION(val); } \
    using NAME2 ## Owner = GlOwner<NAME2, NAME2 ## _destroy>;

// --------------------------------------------------------------------
// type strong aliases and owner classes
// --------------------------------------------------------------------
//      The following wrap GLuint in type strong aliases classes, which
//      can help avoid confusion in cases where functions accept
//      multiple GLuint parameters.
// --------------------------------------------------------------------
LP3_GL_WRAP_GLUINT_TYPE(BufferObjectName)

LP3_GL_WRAP_GLUINT_TYPE(Index)

LP3_GL_WRAP_GLUINT_TYPE(Program)

LP3_GL_WRAP_GLUINT_TYPE(Shader)
LP3_GL_WRAP_GLUINT_TYPE_OWNER(Shader, glDeleteShader)

LP3_GL_WRAP_GLUINT_TYPE(TextureID)

LP3_GL_WRAP_GLUINT_TYPE(VertexAttributeIndex)

LP3_GL_SUBCLASS(UniformVariableLocation, VertexAttributeIndex)

// --------------------------------------------------------------------
// Wrapped GL functions
// --------------------------------------------------------------------
//     Wraps OpenGL functions using the aliases above and checks for
//	   errors with debug assertions.
// --------------------------------------------------------------------

inline void attach_shader(Program program, Shader shader) {
	glAttachShader(program, shader);
	LP3_GL_ASSERT_NO_ERRORS();
}

inline void bind_attrib_location(Program program, VertexAttributeIndex index,
	                             gsl::czstring<> name) {
    LP3_ASSERT(name != nullptr);
	glBindAttribLocation(program, index, name);
	LP3_GL_ASSERT_NO_ERRORS();
}

inline void bind_buffer(GLenum target, BufferObjectName buffer) {
	glBindBuffer(target, buffer);
	LP3_GL_ASSERT_NO_ERRORS();
}

template<typename GlType>
inline void buffer_data(GLenum target, gsl::span<const GlType> data, GLenum usage) {
	const GLsizeiptr size = sizeof(GlType) * data.size();
	glBufferData(target, size, data.data(), usage);
	LP3_GL_ASSERT_NO_ERRORS();
}

inline void draw_arrays(GLenum mode, GLint first, GLsizei count) {
	glDrawArrays(mode, first, count);
	LP3_GL_ASSERT_NO_ERRORS();
}

inline void enable_vertex_attrib_array(VertexAttributeIndex index) {
	glEnableVertexAttribArray(index);
	LP3_GL_ASSERT_NO_ERRORS();
}

inline void disable_vertex_attrib_array(VertexAttributeIndex index) {
    glDisableVertexAttribArray(index);
    LP3_GL_ASSERT_NO_ERRORS();
}

template<std::size_t size>
void gen_buffers(BufferObjectName (&buffers)[size]) {
	GLuint gl_buffers[size];
	glGenBuffers(size, gl_buffers);
	LP3_GL_ASSERT_NO_ERRORS();
	for (std::size_t i = 0; i < size; ++i) {
		buffers[i].value = gl_buffers[i];
	}
}

inline BufferObjectName gen_buffer() {
	BufferObjectName bon[1];
	gen_buffers(bon);
	return bon[0];
}

inline UniformVariableLocation get_uniform_location(
		Program program, gsl::czstring<> name) {
    LP3_ASSERT(name != nullptr);
	UniformVariableLocation result(
		glGetUniformLocation(program, name));
	LP3_GL_ASSERT_NO_ERRORS();
	return result;
}

inline void use_program(Program program) {
	glUseProgram(program.value);
	LP3_GL_ASSERT_NO_ERRORS();
}

inline void vertex_attrib_4fv(VertexAttributeIndex index, const GLfloat(&v)[4]) {
	glVertexAttrib4fv(index, v);
	LP3_GL_ASSERT_NO_ERRORS();
}
inline void vertex_attrib_pointer(
	const VertexAttributeIndex index, GLint size, GLenum type,
	bool normalized, GLsizei stride, const void * pointer) {
	// pointer is the offset from the current thing stored in the buffer
	glVertexAttribPointer(
		index, size, type, normalized ? GL_TRUE : GL_FALSE,
		stride, pointer);
	LP3_GL_ASSERT_NO_ERRORS();
}

inline void uniform_matrix_4fv(UniformVariableLocation location,
	                           gsl::span<glm::mat4> value,
							   bool transpose=false) {
	glm::mat4 & ref = value[0];
	const GLfloat * values = &ref[0][0];
	glUniformMatrix4fv(GLint(location),
					   GLsizei(value.size()),
					   (transpose ? GL_TRUE : GL_FALSE),
		               values);
	LP3_GL_ASSERT_NO_ERRORS();
}

inline void uniform_matrix_4fv(UniformVariableLocation location,
							   glm::mat4 & value,
							   bool transpose = false) {
	uniform_matrix_4fv(location, gsl::make_span<glm::mat4>(&value, 1), transpose);
}

// --------------------------------------------------------------------
// Wrapped deleters / creators and owner objects.
// --------------------------------------------------------------------
// --------------------------------------------------------------------

inline void delete_texture(TextureID id) {
	GLuint gl_id = id;
	glDeleteTextures(1, &gl_id);
	LP3_GL_ASSERT_NO_ERRORS();
}

LP3_GL_WRAP_GLUINT_TYPE_OWNER(TextureID, delete_texture)

inline void delete_program(Program program) {
	glDeleteProgram(program);
	LP3_GL_ASSERT_NO_ERRORS();
}

LP3_GL_WRAP_GLUINT_TYPE_OWNER(Program, delete_program)

inline ProgramOwner create_program() {
	Program result{ glCreateProgram() };
	SDL_assert(result);
	return ProgramOwner{ result };
}


// --------------------------------------------------------------------
// GL Helper Functions
// --------------------------------------------------------------------
//     These functions offer a bit of extra utility and may combine
//     several OpenGL calls.
// --------------------------------------------------------------------
LP3_GL_API
ShaderOwner compile_shader(const GLenum type, gsl::czstring<> src);

LP3_GL_API
void link_program(Program program);

// -/

} }

#endif
