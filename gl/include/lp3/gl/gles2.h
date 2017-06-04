#ifndef POINTER_C_GENERATED_HEADER_OPENGLES_H
#define POINTER_C_GENERATED_HEADER_OPENGLES_H

#if defined(__glew_h__) || defined(__GLEW_H__)
#error Attempt to include auto-generated header after including glew.h
#endif
#if defined(__gl_h_) || defined(__GL_H__)
#error Attempt to include auto-generated header after including gl.h
#endif
#if defined(__glext_h_) || defined(__GLEXT_H_)
#error Attempt to include auto-generated header after including glext.h
#endif
#if defined(__gl2_h_) || defined(__GL2_H__)
#error Attempt to include auto-generated header after including GLES2/gl2.h
#endif
#if defined(__gl2ext_h_) || defined(__GL2EXT_H_)
#error Attempt to include auto-generated header after including GLES2/gl2ext.h
#endif
#if defined(__gl3_h_) || defined(__GL3_H__)
#error Attempt to include auto-generated header after including GLES3/gl3.h
#endif
#if defined(__gl31_h_) || defined(__GL31_H__)
#error Attempt to include auto-generated header after including GLES3/gl31.h
#endif
#if defined(__gl32_h_) || defined(__GL32_H__)
#error Attempt to include auto-generated header after including GLES3/gl32.h
#endif
#if defined(__gles20_h_)
#error Attempt to include auto-generated header after including ES2/gl.h
#endif
#if defined(__gles20ext_h_)
#error Attempt to include auto-generated header after including ES2/glext.h
#endif
#if defined(__gles30_h_)
#error Attempt to include auto-generated header after including ES3/gl.h
#endif
#if defined(__gles30ext_h_)
#error Attempt to include auto-generated header after including ES3/glext.h
#endif
#if defined(__gltypes_h_)
#error Attempt to include auto-generated header after gltypes.h
#endif

#define __glew_h__
#define __GLEW_H__
#define __gl_h_
#define __GL_H__
#define __glext_h_
#define __GLEXT_H_
#define __gl2_h_
#define __GL2_H__
#define __gl2ext_h_
#define __GL2EXT_H_
#define __gl3_h_
#define __GL3_H__
#define __gl31_h_
#define __GL31_H__
#define __gl32_h_
#define __GL32_H__
#define __gles20_h_
#define __gles20ext_h_
#define __gles30_h_
#define __gles30ext_h_
#define __gltypes_h_

#ifndef APIENTRY
    #if defined(__MINGW32__)
        #ifndef WIN32_LEAN_AND_MEAN
            #define WIN32_LEAN_AND_MEAN 1
        #endif
        #ifndef NOMINMAX
            #define NOMINMAX
        #endif
        #include <windows.h>
    #elif (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__)
        #ifndef WIN32_LEAN_AND_MEAN
            #define WIN32_LEAN_AND_MEAN 1
        #endif
        #ifndef NOMINMAX
            #define NOMINMAX
        #endif
        #include <windows.h>
    #else
        #define APIENTRY
    #endif
#endif /*APIENTRY*/

#ifndef CODEGEN_FUNCPTR
    #define CODEGEN_REMOVE_FUNCPTR
    #if defined(_WIN32)
        #define CODEGEN_FUNCPTR APIENTRY
    #else
        #define CODEGEN_FUNCPTR
    #endif
#endif /*CODEGEN_FUNCPTR*/

#ifndef GLAPI
    #define GLAPI extern
#endif


#ifndef GL_LOAD_GEN_BASIC_OPENGL_TYPEDEFS
#define GL_LOAD_GEN_BASIC_OPENGL_TYPEDEFS


#endif /*GL_LOAD_GEN_BASIC_OPENGL_TYPEDEFS*/


