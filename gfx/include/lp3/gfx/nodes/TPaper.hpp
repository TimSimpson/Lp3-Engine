#ifndef FILE_LP3_GFX_TPAPER_HPP
#define FILE_LP3_GFX_TPAPER_HPP

#include "../config.hpp"
#include <lp3/gl.hpp>
#include "../buffers.hpp"
#include "../elements.hpp"
#include "../management.hpp"
#include "../programs/SimpleTextured.hpp"

namespace lp3 { namespace gfx {
// Establishes a 2D screen consisting of textured colors.
// The screen can be moved around using the matrix arg like a normal scene node.
LP3_GFX_API
class TPaper {
public:
    TPaper(const glm::ivec2 resolution, gl::TextureID texture,
           const std::size_t vertex_count);

    // Draws the screen
    void operator()(const glm::mat4 & previous) const;

	ElementWriter<TexVert> & elements() {
		return _elements;
	}

private:
    programs::SimpleTextured program;
    glm::ivec2 resolution;
	gl::TextureID texture;
	ElementWriter<TexVert> _elements;
};

} }

#endif
