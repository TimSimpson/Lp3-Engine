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


	// Loads up EARTH
	auto bmp_file = media.load("Engine/Gfx/pizza.bmp");

    glm::ivec3 color_key[] = { glm::ivec3{ 255, 0, 220 }};
	gfx::Texture bitmap(
        //IMG_LoadTyped_RW(bmp_file, 0, "BMP"),
        IMG_Load("Engine/Gfx/pizza.bmp"),
        gsl::make_span(color_key, 1));



    const std::size_t number_of_quads = 1000; //00;

    // Now make our construction paper with a resolution of 64 by 64.
    gfx::TPaper tpaper(glm::ivec2{ 32, 32 }, bitmap.gl_id(),
                       number_of_quads + 100);

    // Make the render list. This will store what to render and do it later
    const GLfloat z = -0.0f;

	gfx::upright_quad(
		tpaper.elements().add_quad(),
		glm::vec2(16.0f, 16.0f), glm::vec2(32.0, 32.0), z,
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 1.0f));
    for (int i = 0; i < 32; ++i) {
		gfx::upright_triangle(
			tpaper.elements().add_triangle(),
			glm::vec2(1.0f * i, 1.0f * i),
			glm::vec2((1.0f * i) + 1.0f, (1.0f * i) + 1.0f),
			z,
			glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
    }

    std::vector<gfx::Quad<gfx::TexVert>> quads;
    for (std::size_t j = 0; j < number_of_quads; ++ j) {
        auto v = glm::vec2(0.1f * (j % 320), 0.1f * (j / 320));

		auto q = tpaper.elements().add_quad();
		gfx::upright_quad(
			q,
			v, v + glm::vec2(1.0f, 1.0f), z,
			glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));

        quads.push_back(q);
    }

	auto triangle = tpaper.elements().add_triangle();
	gfx::upright_triangle(
		triangle,
        glm::vec2(0.0f, 0.0f), glm::vec2(32.0f, 32.0f), z,
        glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));

    sims::FrameTimer frame_timer;


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

        window.render(std::ref(tpaper));

        LP3_LOG_DEBUG("FPS: %d\tAvg FPS: %d",
                      frame_timer.get_fps(),
                      frame_timer.get_average_fps());
        frame_timer.next_frame();

        return !quit;
    });
}

LP3_MAIN(_main)
