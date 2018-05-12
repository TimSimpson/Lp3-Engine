#ifndef FILE_LP3_GFX_SCREEN_HPP
#define FILE_LP3_GFX_SCREEN_HPP

#include "config.hpp"
#include <lp3/gl.hpp>
#include "management.hpp"

namespace lp3 { namespace gfx {

LP3_GFX_API
class Screen;

using ScreenObject = std::function<void(const Screen & screen)>;

// TBH, this seems like a bad idea which is why it isn't in the docs.
// I think I was trying to make a bridge from the old v1. Engine's "screen"
// class. That engine was really aiming for Dreamcast compatability and had
// to work with fixed function style rendering, and had a notion of a "screen"
// which you'd give an objec to work with. But in hindsight that wasn't even
// a very nice abstraction. I think before too long this will go away too.
LP3_GFX_API
class Screen {
public:
	Screen(const glm::ivec2 resolution);

	// Draws the screen
	void operator()(const glm::mat4 & previous) const;

	void set_object(ScreenObject obj);

	// Index of the position vertex attribute.
	const gl::VertexAttributeIndex a_position{ 0 };

	// Index of the color attribute.
	const gl::VertexAttributeIndex a_color{ 1 };
private:
	glm::ivec2 resolution;
	gl::ShaderOwner vertex_shader;
	gl::ShaderOwner fragment_shader;
	gl::ProgramOwner program_object;
	ScreenObject object;
};

} }

#endif
