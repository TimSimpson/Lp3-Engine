#ifndef FILE_LP3_GFX_MANAGEMENT_HPP
#define FILE_LP3_GFX_MANAGEMENT_HPP

#include "config.hpp"
#include <tuple>
#include <lp3/core.hpp>
#include <lp3/gl.hpp>
#include <lp3/sdl.hpp>

#include "config.hpp"
#include <glm/matrix.hpp>
#include <glm/vec2.hpp>


namespace lp3 { namespace gfx {

using SceneNodeFunc = std::function<void(const glm::mat4 & previous)>;

// Creates and manages a simple SDL Window that uses OpenGL.
// This can be thought of as a simple pairing of the SDL Window and GL context
// handles, as it manages them, combined with some helper functions to render
// and handle SDL events in typical ways (by resizing the screen for example).
LP3_GFX_API
class Window {
public:
	// Accepts the title of the window and the virtual resolution.
	Window(gsl::not_null<gsl::czstring<>> title, const glm::ivec2 & size);

	// How many pixels we're actually using. If this abstraction is successful
	// this will rarely be used.
	inline const glm::ivec2 & display_resolution() {
		return display.resolution;
	}

	void handle_events(const SDL_WindowEvent & window_event);

	void render(SceneNodeFunc f);

	inline lp3::sdl::Window & sdl_window() {
		return _window;
	}

	// How many pixels we're pretending to use. 
	inline const glm::ivec2 & virtual_resolution() {
		return _virtual_resolution;
	}

	// The scaling factor - the actual over the virtual resolution.
	inline glm::vec2 display_scale() {
		return glm::vec2(display.resolution) / glm::vec2(_virtual_resolution);
	}

	// Call this when you want to work with this OpenGL context outside of
	// rendering (for example when creating textures or programs).
	void work_with();
private:
	lp3::sdl::GLContext gl_context;
	boost::optional<glm::ivec2> new_size;
	glm::ivec2 _virtual_resolution;
	lp3::sdl::Window _window;
	Uint32 window_id;	
	
	struct DisplayProperties {
		glm::ivec2 resolution;
		glm::ivec2 scissor_size;
		glm::ivec2 scissor_start;
		glm::mat4 scale;
	};
	
	DisplayProperties calculate_display_properties(
		const glm::ivec2 & new_display_resolution);

	DisplayProperties display;
};

// Creates an OpenGL texture from an SDL texture and has the ability to
// restore it when necessary.
LP3_GFX_API
class Texture {
public:
	// Creates a new Texture from an SDL_Surface.
	// The passed surface will be used and deleted by this class.
	// The passes span of vectors will are expected to have their RGB values
	// set to colors which should become transparent.
	Texture(gsl::owner<SDL_Surface *> surface,
			const gsl::span<glm::ivec3> color_keys={});

	~Texture();

	// The handle usable by OpenGL.
	inline gl::TextureID gl_id() const {
		return texture;
	}

	// Recreates the OpenGL texture in video memory.
	// Can be useful in cases where the graphics device causes memory to be
	// wiped (like when changing screen modes).
	void restore();

	glm::vec2 size() const {
		return glm::vec2{surface->w, surface->h};
	}
private:
	gsl::owner<SDL_Surface *> surface;
	gl::TextureIDOwner texture;
};

} }

#endif
