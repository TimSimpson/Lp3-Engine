#define LP3_CORE_API_CREATE
#include <lp3/gl.hpp>
#include <vector>
#include <lp3/core/Exception.hpp>

namespace lp3 { namespace gl {

#define CASE(X) case X : { return #X ; }

const char * error_to_string(const GLenum error) {
    switch (error) {
        CASE(GL_NO_ERROR)
        CASE(GL_INVALID_ENUM)
        CASE(GL_INVALID_VALUE)
        CASE(GL_INVALID_OPERATION)
        CASE(GL_INVALID_FRAMEBUFFER_OPERATION)
        CASE(GL_OUT_OF_MEMORY)
        CASE(GL_STACK_UNDERFLOW)
        CASE(GL_STACK_OVERFLOW)
        default:
            return "unknown";
    }
}

void throw_gl_error() {
    #ifdef LP3_COMPILE_WITH_DEBUGGING
        const GLenum err = glGetError();
        LP3_LOG_ERROR("GL Error: %s", error_to_string(err));
    #endif
    LP3_THROW2(lp3::core::Exception, "OpenGL error.");
}

ShaderOwner compile_shader(const GLenum type, const char * src) {
    ShaderOwner shader = glCreateShader(type);
    if (!shader) {
        throw_gl_error();
    }
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint info_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_length);
        if (info_length > 1) {
            std::vector<char> info_log(info_length, '.');
            glGetShaderInfoLog(shader, info_length, nullptr, info_log.data());
            LP3_LOG_ERROR("Error compiling shader: %s", info_log.data());
        }
        glDeleteShader(shader);
        LP3_THROW2(lp3::core::Exception, "OpenGL Error");
    }
    return shader;
}

void link_program(Program obj) {
    glLinkProgram(obj);
    GLint linked;
    glGetProgramiv(obj, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint info_length;
        glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &info_length);
        if (info_length > 1) {
            std::vector<char> info_log(info_length, '.');
            glGetProgramInfoLog(obj, info_length, nullptr, info_log.data());
            LP3_LOG_ERROR("Error linking program: %s", info_log.data());
        }
        // Note: Program not deleted!
        LP3_THROW2(lp3::core::Exception, "OpenGL Error");
    }
}

}   }
