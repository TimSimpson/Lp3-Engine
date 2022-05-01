#include <lp3/core.hpp>
#include <lp3/gfx.hpp>
#include <lp3/main.hpp>
#include <lp3/sdl.hpp>
#include <lp3/sims.hpp>

namespace core = lp3::core;
namespace gl = lp3::gl;
namespace gfx = lp3::gfx;
namespace sdl = lp3::sdl;
namespace sims = lp3::sims;

int _main(core::PlatformLoop & loop) {
    sdl::SDL2 sdl2(SDL_INIT_VIDEO);
    core::LogSystem log;
    core::MediaManager media;

    // Creates SDL window and GL Context and calls lp3::gl::initialize()
    gfx::Window window("Construction Paper", glm::vec2{ 640, 480 });

	glEnable(GL_DEPTH_TEST);

	// Loads up EARTH
	auto earth_file = media.load("Engine/Gfx/Earth512.bmp");
	gfx::Texture earth(IMG_Load_RW(earth_file, 0));

	gfx::ElementWriter<gfx::TexCVert> earth_elements{ 4 };
    
    glm::vec4 color{0.0f, 1.0f, 0.0f, 1.0f};

	gfx::upright_quad(
		earth_elements.add_quad(),
		glm::vec2(0.0f, 0.0f), glm::vec2(32.0, 32.0), 0.9f,
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
        color);


	// Now draw a bunch of pizza
	auto pizza_file = media.load("Engine/Gfx/pizza.bmp");
    glm::ivec3 color_key[] = { glm::ivec3{ 255, 0, 220 }};
	gfx::Texture pizza(
        //IMG_LoadTyped_RW(bmp_file, 0, "BMP"),
        IMG_Load_RW(pizza_file, 0),
        gsl::make_span(color_key, 1));

	gfx::programs::ColoredTexture earth_program;

    gfx::programs::SimpleTextured pizza_program;

	const std::size_t number_of_quads = 1000; //00;

	gfx::ElementWriter<gfx::TexVert>
		pizza_elements{ (number_of_quads + 100) * 4 };



    // Now make our construction paper with a resolution of 64 by 64.
	const glm::ivec2 res2d(32, 32);

    // Make the render list. This will store what to render and do it later
    const GLfloat z = -0.0f;

	gfx::upright_quad(
		pizza_elements.add_quad(),
		glm::vec2(16.0f, 16.0f), glm::vec2(32.0, 32.0), z,
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 1.0f));
    for (int i = 0; i < 32; ++i) {
		gfx::upright_triangle(
			pizza_elements.add_triangle(),
			glm::vec2(1.0f * i, 1.0f * i),
			glm::vec2((1.0f * i) + 1.0f, (1.0f * i) + 1.0f),
			z,
			glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
    }

    std::vector<gfx::Quad<gfx::TexVert>> quads;
    for (std::size_t j = 0; j < number_of_quads; ++ j) {
        auto v = glm::vec2(0.1f * (j % 320), 0.1f * (j / 320));

		auto q = pizza_elements.add_quad();
		gfx::upright_quad(
			q,
			v, v + glm::vec2(1.0f, 1.0f), z,
			glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));

        quads.push_back(q);
    }

	auto triangle = pizza_elements.add_triangle();
	gfx::upright_triangle(
		triangle,
        glm::vec2(0.0f, 0.0f), glm::vec2(32.0f, 32.0f), z,
        glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));

    sims::FrameTimer frame_timer;

	auto drawer = [&](const glm::mat4 & previous) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto _2d = gfx::create_2d_screen(previous, res2d);

        pizza_program.use();
		pizza_program.set_mvp(_2d);
		pizza_program.set_texture(pizza.gl_id());
		pizza_program.draw(pizza_elements);

		// Render the Earth after the pizzas - this is done intentionally to
		// expose any bugs that may exist with the depth buffer or alpha
		// channels.
        earth_program.use();
        earth_program.set_mvp(_2d);
		earth_program.set_texture(earth.gl_id());
		earth_program.draw(earth_elements);
	};

    return loop.run([&]() {
        bool quit = false;
        SDL_Event e;
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



        triangle.v1().x() += 0.01f;
        triangle.v2().x() += 0.01f;
        triangle.v3().x() += 0.01f;

        LP3_LOG_INFO("Triangle ! %d, %d", triangle.v1().x(), triangle.v1().y());
        for (auto & quad : quads) {
            glm::vec2 v = quad.ul().pos_to_vec2();
            v.y -= 0.01f;
            if (v.y < -1.0f) {
                v.y = 32.0f;
            }
            upright_quad(quad, v, v + glm::vec2{1.0f, 1.0f}, z,
                         glm::vec2{0.0f, 0.0f}, glm::vec2{1.0f, 1.0f});
        }

        window.render(drawer);

        LP3_LOG_DEBUG("FPS: %d\tAvg FPS: %d",
                      frame_timer.get_fps(),
                      frame_timer.get_average_fps());
        frame_timer.next_frame();

        return !quit;
    });
}

LP3_MAIN(_main)
