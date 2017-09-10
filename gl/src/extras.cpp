#define LP3_GL_API_CREATE
#include <glm/glm.hpp>
#include <lp3/gl/extras.hpp>
#include <vector>
#include <lp3/core.hpp>

namespace lp3 { namespace gl {

LP3_GL_API
void initialize() {
	if (ogl_LOAD_SUCCEEDED != ogl_LoadFunctions()) {
		LP3_LOG_ERROR("Error calling ogl_LoadFunctions");
		LP3_THROW2(lp3::core::Exception, "Error calling ogl_LoadFunctions");
	}
}

#define CASE(X) case X : { return #X ; }

LP3_GL_API
gsl::not_null<gsl::czstring<>> error_to_string(const GLenum error) {
    switch (error) {
        CASE(GL_NO_ERROR)
        CASE(GL_INVALID_ENUM)
        CASE(GL_INVALID_VALUE)
        CASE(GL_INVALID_OPERATION)
        CASE(GL_INVALID_FRAMEBUFFER_OPERATION)
        CASE(GL_OUT_OF_MEMORY)
		default:
			return "unknown";
    }
}

LP3_GL_API
void throw_gl_error() {
    #ifdef LP3_COMPILE_WITH_DEBUGGING
        const GLenum err = glGetError();
        LP3_LOG_ERROR("GL Error: %s", error_to_string(err));
    #endif
    LP3_THROW2(lp3::core::Exception, "OpenGL error.");
}

LP3_GL_API
ShaderOwner compile_shader(const GLenum type, gsl::not_null<gsl::czstring<>> src) {
    LP3_LOG_DEBUG("Compiling shader:\n%s", src);
	ShaderOwner shader{ glCreateShader(type) };
    if (!shader) {
        throw_gl_error();
    }
	const GLchar * src_as_ptr = src.get();
	// Expects array of char arrays. :/
    glShaderSource(shader, 1, &src_as_ptr, nullptr);
	LP3_GL_ASSERT_NO_ERRORS();
    glCompileShader(shader);
	LP3_GL_ASSERT_NO_ERRORS();
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	LP3_GL_ASSERT_NO_ERRORS();
    if (!compiled) {
        GLint info_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_length);
		LP3_GL_ASSERT_NO_ERRORS();
        if (info_length > 1) {
            std::vector<char> info_log(info_length, '.');
            glGetShaderInfoLog(shader, info_length, nullptr, info_log.data());
			LP3_GL_ASSERT_NO_ERRORS();
            LP3_LOG_ERROR("Error compiling shader: %s", info_log.data());
        }
        glDeleteShader(shader);
		LP3_GL_ASSERT_NO_ERRORS();
        LP3_THROW2(lp3::core::Exception, "OpenGL Error");

    }
    return shader;
}

LP3_GL_API
void link_program(Program obj) {
    glLinkProgram(obj);
	LP3_GL_ASSERT_NO_ERRORS();
    GLint linked;
    glGetProgramiv(obj, GL_LINK_STATUS, &linked);
	LP3_GL_ASSERT_NO_ERRORS();
    if (!linked) {
        GLint info_length;
        glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &info_length);
		LP3_GL_ASSERT_NO_ERRORS();
        if (info_length > 1) {
            std::vector<char> info_log(info_length, '.');
            glGetProgramInfoLog(obj, info_length, nullptr, info_log.data());
			LP3_GL_ASSERT_NO_ERRORS();
            LP3_LOG_ERROR("Error linking program: %s", info_log.data());
        }
        // Note: Program not deleted!
        LP3_THROW2(lp3::core::Exception, "OpenGL Error");
    }
}

}   }
