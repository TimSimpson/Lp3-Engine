#pragma once
#ifndef FILE_LP3_GL_HPP
#define FILE_LP3_GL_HPP

#include "core/config.hpp"
#include "core/Exception.hpp"
#include "log.hpp"
#include <GL/glew.h>


namespace lp3 { namespace gl {

LP3_CORE_API
const char * error_to_string(const GLenum);

LP3_CORE_API
void throw_gl_error();

/*
    Template which checks to see if a resource is nullptr on creation and
    calls a deleter function on destruction.
 */
template<typename ResourceType, void(*Deleter)(ResourceType)>
class GlOwner {
public:
	GlOwner(GLuint _value)
		: value(ResourceType{ _value })
	{
		if (!value) {
			throw_gl_error();
		}
	}

	GlOwner(GlOwner && other) 
		: value(other.value) 
	{
		other.value = 0;
	}

    ~GlOwner() {
        if (value) {
            Deleter(value);
        }
    }

    GlOwner & operator=(const GlOwner & other)
        = delete;

    operator ResourceType () {
        return value;
    }
	operator GLuint() {	
		return value.value;
	}
private:
    ResourceType value;
};

#define LP3_GL_WRAP_GLUINT_TYPE(NAME) \
	struct NAME { \
		GLuint value; \
		NAME(GLuint _glInt) \
		: value(_glInt) { \
		}\
		operator GLuint() { \
				return value; \
		} \
	};



#define LP3_GL_WRAP_GLUINT_TYPE_OWNER(NAME2, DELETER_FUNCTION) \
	inline void NAME2 ## _destroy(NAME2 val) { DELETER_FUNCTION(val); } \
	using NAME2 ## Owner = GlOwner<NAME2, NAME2 ## _destroy>;
	
LP3_GL_WRAP_GLUINT_TYPE(Shader)
LP3_GL_WRAP_GLUINT_TYPE_OWNER(Shader, glDeleteShader)

LP3_GL_WRAP_GLUINT_TYPE(Program)
LP3_GL_WRAP_GLUINT_TYPE_OWNER(Program, glDeleteProgram)

LP3_CORE_API
ShaderOwner compile_shader(const GLenum type, const char * src);

LP3_CORE_API
void link_program(Program program);

} }

#endif
