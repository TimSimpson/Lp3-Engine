#include <lp3/core.hpp>
#include <lp3/gfx.hpp>
#include <lp3/main.hpp>
#include <lp3/sdl.hpp>

namespace core = lp3::core;
namespace gl = lp3::gl;
namespace gfx = lp3::gfx;
namespace sdl = lp3::sdl;

int _main(core::PlatformLoop & loop) {
	sdl::SDL2 sdl2(SDL_INIT_VIDEO);
	core::LogSystem log;
	core::MediaManager media;

	// Creates SDL window and GL Context and calls lp3::gl::initialize()
	gfx::Window window("Super Simple", glm::ivec2{640, 480});

	const auto bmp_file = media.path("Engine/Gfx/Earth.bmp");

	gfx::Texture bitmap(SDL_LoadBMP(bmp_file.c_str()));

	gfx::Screen screen(glm::ivec2{ 64, 64 });


	gl::BufferObjectName vertex_pos_object = gl::gen_buffer();

	// GLfloat l = -1.0f; //0.0f;
	// GLfloat m = 0.0f; // 50.0f;
	// GLfloat f = 1.0f; //100.0f;
	// GLfloat z = -0.0f;

	//GLfloat l = 0.0f; //0.0f;
	//GLfloat m = 50.0f; // 50.0f;
	//GLfloat f = 90.0f; //100.0f;
	GLfloat z = -0.0f;


	//    B    //
	//  /   \  //
	// A --- C //

	std::vector<GLfloat> vertices;
	struct color_holder {
		GLfloat data[4];
	};
	std::vector<color_holder> colors;
	auto draw_triangle = [&](glm::vec4 color, glm::vec2 a, glm::vec2 b) {
		color_holder ch;
		ch.data[0] = color[0];
		ch.data[1] = color[1];
		ch.data[2] = color[2];
		ch.data[3] = color[3];
		colors.push_back(ch);
		glm::vec2 size = b - a;
		glm::vec3 v1(a.x + (size.x * 0.5f), a.y, z);
		glm::vec3 v2(a.x,                   b.y, z);
		glm::vec3 v3(b.x,                   b.y, z);
		vertices.push_back(v1.x);
		vertices.push_back(v1.y);
		vertices.push_back(v1.z);
		vertices.push_back(v2.x);
		vertices.push_back(v2.y);
		vertices.push_back(v2.z);
		vertices.push_back(v3.x);
		vertices.push_back(v3.y);
		vertices.push_back(v3.z);
	};

	//GLfloat red[4] = { 1.0, 0.0f, 0.0f, 1.0f };
	//GLfloat yellow[4] = { 1.0, 1.0f, 0.0f, 1.0f };
//	GLfloat green[4] = { 0.0, 1.0f, 0.0f, 1.0f };
	//GLfloat blue[4] = { 0.0, 0.0f, 1.0f, 1.0f };


	draw_triangle(gfx::red, glm::vec2(-1.0f, -1.0f), glm::vec2(1.0, 1.0));
	draw_triangle(gfx::yellow, glm::vec2(0.0f, 0.0f), glm::vec2(1.0, 1.0));
	draw_triangle(gfx::green, glm::vec2(1.0f, 1.0f), glm::vec2(2.0, 2.0));

	for (int i = 1; i < 50; ++i) {
		draw_triangle(gfx::blue, glm::vec2(1.0f * i, 1.0f * i), glm::vec2((1.0f * i) + 1.0f, (1.0f * i) + 1.0f));
	}
	// GLfloat vertices[] = {
	// 	m,  l, z,
	// 	l, f, z,
	// 	f, m, z,

	// 	// m,  l, z,
	// 	// f, m, z,
	// 	// l, f, z

	// 	m, f, z,
	// 	l, l, z,
	// 	f, l, z
	// };
	// GLfloat vertices[] = {
	// 	960.0f,  0.0f, 0.0f,
	// 	0.0f, 1080.0f, 1.0f,
	// 	1920.0f, 1080.0f, 0.0f
	// };
	// GLfloat vertices[] = {
	// 	0.0f,  1.0f, 0.0f,
	// 	-1.0f, -1.0f, 1.0f,
	// 	1.0f, -1.0f, 0.0f
	// };
	gl::bind_buffer(GL_ARRAY_BUFFER, vertex_pos_object);
	gl::buffer_data(GL_ARRAY_BUFFER, gsl::span<const GLfloat>(vertices),//gsl::span<const GLfloat>(vertices),
		GL_STATIC_DRAW);

	screen.set_object([&](const gfx::Screen & _screen) {
		GLfloat yellow[4] = { 1.0, 1.0f, 0.0f, 1.0f };
		gl::vertex_attrib_4fv(_screen.a_color, yellow);
		gl::bind_buffer(GL_ARRAY_BUFFER, vertex_pos_object);
		gl::vertex_attrib_pointer(_screen.a_position, 3, GL_FLOAT, 0, 0, 0);

		gl::enable_vertext_attrib_array(_screen.a_position);
		gl::draw_arrays(GL_TRIANGLES, 0, 3);

		// GLfloat red[4] = { 1.0, 0.0f, 0.0f, 1.0f };
		for (std::size_t i = 0; i < colors.size(); ++ i) {
			GLfloat color[4];
			color[0] = colors[i].data[0];
			color[1] = colors[i].data[1];
			color[2] = colors[i].data[2];
			color[3] = colors[i].data[3];
			gl::vertex_attrib_4fv(_screen.a_color, color);
			gl::draw_arrays(GL_TRIANGLES, (i * 3), 3);
		}

		//glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
		//glBindTexture(GL_TEXTURE_2D, bitmap.gl_id());
	});
	return loop.run([&]() {
		SDL_Event e;
		bool quit = false;
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_WINDOWEVENT:
				window.handle_events(e.window);
				break;
			}
		}
		window.render(std::ref(screen));
		return !quit;
	});
}

LP3_MAIN(_main)