#include <stddef.h>
#ifndef GLEXT_64_TYPES_DEFINED
/* This code block is duplicated in glxext.h, so must be protected */
#define GLEXT_64_TYPES_DEFINED
/* Define int32_t, int64_t, and uint64_t types for UST/MSC */
/* (as used in the GL_EXT_timer_query extension). */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#include <inttypes.h>
#elif defined(__sun__) || defined(__digital__)
#include <inttypes.h>
#if defined(__STDC__)
#if defined(__arch64__) || defined(_LP64)
typedef long int int64_t;
typedef unsigned long int uint64_t;
#else
typedef long long int int64_t;
typedef unsigned long long int uint64_t;
#endif /* __arch64__ */
#endif /* __STDC__ */
#elif defined( __VMS ) || defined(__sgi)
#include <inttypes.h>
#elif defined(__SCO__) || defined(__USLC__)
#include <stdint.h>
#elif defined(__UNIXOS2__) || defined(__SOL64__)
typedef long int int32_t;
typedef long long int int64_t;
typedef unsigned long long int uint64_t;
#elif defined(_WIN32) && defined(__GNUC__)
#include <stdint.h>
#elif defined(_WIN32)
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
/* Fallback if nothing above works */
#include <inttypes.h>
#endif
#endif
    typedef unsigned int GLenum;
    typedef unsigned char GLboolean;
    typedef unsigned int GLbitfield;
    typedef void GLvoid;
    typedef signed char GLbyte;
    typedef short GLshort;
    typedef int GLint;
    typedef unsigned char GLubyte;
    typedef unsigned short GLushort;
    typedef unsigned int GLuint;
    typedef int GLsizei;
    typedef float GLfloat;
    typedef float GLclampf;
    typedef void *GLeglImageOES;
    typedef char GLchar;
    typedef unsigned short GLhalf;
    typedef GLint GLfixed;
    typedef ptrdiff_t GLintptr;
    typedef ptrdiff_t GLsizeiptr;
    typedef int64_t GLint64;
    typedef uint64_t GLuint64;
    typedef struct __GLsync *GLsync;
    typedef void (APIENTRY *GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
    typedef void (APIENTRY *GLDEBUGPROCKHR)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

#define GL_ACTIVE_ATTRIBUTES 0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH 0x8B8A
#define GL_ACTIVE_TEXTURE 0x84E0
#define GL_ACTIVE_UNIFORMS 0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH 0x8B87
#define GL_ALIASED_LINE_WIDTH_RANGE 0x846E
#define GL_ALIASED_POINT_SIZE_RANGE 0x846D
#define GL_ALPHA 0x1906
#define GL_ALPHA_BITS 0x0D55
#define GL_ALWAYS 0x0207
#define GL_ARRAY_BUFFER 0x8892
#define GL_ARRAY_BUFFER_BINDING 0x8894
#define GL_ATTACHED_SHADERS 0x8B85
#define GL_BACK 0x0405
#define GL_BLEND 0x0BE2
#define GL_BLEND_COLOR 0x8005
#define GL_BLEND_DST_ALPHA 0x80CA
#define GL_BLEND_DST_RGB 0x80C8
#define GL_BLEND_EQUATION 0x8009
#define GL_BLEND_EQUATION_ALPHA 0x883D
#define GL_BLEND_EQUATION_RGB 0x8009
#define GL_BLEND_SRC_ALPHA 0x80CB
#define GL_BLEND_SRC_RGB 0x80C9
#define GL_BLUE_BITS 0x0D54
#define GL_BOOL 0x8B56
#define GL_BOOL_VEC2 0x8B57
#define GL_BOOL_VEC3 0x8B58
#define GL_BOOL_VEC4 0x8B59
#define GL_BUFFER_SIZE 0x8764
#define GL_BUFFER_USAGE 0x8765
#define GL_BYTE 0x1400
#define GL_CCW 0x0901
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_COLOR_CLEAR_VALUE 0x0C22
#define GL_COLOR_WRITEMASK 0x0C23
#define GL_COMPILE_STATUS 0x8B81
#define GL_COMPRESSED_TEXTURE_FORMATS 0x86A3
#define GL_CONSTANT_ALPHA 0x8003
#define GL_CONSTANT_COLOR 0x8001
#define GL_CULL_FACE 0x0B44
#define GL_CULL_FACE_MODE 0x0B45
#define GL_CURRENT_PROGRAM 0x8B8D
#define GL_CURRENT_VERTEX_ATTRIB 0x8626
#define GL_CW 0x0900
#define GL_DECR 0x1E03
#define GL_DECR_WRAP 0x8508
#define GL_DELETE_STATUS 0x8B80
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_DEPTH_BITS 0x0D56
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_DEPTH_CLEAR_VALUE 0x0B73
#define GL_DEPTH_COMPONENT 0x1902
#define GL_DEPTH_COMPONENT16 0x81A5
#define GL_DEPTH_FUNC 0x0B74
#define GL_DEPTH_RANGE 0x0B70
#define GL_DEPTH_TEST 0x0B71
#define GL_DEPTH_WRITEMASK 0x0B72
#define GL_DITHER 0x0BD0
#define GL_DONT_CARE 0x1100
#define GL_DST_ALPHA 0x0304
#define GL_DST_COLOR 0x0306
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_ELEMENT_ARRAY_BUFFER_BINDING 0x8895
#define GL_EQUAL 0x0202
#define GL_EXTENSIONS 0x1F03
#define GL_FALSE 0
#define GL_FASTEST 0x1101
#define GL_FIXED 0x140C
#define GL_FLOAT 0x1406
#define GL_FLOAT_MAT2 0x8B5A
#define GL_FLOAT_MAT3 0x8B5B
#define GL_FLOAT_MAT4 0x8B5C
#define GL_FLOAT_VEC2 0x8B50
#define GL_FLOAT_VEC3 0x8B51
#define GL_FLOAT_VEC4 0x8B52
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_FRAMEBUFFER 0x8D40
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_BINDING 0x8CA6
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS 0x8CD9
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_UNSUPPORTED 0x8CDD
#define GL_FRONT 0x0404
#define GL_FRONT_AND_BACK 0x0408
#define GL_FRONT_FACE 0x0B46
#define GL_FUNC_ADD 0x8006
#define GL_FUNC_REVERSE_SUBTRACT 0x800B
#define GL_FUNC_SUBTRACT 0x800A
#define GL_GENERATE_MIPMAP_HINT 0x8192
#define GL_GEQUAL 0x0206
#define GL_GREATER 0x0204
#define GL_GREEN_BITS 0x0D53
#define GL_HIGH_FLOAT 0x8DF2
#define GL_HIGH_INT 0x8DF5
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT 0x8B9B
#define GL_IMPLEMENTATION_COLOR_READ_TYPE 0x8B9A
#define GL_INCR 0x1E02
#define GL_INCR_WRAP 0x8507
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_INT 0x1404
#define GL_INT_VEC2 0x8B53
#define GL_INT_VEC3 0x8B54
#define GL_INT_VEC4 0x8B55
#define GL_INVALID_ENUM 0x0500
#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#define GL_INVALID_OPERATION 0x0502
#define GL_INVALID_VALUE 0x0501
#define GL_INVERT 0x150A
#define GL_KEEP 0x1E00
#define GL_LEQUAL 0x0203
#define GL_LESS 0x0201
#define GL_LINEAR 0x2601
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#define GL_LINES 0x0001
#define GL_LINE_LOOP 0x0002
#define GL_LINE_STRIP 0x0003
#define GL_LINE_WIDTH 0x0B21
#define GL_LINK_STATUS 0x8B82
#define GL_LOW_FLOAT 0x8DF0
#define GL_LOW_INT 0x8DF3
#define GL_LUMINANCE 0x1909
#define GL_LUMINANCE_ALPHA 0x190A
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE 0x851C
#define GL_MAX_FRAGMENT_UNIFORM_VECTORS 0x8DFD
#define GL_MAX_RENDERBUFFER_SIZE 0x84E8
#define GL_MAX_TEXTURE_IMAGE_UNITS 0x8872
#define GL_MAX_TEXTURE_SIZE 0x0D33
#define GL_MAX_VARYING_VECTORS 0x8DFC
#define GL_MAX_VERTEX_ATTRIBS 0x8869
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define GL_MAX_VERTEX_UNIFORM_VECTORS 0x8DFB
#define GL_MAX_VIEWPORT_DIMS 0x0D3A
#define GL_MEDIUM_FLOAT 0x8DF1
#define GL_MEDIUM_INT 0x8DF4
#define GL_MIRRORED_REPEAT 0x8370
#define GL_NEAREST 0x2600
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_NEVER 0x0200
#define GL_NICEST 0x1102
#define GL_NONE 0
#define GL_NOTEQUAL 0x0205
#define GL_NO_ERROR 0
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_NUM_SHADER_BINARY_FORMATS 0x8DF9
#define GL_ONE 1
#define GL_ONE_MINUS_CONSTANT_ALPHA 0x8004
#define GL_ONE_MINUS_CONSTANT_COLOR 0x8002
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_OUT_OF_MEMORY 0x0505
#define GL_PACK_ALIGNMENT 0x0D05
#define GL_POINTS 0x0000
#define GL_POLYGON_OFFSET_FACTOR 0x8038
#define GL_POLYGON_OFFSET_FILL 0x8037
#define GL_POLYGON_OFFSET_UNITS 0x2A00
#define GL_RED_BITS 0x0D52
#define GL_RENDERBUFFER 0x8D41
#define GL_RENDERBUFFER_ALPHA_SIZE 0x8D53
#define GL_RENDERBUFFER_BINDING 0x8CA7
#define GL_RENDERBUFFER_BLUE_SIZE 0x8D52
#define GL_RENDERBUFFER_DEPTH_SIZE 0x8D54
#define GL_RENDERBUFFER_GREEN_SIZE 0x8D51
#define GL_RENDERBUFFER_HEIGHT 0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT 0x8D44
#define GL_RENDERBUFFER_RED_SIZE 0x8D50
#define GL_RENDERBUFFER_STENCIL_SIZE 0x8D55
#define GL_RENDERBUFFER_WIDTH 0x8D42
#define GL_RENDERER 0x1F01
#define GL_REPEAT 0x2901
#define GL_REPLACE 0x1E01
#define GL_RGB 0x1907
#define GL_RGB565 0x8D62
#define GL_RGB5_A1 0x8057
#define GL_RGBA 0x1908
#define GL_RGBA4 0x8056
#define GL_SAMPLER_2D 0x8B5E
#define GL_SAMPLER_CUBE 0x8B60
#define GL_SAMPLES 0x80A9
#define GL_SAMPLE_ALPHA_TO_COVERAGE 0x809E
#define GL_SAMPLE_BUFFERS 0x80A8
#define GL_SAMPLE_COVERAGE 0x80A0
#define GL_SAMPLE_COVERAGE_INVERT 0x80AB
#define GL_SAMPLE_COVERAGE_VALUE 0x80AA
#define GL_SCISSOR_BOX 0x0C10
#define GL_SCISSOR_TEST 0x0C11
#define GL_SHADER_BINARY_FORMATS 0x8DF8
#define GL_SHADER_COMPILER 0x8DFA
#define GL_SHADER_SOURCE_LENGTH 0x8B88
#define GL_SHADER_TYPE 0x8B4F
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#define GL_SHORT 0x1402
#define GL_SRC_ALPHA 0x0302
#define GL_SRC_ALPHA_SATURATE 0x0308
#define GL_SRC_COLOR 0x0300
#define GL_STATIC_DRAW 0x88E4
#define GL_STENCIL_ATTACHMENT 0x8D20
#define GL_STENCIL_BACK_FAIL 0x8801
#define GL_STENCIL_BACK_FUNC 0x8800
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL 0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS 0x8803
#define GL_STENCIL_BACK_REF 0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK 0x8CA4
#define GL_STENCIL_BACK_WRITEMASK 0x8CA5
#define GL_STENCIL_BITS 0x0D57
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_STENCIL_CLEAR_VALUE 0x0B91
#define GL_STENCIL_FAIL 0x0B94
#define GL_STENCIL_FUNC 0x0B92
#define GL_STENCIL_INDEX8 0x8D48
#define GL_STENCIL_PASS_DEPTH_FAIL 0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS 0x0B96
#define GL_STENCIL_REF 0x0B97
#define GL_STENCIL_TEST 0x0B90
#define GL_STENCIL_VALUE_MASK 0x0B93
#define GL_STENCIL_WRITEMASK 0x0B98
#define GL_STREAM_DRAW 0x88E0
#define GL_SUBPIXEL_BITS 0x0D50
#define GL_TEXTURE 0x1702
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE10 0x84CA
#define GL_TEXTURE11 0x84CB
#define GL_TEXTURE12 0x84CC
#define GL_TEXTURE13 0x84CD
#define GL_TEXTURE14 0x84CE
#define GL_TEXTURE15 0x84CF
#define GL_TEXTURE16 0x84D0
#define GL_TEXTURE17 0x84D1
#define GL_TEXTURE18 0x84D2
#define GL_TEXTURE19 0x84D3
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE20 0x84D4
#define GL_TEXTURE21 0x84D5
#define GL_TEXTURE22 0x84D6
#define GL_TEXTURE23 0x84D7
#define GL_TEXTURE24 0x84D8
#define GL_TEXTURE25 0x84D9
#define GL_TEXTURE26 0x84DA
#define GL_TEXTURE27 0x84DB
#define GL_TEXTURE28 0x84DC
#define GL_TEXTURE29 0x84DD
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE30 0x84DE
#define GL_TEXTURE31 0x84DF
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7
#define GL_TEXTURE8 0x84C8
#define GL_TEXTURE9 0x84C9
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_BINDING_2D 0x8069
#define GL_TEXTURE_BINDING_CUBE_MAP 0x8514
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_FAN 0x0006
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRUE 1
#define GL_UNPACK_ALIGNMENT 0x0CF5
#define GL_UNSIGNED_BYTE 0x1401
#define GL_UNSIGNED_INT 0x1405
#define GL_UNSIGNED_SHORT 0x1403
#define GL_UNSIGNED_SHORT_4_4_4_4 0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1 0x8034
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#define GL_VALIDATE_STATUS 0x8B83
#define GL_VENDOR 0x1F00
#define GL_VERSION 0x1F02
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED 0x8622
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#define GL_VERTEX_ATTRIB_ARRAY_POINTER 0x8645
#define GL_VERTEX_ATTRIB_ARRAY_SIZE 0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE 0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE 0x8625
#define GL_VERTEX_SHADER 0x8B31
#define GL_VIEWPORT 0x0BA2
#define GL_ZERO 0

extern void (CODEGEN_FUNCPTR *_ptrc_glActiveTexture)(GLenum texture);
#define glActiveTexture _ptrc_glActiveTexture
extern void (CODEGEN_FUNCPTR *_ptrc_glAttachShader)(GLuint program, GLuint shader);
#define glAttachShader _ptrc_glAttachShader
extern void (CODEGEN_FUNCPTR *_ptrc_glBindAttribLocation)(GLuint program, GLuint index, const GLchar * name);
#define glBindAttribLocation _ptrc_glBindAttribLocation
extern void (CODEGEN_FUNCPTR *_ptrc_glBindBuffer)(GLenum target, GLuint buffer);
#define glBindBuffer _ptrc_glBindBuffer
extern void (CODEGEN_FUNCPTR *_ptrc_glBindFramebuffer)(GLenum target, GLuint framebuffer);
#define glBindFramebuffer _ptrc_glBindFramebuffer
extern void (CODEGEN_FUNCPTR *_ptrc_glBindRenderbuffer)(GLenum target, GLuint renderbuffer);
#define glBindRenderbuffer _ptrc_glBindRenderbuffer
extern void (CODEGEN_FUNCPTR *_ptrc_glBindTexture)(GLenum target, GLuint texture);
#define glBindTexture _ptrc_glBindTexture
extern void (CODEGEN_FUNCPTR *_ptrc_glBlendColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
#define glBlendColor _ptrc_glBlendColor
extern void (CODEGEN_FUNCPTR *_ptrc_glBlendEquation)(GLenum mode);
#define glBlendEquation _ptrc_glBlendEquation
extern void (CODEGEN_FUNCPTR *_ptrc_glBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha);
#define glBlendEquationSeparate _ptrc_glBlendEquationSeparate
extern void (CODEGEN_FUNCPTR *_ptrc_glBlendFunc)(GLenum sfactor, GLenum dfactor);
#define glBlendFunc _ptrc_glBlendFunc
extern void (CODEGEN_FUNCPTR *_ptrc_glBlendFuncSeparate)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
#define glBlendFuncSeparate _ptrc_glBlendFuncSeparate
extern void (CODEGEN_FUNCPTR *_ptrc_glBufferData)(GLenum target, GLsizeiptr size, const void * data, GLenum usage);
#define glBufferData _ptrc_glBufferData
extern void (CODEGEN_FUNCPTR *_ptrc_glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const void * data);
#define glBufferSubData _ptrc_glBufferSubData
extern GLenum (CODEGEN_FUNCPTR *_ptrc_glCheckFramebufferStatus)(GLenum target);
#define glCheckFramebufferStatus _ptrc_glCheckFramebufferStatus
extern void (CODEGEN_FUNCPTR *_ptrc_glClear)(GLbitfield mask);
#define glClear _ptrc_glClear
extern void (CODEGEN_FUNCPTR *_ptrc_glClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
#define glClearColor _ptrc_glClearColor
extern void (CODEGEN_FUNCPTR *_ptrc_glClearDepthf)(GLfloat d);
#define glClearDepthf _ptrc_glClearDepthf
extern void (CODEGEN_FUNCPTR *_ptrc_glClearStencil)(GLint s);
#define glClearStencil _ptrc_glClearStencil
extern void (CODEGEN_FUNCPTR *_ptrc_glColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
#define glColorMask _ptrc_glColorMask
extern void (CODEGEN_FUNCPTR *_ptrc_glCompileShader)(GLuint shader);
#define glCompileShader _ptrc_glCompileShader
extern void (CODEGEN_FUNCPTR *_ptrc_glCompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void * data);
#define glCompressedTexImage2D _ptrc_glCompressedTexImage2D
extern void (CODEGEN_FUNCPTR *_ptrc_glCompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void * data);
#define glCompressedTexSubImage2D _ptrc_glCompressedTexSubImage2D
extern void (CODEGEN_FUNCPTR *_ptrc_glCopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
#define glCopyTexImage2D _ptrc_glCopyTexImage2D
extern void (CODEGEN_FUNCPTR *_ptrc_glCopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
#define glCopyTexSubImage2D _ptrc_glCopyTexSubImage2D
extern GLuint (CODEGEN_FUNCPTR *_ptrc_glCreateProgram)(void);
#define glCreateProgram _ptrc_glCreateProgram
extern GLuint (CODEGEN_FUNCPTR *_ptrc_glCreateShader)(GLenum type);
#define glCreateShader _ptrc_glCreateShader
extern void (CODEGEN_FUNCPTR *_ptrc_glCullFace)(GLenum mode);
#define glCullFace _ptrc_glCullFace
extern void (CODEGEN_FUNCPTR *_ptrc_glDeleteBuffers)(GLsizei n, const GLuint * buffers);
#define glDeleteBuffers _ptrc_glDeleteBuffers
extern void (CODEGEN_FUNCPTR *_ptrc_glDeleteFramebuffers)(GLsizei n, const GLuint * framebuffers);
#define glDeleteFramebuffers _ptrc_glDeleteFramebuffers
extern void (CODEGEN_FUNCPTR *_ptrc_glDeleteProgram)(GLuint program);
#define glDeleteProgram _ptrc_glDeleteProgram
extern void (CODEGEN_FUNCPTR *_ptrc_glDeleteRenderbuffers)(GLsizei n, const GLuint * renderbuffers);
#define glDeleteRenderbuffers _ptrc_glDeleteRenderbuffers
extern void (CODEGEN_FUNCPTR *_ptrc_glDeleteShader)(GLuint shader);
#define glDeleteShader _ptrc_glDeleteShader
extern void (CODEGEN_FUNCPTR *_ptrc_glDeleteTextures)(GLsizei n, const GLuint * textures);
#define glDeleteTextures _ptrc_glDeleteTextures
extern void (CODEGEN_FUNCPTR *_ptrc_glDepthFunc)(GLenum func);
#define glDepthFunc _ptrc_glDepthFunc
extern void (CODEGEN_FUNCPTR *_ptrc_glDepthMask)(GLboolean flag);
#define glDepthMask _ptrc_glDepthMask
extern void (CODEGEN_FUNCPTR *_ptrc_glDepthRangef)(GLfloat n, GLfloat f);
#define glDepthRangef _ptrc_glDepthRangef
extern void (CODEGEN_FUNCPTR *_ptrc_glDetachShader)(GLuint program, GLuint shader);
#define glDetachShader _ptrc_glDetachShader
extern void (CODEGEN_FUNCPTR *_ptrc_glDisable)(GLenum cap);
#define glDisable _ptrc_glDisable
extern void (CODEGEN_FUNCPTR *_ptrc_glDisableVertexAttribArray)(GLuint index);
#define glDisableVertexAttribArray _ptrc_glDisableVertexAttribArray
extern void (CODEGEN_FUNCPTR *_ptrc_glDrawArrays)(GLenum mode, GLint first, GLsizei count);
#define glDrawArrays _ptrc_glDrawArrays
extern void (CODEGEN_FUNCPTR *_ptrc_glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void * indices);
#define glDrawElements _ptrc_glDrawElements
extern void (CODEGEN_FUNCPTR *_ptrc_glEnable)(GLenum cap);
#define glEnable _ptrc_glEnable
extern void (CODEGEN_FUNCPTR *_ptrc_glEnableVertexAttribArray)(GLuint index);
#define glEnableVertexAttribArray _ptrc_glEnableVertexAttribArray
extern void (CODEGEN_FUNCPTR *_ptrc_glFinish)(void);
#define glFinish _ptrc_glFinish
extern void (CODEGEN_FUNCPTR *_ptrc_glFlush)(void);
#define glFlush _ptrc_glFlush
extern void (CODEGEN_FUNCPTR *_ptrc_glFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
#define glFramebufferRenderbuffer _ptrc_glFramebufferRenderbuffer
extern void (CODEGEN_FUNCPTR *_ptrc_glFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
#define glFramebufferTexture2D _ptrc_glFramebufferTexture2D
extern void (CODEGEN_FUNCPTR *_ptrc_glFrontFace)(GLenum mode);
#define glFrontFace _ptrc_glFrontFace
extern void (CODEGEN_FUNCPTR *_ptrc_glGenBuffers)(GLsizei n, GLuint * buffers);
#define glGenBuffers _ptrc_glGenBuffers
extern void (CODEGEN_FUNCPTR *_ptrc_glGenFramebuffers)(GLsizei n, GLuint * framebuffers);
#define glGenFramebuffers _ptrc_glGenFramebuffers
extern void (CODEGEN_FUNCPTR *_ptrc_glGenRenderbuffers)(GLsizei n, GLuint * renderbuffers);
#define glGenRenderbuffers _ptrc_glGenRenderbuffers
extern void (CODEGEN_FUNCPTR *_ptrc_glGenTextures)(GLsizei n, GLuint * textures);
#define glGenTextures _ptrc_glGenTextures
extern void (CODEGEN_FUNCPTR *_ptrc_glGenerateMipmap)(GLenum target);
#define glGenerateMipmap _ptrc_glGenerateMipmap
extern void (CODEGEN_FUNCPTR *_ptrc_glGetActiveAttrib)(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name);
#define glGetActiveAttrib _ptrc_glGetActiveAttrib
extern void (CODEGEN_FUNCPTR *_ptrc_glGetActiveUniform)(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name);
#define glGetActiveUniform _ptrc_glGetActiveUniform
extern void (CODEGEN_FUNCPTR *_ptrc_glGetAttachedShaders)(GLuint program, GLsizei maxCount, GLsizei * count, GLuint * shaders);
#define glGetAttachedShaders _ptrc_glGetAttachedShaders
extern GLint (CODEGEN_FUNCPTR *_ptrc_glGetAttribLocation)(GLuint program, const GLchar * name);
#define glGetAttribLocation _ptrc_glGetAttribLocation
extern void (CODEGEN_FUNCPTR *_ptrc_glGetBooleanv)(GLenum pname, GLboolean * data);
#define glGetBooleanv _ptrc_glGetBooleanv
extern void (CODEGEN_FUNCPTR *_ptrc_glGetBufferParameteriv)(GLenum target, GLenum pname, GLint * params);
#define glGetBufferParameteriv _ptrc_glGetBufferParameteriv
extern GLenum (CODEGEN_FUNCPTR *_ptrc_glGetError)(void);
#define glGetError _ptrc_glGetError
extern void (CODEGEN_FUNCPTR *_ptrc_glGetFloatv)(GLenum pname, GLfloat * data);
#define glGetFloatv _ptrc_glGetFloatv
extern void (CODEGEN_FUNCPTR *_ptrc_glGetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint * params);
#define glGetFramebufferAttachmentParameteriv _ptrc_glGetFramebufferAttachmentParameteriv
extern void (CODEGEN_FUNCPTR *_ptrc_glGetIntegerv)(GLenum pname, GLint * data);
#define glGetIntegerv _ptrc_glGetIntegerv
extern void (CODEGEN_FUNCPTR *_ptrc_glGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog);
#define glGetProgramInfoLog _ptrc_glGetProgramInfoLog
extern void (CODEGEN_FUNCPTR *_ptrc_glGetProgramiv)(GLuint program, GLenum pname, GLint * params);
#define glGetProgramiv _ptrc_glGetProgramiv
extern void (CODEGEN_FUNCPTR *_ptrc_glGetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint * params);
#define glGetRenderbufferParameteriv _ptrc_glGetRenderbufferParameteriv
extern void (CODEGEN_FUNCPTR *_ptrc_glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog);
#define glGetShaderInfoLog _ptrc_glGetShaderInfoLog
extern void (CODEGEN_FUNCPTR *_ptrc_glGetShaderPrecisionFormat)(GLenum shadertype, GLenum precisiontype, GLint * range, GLint * precision);
#define glGetShaderPrecisionFormat _ptrc_glGetShaderPrecisionFormat
extern void (CODEGEN_FUNCPTR *_ptrc_glGetShaderSource)(GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * source);
#define glGetShaderSource _ptrc_glGetShaderSource
extern void (CODEGEN_FUNCPTR *_ptrc_glGetShaderiv)(GLuint shader, GLenum pname, GLint * params);
#define glGetShaderiv _ptrc_glGetShaderiv
extern const GLubyte * (CODEGEN_FUNCPTR *_ptrc_glGetString)(GLenum name);
#define glGetString _ptrc_glGetString
extern void (CODEGEN_FUNCPTR *_ptrc_glGetTexParameterfv)(GLenum target, GLenum pname, GLfloat * params);
#define glGetTexParameterfv _ptrc_glGetTexParameterfv
extern void (CODEGEN_FUNCPTR *_ptrc_glGetTexParameteriv)(GLenum target, GLenum pname, GLint * params);
#define glGetTexParameteriv _ptrc_glGetTexParameteriv
extern GLint (CODEGEN_FUNCPTR *_ptrc_glGetUniformLocation)(GLuint program, const GLchar * name);
#define glGetUniformLocation _ptrc_glGetUniformLocation
extern void (CODEGEN_FUNCPTR *_ptrc_glGetUniformfv)(GLuint program, GLint location, GLfloat * params);
#define glGetUniformfv _ptrc_glGetUniformfv
extern void (CODEGEN_FUNCPTR *_ptrc_glGetUniformiv)(GLuint program, GLint location, GLint * params);
#define glGetUniformiv _ptrc_glGetUniformiv
extern void (CODEGEN_FUNCPTR *_ptrc_glGetVertexAttribPointerv)(GLuint index, GLenum pname, void ** pointer);
#define glGetVertexAttribPointerv _ptrc_glGetVertexAttribPointerv
extern void (CODEGEN_FUNCPTR *_ptrc_glGetVertexAttribfv)(GLuint index, GLenum pname, GLfloat * params);
#define glGetVertexAttribfv _ptrc_glGetVertexAttribfv
extern void (CODEGEN_FUNCPTR *_ptrc_glGetVertexAttribiv)(GLuint index, GLenum pname, GLint * params);
#define glGetVertexAttribiv _ptrc_glGetVertexAttribiv
extern void (CODEGEN_FUNCPTR *_ptrc_glHint)(GLenum target, GLenum mode);
#define glHint _ptrc_glHint
extern GLboolean (CODEGEN_FUNCPTR *_ptrc_glIsBuffer)(GLuint buffer);
#define glIsBuffer _ptrc_glIsBuffer
extern GLboolean (CODEGEN_FUNCPTR *_ptrc_glIsEnabled)(GLenum cap);
#define glIsEnabled _ptrc_glIsEnabled
extern GLboolean (CODEGEN_FUNCPTR *_ptrc_glIsFramebuffer)(GLuint framebuffer);
#define glIsFramebuffer _ptrc_glIsFramebuffer
extern GLboolean (CODEGEN_FUNCPTR *_ptrc_glIsProgram)(GLuint program);
#define glIsProgram _ptrc_glIsProgram
extern GLboolean (CODEGEN_FUNCPTR *_ptrc_glIsRenderbuffer)(GLuint renderbuffer);
#define glIsRenderbuffer _ptrc_glIsRenderbuffer
extern GLboolean (CODEGEN_FUNCPTR *_ptrc_glIsShader)(GLuint shader);
#define glIsShader _ptrc_glIsShader
extern GLboolean (CODEGEN_FUNCPTR *_ptrc_glIsTexture)(GLuint texture);
#define glIsTexture _ptrc_glIsTexture
extern void (CODEGEN_FUNCPTR *_ptrc_glLineWidth)(GLfloat width);
#define glLineWidth _ptrc_glLineWidth
extern void (CODEGEN_FUNCPTR *_ptrc_glLinkProgram)(GLuint program);
#define glLinkProgram _ptrc_glLinkProgram
extern void (CODEGEN_FUNCPTR *_ptrc_glPixelStorei)(GLenum pname, GLint param);
#define glPixelStorei _ptrc_glPixelStorei
extern void (CODEGEN_FUNCPTR *_ptrc_glPolygonOffset)(GLfloat factor, GLfloat units);
#define glPolygonOffset _ptrc_glPolygonOffset
extern void (CODEGEN_FUNCPTR *_ptrc_glReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void * pixels);
#define glReadPixels _ptrc_glReadPixels
extern void (CODEGEN_FUNCPTR *_ptrc_glReleaseShaderCompiler)(void);
#define glReleaseShaderCompiler _ptrc_glReleaseShaderCompiler
extern void (CODEGEN_FUNCPTR *_ptrc_glRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
#define glRenderbufferStorage _ptrc_glRenderbufferStorage
extern void (CODEGEN_FUNCPTR *_ptrc_glSampleCoverage)(GLfloat value, GLboolean invert);
#define glSampleCoverage _ptrc_glSampleCoverage
extern void (CODEGEN_FUNCPTR *_ptrc_glScissor)(GLint x, GLint y, GLsizei width, GLsizei height);
#define glScissor _ptrc_glScissor
extern void (CODEGEN_FUNCPTR *_ptrc_glShaderBinary)(GLsizei count, const GLuint * shaders, GLenum binaryformat, const void * binary, GLsizei length);
#define glShaderBinary _ptrc_glShaderBinary
extern void (CODEGEN_FUNCPTR *_ptrc_glShaderSource)(GLuint shader, GLsizei count, const GLchar *const* string, const GLint * length);
#define glShaderSource _ptrc_glShaderSource
extern void (CODEGEN_FUNCPTR *_ptrc_glStencilFunc)(GLenum func, GLint ref, GLuint mask);
#define glStencilFunc _ptrc_glStencilFunc
extern void (CODEGEN_FUNCPTR *_ptrc_glStencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint mask);
#define glStencilFuncSeparate _ptrc_glStencilFuncSeparate
extern void (CODEGEN_FUNCPTR *_ptrc_glStencilMask)(GLuint mask);
#define glStencilMask _ptrc_glStencilMask
extern void (CODEGEN_FUNCPTR *_ptrc_glStencilMaskSeparate)(GLenum face, GLuint mask);
#define glStencilMaskSeparate _ptrc_glStencilMaskSeparate
extern void (CODEGEN_FUNCPTR *_ptrc_glStencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
#define glStencilOp _ptrc_glStencilOp
extern void (CODEGEN_FUNCPTR *_ptrc_glStencilOpSeparate)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
#define glStencilOpSeparate _ptrc_glStencilOpSeparate
extern void (CODEGEN_FUNCPTR *_ptrc_glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * pixels);
#define glTexImage2D _ptrc_glTexImage2D
extern void (CODEGEN_FUNCPTR *_ptrc_glTexParameterf)(GLenum target, GLenum pname, GLfloat param);
#define glTexParameterf _ptrc_glTexParameterf
extern void (CODEGEN_FUNCPTR *_ptrc_glTexParameterfv)(GLenum target, GLenum pname, const GLfloat * params);
#define glTexParameterfv _ptrc_glTexParameterfv
extern void (CODEGEN_FUNCPTR *_ptrc_glTexParameteri)(GLenum target, GLenum pname, GLint param);
#define glTexParameteri _ptrc_glTexParameteri
extern void (CODEGEN_FUNCPTR *_ptrc_glTexParameteriv)(GLenum target, GLenum pname, const GLint * params);
#define glTexParameteriv _ptrc_glTexParameteriv
extern void (CODEGEN_FUNCPTR *_ptrc_glTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels);
#define glTexSubImage2D _ptrc_glTexSubImage2D
extern void (CODEGEN_FUNCPTR *_ptrc_glUniform1f)(GLint location, GLfloat v0);
#define glUniform1f _ptrc_glUniform1f
extern void (CODEGEN_FUNCPTR *_ptrc_glUniform1fv)(GLint location, GLsizei count, const GLfloat * value);
#define glUniform1fv _ptrc_glUniform1fv
extern void (CODEGEN_FUNCPTR *_ptrc_glUniform1i)(GLint location, GLint v0);
#define glUniform1i _ptrc_glUniform1i
extern void (CODEGEN_FUNCPTR *_ptrc_glUniform1iv)(GLint location, GLsizei count, const GLint * value);
#define glUniform1iv _ptrc_glUniform1iv
extern void (CODEGEN_FUNCPTR *_ptrc_glUniform2f)(GLint location, GLfloat v0, GLfloat v1);
#define glUniform2f _ptrc_glUniform2f
extern void (CODEGEN_FUNCPTR *_ptrc_glUniform2fv)(GLint location, GLsizei count, const GLfloat * value);
#define glUniform2fv _ptrc_glUniform2fv
extern void (CODEGEN_FUNCPTR *_ptrc_glUniform2i)(GLint location, GLint v0, GLint v1);
#define glUniform2i _ptrc_glUniform2i
extern void (CODEGEN_FUNCPTR *_ptrc_glUniform2iv)(GLint location, GLsizei count, const GLint * value);
#define glUniform2iv _ptrc_glUniform2iv
extern void (CODEGEN_FUNCPTR *_ptrc_glUniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
#define glUniform3f _ptrc_glUniform3f
extern void (CODEGEN_FUNCPTR *_ptrc_glUniform3fv)(GLint location, GLsizei count, const GLfloat * value);
#define glUniform3fv _ptrc_glUniform3fv
extern void (CODEGEN_FUNCPTR *_ptrc_glUniform3i)(GLint location, GLint v0, GLint v1, GLint v2);
#define glUniform3i _ptrc_glUniform3i
extern void (CODEGEN_FUNCPTR *_ptrc_glUniform3iv)(GLint location, GLsizei count, const GLint * value);
#define glUniform3iv _ptrc_glUniform3iv
extern void (CODEGEN_FUNCPTR *_ptrc_glUniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
#define glUniform4f _ptrc_glUniform4f
extern void (CODEGEN_FUNCPTR *_ptrc_glUniform4fv)(GLint location, GLsizei count, const GLfloat * value);
#define glUniform4fv _ptrc_glUniform4fv
extern void (CODEGEN_FUNCPTR *_ptrc_glUniform4i)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
#define glUniform4i _ptrc_glUniform4i
extern void (CODEGEN_FUNCPTR *_ptrc_glUniform4iv)(GLint location, GLsizei count, const GLint * value);
#define glUniform4iv _ptrc_glUniform4iv
extern void (CODEGEN_FUNCPTR *_ptrc_glUniformMatrix2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
#define glUniformMatrix2fv _ptrc_glUniformMatrix2fv
extern void (CODEGEN_FUNCPTR *_ptrc_glUniformMatrix3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
#define glUniformMatrix3fv _ptrc_glUniformMatrix3fv
extern void (CODEGEN_FUNCPTR *_ptrc_glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
#define glUniformMatrix4fv _ptrc_glUniformMatrix4fv
extern void (CODEGEN_FUNCPTR *_ptrc_glUseProgram)(GLuint program);
#define glUseProgram _ptrc_glUseProgram
extern void (CODEGEN_FUNCPTR *_ptrc_glValidateProgram)(GLuint program);
#define glValidateProgram _ptrc_glValidateProgram
extern void (CODEGEN_FUNCPTR *_ptrc_glVertexAttrib1f)(GLuint index, GLfloat x);
#define glVertexAttrib1f _ptrc_glVertexAttrib1f
extern void (CODEGEN_FUNCPTR *_ptrc_glVertexAttrib1fv)(GLuint index, const GLfloat * v);
#define glVertexAttrib1fv _ptrc_glVertexAttrib1fv
extern void (CODEGEN_FUNCPTR *_ptrc_glVertexAttrib2f)(GLuint index, GLfloat x, GLfloat y);
#define glVertexAttrib2f _ptrc_glVertexAttrib2f
extern void (CODEGEN_FUNCPTR *_ptrc_glVertexAttrib2fv)(GLuint index, const GLfloat * v);
#define glVertexAttrib2fv _ptrc_glVertexAttrib2fv
extern void (CODEGEN_FUNCPTR *_ptrc_glVertexAttrib3f)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
#define glVertexAttrib3f _ptrc_glVertexAttrib3f
extern void (CODEGEN_FUNCPTR *_ptrc_glVertexAttrib3fv)(GLuint index, const GLfloat * v);
#define glVertexAttrib3fv _ptrc_glVertexAttrib3fv
extern void (CODEGEN_FUNCPTR *_ptrc_glVertexAttrib4f)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
#define glVertexAttrib4f _ptrc_glVertexAttrib4f
extern void (CODEGEN_FUNCPTR *_ptrc_glVertexAttrib4fv)(GLuint index, const GLfloat * v);
#define glVertexAttrib4fv _ptrc_glVertexAttrib4fv
extern void (CODEGEN_FUNCPTR *_ptrc_glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer);
#define glVertexAttribPointer _ptrc_glVertexAttribPointer
extern void (CODEGEN_FUNCPTR *_ptrc_glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);
#define glViewport _ptrc_glViewport

enum ogl_LoadStatus
{
    ogl_LOAD_FAILED = 0,
    ogl_LOAD_SUCCEEDED = 1,
};

int ogl_LoadFunctions();


#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*POINTER_C_GENERATED_HEADER_OPENGLES_H*/
