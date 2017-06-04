#ifndef FILE_LP3_GFX_CPAPER_HPP
#define FILE_LP3_GFX_CPAPER_HPP

#include "config.hpp"
#include <lp3/gl.hpp>
#include "management.hpp"

namespace lp3 { namespace gfx {

// Establishes a 2D screen consisting of solid colors only.
// The screen can be moved around using the matrix arg like a normal scene node.
LP3_GFX_API
class CPaper {
public:
    class RendererList {
		friend class CPaper;
    public:
		RendererList();

        // Draw a triangle whose upper left corner is a and bottom left corner
        // is b.
        void triangle(glm::vec4 color, glm::vec2 a, glm::vec2 b, GLfloat z);
    private:
		void render(const CPaper & cpaper) const;
		std::vector<glm::vec4> colors;
        std::vector<GLfloat> vertices;
		gl::BufferObjectName vertex_pos_object;
    };

    CPaper(const glm::ivec2 resolution);

    // Draws the screen
    void operator()(const glm::mat4 & previous) const;

    void set_render_list(const RendererList & list);

	// Index of the position vertex attribute.
	const gl::VertexAttributeIndex a_position{ 0 };

	// Index of the color attribute.
	const gl::VertexAttributeIndex a_color{ 1 };
private:
    glm::ivec2 resolution;
    gl::ShaderOwner vertex_shader;
    gl::ShaderOwner fragment_shader;
    gl::ProgramOwner program_object;
    RendererList list;
};

} }

#endif
