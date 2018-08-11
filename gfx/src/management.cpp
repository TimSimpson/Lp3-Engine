#define LP3_GFX_API_CREATE
#include <lp3/gfx/management.hpp>
#include <lp3/gl.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace lp3 { namespace gfx {

glm::ivec2 fudge_real_size(const glm::ivec2 & size) {
	#ifdef LP3_COMPILE_TARGET_POCKETCHIP
		glm::ivec2 new_size = size;
		if (size.x > 480) {
			new_size.x = 480;
		}
		if (size.y > 272) {
			new_size.y = 272;
		}
		return new_size;
	#else
		return size;
	#endif
}

LP3_GFX_API
Window::Window(gsl::czstring<> title, const glm::ivec2 & size)
:	gl_context(),
	new_size(boost::none),
	_virtual_resolution(size),
	_window(SDL_CreateWindow(
		gsl::not_null(title),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		size.x,
		size.y,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	)),
	window_id(SDL_GetWindowID(_window)),
	display(calculate_display_properties(size))
{	
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
Window::DisplayProperties Window::calculate_display_properties(
	const glm::ivec2 & new_display_resolution)
{
	const glm::vec2 vr = this->_virtual_resolution;
	const glm::vec2 dr = new_display_resolution;

	const GLfloat desired_ar = vr.x / vr.y;
	const GLfloat actual_ar = dr.x / dr.y;
	const GLfloat x_stretch = desired_ar / actual_ar;

	const glm::vec2 stretch =
		x_stretch <= 1.0f
		? glm::vec2{ x_stretch, 1.0f }
	: glm::vec2{ 1.0f, 1.0f / x_stretch };

	const glm::vec2 scissor_size = dr * stretch;

	const glm::vec2 scissor_start = (dr - scissor_size) / 2.0f;

	const glm::mat4 scale = glm::scale(
		glm::mat4(),
		glm::vec3(stretch.x, stretch.y, 1.0f));

	DisplayProperties props;
	// TODO: Check casts
	props.resolution = new_display_resolution;
	props.scale = scale;
	props.scissor_start = scissor_start;
	props.scissor_size = scissor_size;
	return props;
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
		this->display = calculate_display_properties(*this->new_size);
		this->new_size = boost::none;		
	}
	LP3_GL_ASSERT_NO_ERRORS();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	LP3_GL_ASSERT_NO_ERRORS();
	glClear(GL_COLOR_BUFFER_BIT);
	LP3_GL_ASSERT_NO_ERRORS();

	glEnable(GL_SCISSOR_TEST);
	glScissor(display.scissor_start.x, display.scissor_start.y,
		      display.scissor_size.x, display.scissor_size.y);

	// Finally, call whatever will do the rendering
	f(display.scale);
	glDisable(GL_SCISSOR_TEST);

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
