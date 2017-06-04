#define LP3_GFX_API_CREATE
#include <lp3/gfx/management.hpp>
#include <lp3/gl.hpp>

namespace lp3 { namespace gfx {

LP3_GFX_API
Window::Window(gsl::not_null<gsl::czstring<>> title, const glm::ivec2 & size)
:	_display_resolution(size),
	_virtual_resolution(size)
{
	this->_window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		size.x,
		size.y,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	this->window_id = SDL_GetWindowID(_window);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetSwapInterval(0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	this->gl_context = SDL_GL_CreateContext(_window);
	if (SDL_GL_MakeCurrent(this->_window, this->gl_context) < 0) {
		LP3_LOG_ERROR("Error creating GL %s", SDL_GetError());
	}
	lp3::gl::initialize();
}

LP3_GFX_API
void Window::handle_events(const SDL_WindowEvent & window_event) {
	if (window_event.windowID != this->window_id) {
		return;
	}
	switch (window_event.event) {
		case SDL_WINDOWEVENT_SIZE_CHANGED:
		{
			const auto width = window_event.data1;
			const auto height = window_event.data2;
			this->new_size = glm::ivec2{ width, height };
			break;
		}
		default:
			break;
	}
}

LP3_GFX_API
void Window::render(SceneNodeFunc f) {
	LP3_GL_ASSERT_NO_ERRORS();
	if (this->new_size) {
		glViewport(0, 0, this->new_size->x, this->new_size->y);
		this->_display_resolution = *this->new_size;
		this->new_size = boost::none;
	}
	LP3_GL_ASSERT_NO_ERRORS();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	LP3_GL_ASSERT_NO_ERRORS();
	glClear(GL_COLOR_BUFFER_BIT);
	LP3_GL_ASSERT_NO_ERRORS();
	f(glm::mat4());

	SDL_GL_SwapWindow(this->_window);
}

LP3_GFX_API
void Window::work_with() {
	LP3_GL_ASSERT_NO_ERRORS();
	if (SDL_GL_MakeCurrent(this->_window, this->gl_context) < 0) {
		LP3_LOG_ERROR("Error switching graphics contexts %s", SDL_GetError());
	}
}


} }
