#ifndef FILE_LP3_GFX_SCREEN_HPP
#define FILE_LP3_GFX_SCREEN_HPP

#include "config.hpp"
#include <lp3/gl.hpp>
#include "management.hpp"

namespace lp3 { namespace gfx {

LP3_GFX_API
class Screen;

using ScreenObject = std::function<void(const Screen & screen)>;

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
