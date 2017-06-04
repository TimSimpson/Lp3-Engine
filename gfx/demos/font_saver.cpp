#include <fstream>
#include <iostream>
#include <memory>
#include <boost/format.hpp>
#include <lp3/core.hpp>
#include <lp3/gl.hpp>
#include <lp3/sdl.hpp>
#include <lp3/log.hpp>
#include <lp3/main.hpp>
#if LP3_COMPILE_TARGET_EMSCRIPTEN
#include <SDL/SDL_ttf.h>
#else
#include <SDL_ttf.h>
#endif
#include <lp3/sims.hpp>
#include <lp3/gfx.hpp>

namespace core = lp3::core;
namespace sdl = lp3::sdl;
namespace gfx = lp3::gfx;
namespace sims = lp3::sims;


void save_file(sdl::RWops & input_file, int font_size, sdl::RWops & out_file) {

    Uint32 rmask, gmask, bmask, amask;

    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    #endif

    glm::ivec2 surface_size(256, 256);
    SDL_Surface * new_surface = SDL_CreateRGBSurface(
        0,  // flags, unusued
        surface_size.x,
        surface_size.y,
        32, // depth
        rmask, gmask, bmask, amask);
    LP3_ASSERT(new_surface);


    LP3_LOG_DEBUG("Loading font...")

    
	auto font = TTF_OpenFontRW(input_file, 0, font_size);
    if (nullptr == font) {
        LP3_LOG_ERROR(SDL_GetError());
        LP3_THROW2(core::Exception, "Error loading font!");
    }

    const GLfloat fh = lp3::narrow<GLfloat>(TTF_FontHeight(font));
    LP3_LOG_INFO("Font Height : %d", fh);

    std::vector<gfx::Glyph> glyphs;
    glyphs.reserve(int{'z'} - int{'a'} + 1);

    GLfloat x = 0;
    GLfloat y = 0;

    for (char letter = ' '; letter <= '~'; ++ letter) {
        const wchar_t ch = letter; //'A';
        int minx, maxx, miny, maxy, advance;
        const auto result = TTF_GlyphMetrics(font, letter, &minx, &maxx, &miny, 
			                                 &maxy, &advance);
		SDL_assert(0 == result);

        if (x + advance > surface_size.x) {
            x = 0;
            y += fh;
        }
        gfx::Glyph g;
        g.x = x + minx;
        g.y = y;
        g.width = lp3::narrow<float>(maxx);
        g.x2 = g.x + g.width;
        g.y2 = g.y + fh;
        g.advance = lp3::narrow<float>(advance);

        // ADVANCE
        x += advance; //(minx + advance) ; //= x + maxx + advance;
        glyphs.emplace_back(g);

        SDL_Rect src, dst;
        src.x = 0;
        src.y = 0;
        src.w = lp3::narrow<int>(g.advance);
        src.h = lp3::narrow<int>(fh);

        dst.x = lp3::narrow<int>(g.x);
        dst.y = lp3::narrow<int>(g.y);
        dst.w = lp3::narrow<int>(g.advance);
        dst.h = lp3::narrow<int>(fh);

        SDL_Color text_color = { 255, 0, 255, 255 };

        auto ch_str = str(boost::format("%s") % letter);
        auto text = TTF_RenderText_Solid(font, ch_str.c_str(), text_color);


        SDL_Surface * single_letter = SDL_CreateRGBSurface(
            0,  // flags, unusued
            64,
            64,
            32, // depth
            rmask, gmask, bmask, amask);
        LP3_ASSERT(single_letter);

        SDL_BlitSurface(text, nullptr, single_letter, NULL);

        const int result2 = SDL_BlitSurface(single_letter, &src, new_surface, &dst);
        LP3_ASSERT(0 == result);
    }

    {
		out_file.write("lp3_font", 8, 1);
        out_file.write("glyphs..", 8, 1);
                
        out_file.write(fh);
		out_file.write(' ');	// first letter
		out_file.write('~'); // last letter
        out_file.write(std::size_t{glyphs.size()});

        for(const gfx::Glyph & original_g : glyphs) {
			gfx::Glyph g;

            // Transform to OpenGL friendly normalized coordinates right before
            // saving.
            g.x = original_g.x / surface_size.x;
            g.y = original_g.y / surface_size.y;
            g.x2 = original_g.x2 / surface_size.x;
            g.y2 = original_g.y2 / surface_size.y;
            
			// These stay expressed in terms of pixels.
			g.advance = original_g.advance;
			g.width = original_g.width;

            out_file.write(g.x);
            out_file.write(g.y);
            out_file.write(g.x2);
            out_file.write(g.y2);
            out_file.write(g.width);
            out_file.write(g.advance);
            // LP3_LOG_INFO("glyph (%d, %d) -> %d", g.x, g.y, g.advance);
        }

        IMG_SavePNG_RW(new_surface, out_file, 0);		
    }
}

int load(core::PlatformLoop & loop,
         sdl::RWops & input_file) {    
    
    gfx::Window window("FONT TIME BABEE", glm::vec2{ 640, 480 });

	gfx::Font font{ input_file };

    LP3_LOG_INFO("glpyh_count = %i", font.glyph_count());
    LP3_ASSERT(font.glyph_count() < 256);
    const std::size_t number_of_quads = 100000;

    gfx::TPaper tpaper(glm::ivec2{640, 480 }, font.texture().gl_id(),
                       number_of_quads + 100);
	gfx::QuadArray<gfx::TexVert> quads1 = tpaper.elements().add_quads(26);
	
    const GLfloat z = -0.0f;
    
    GLfloat l = 10.0f;
    GLfloat y = 10.0f;
	int letter_i = 0;
    for (char letter = 'a'; letter <= 'z'; ++ letter) {
        l += gfx::write_letter(quads1[letter_i++], font, glm::vec2(l, y), z, letter);
        if ('o' == letter) {
            y += font.font_height();
            l = 10.0f;
        }
    }

    //auto write_string = [&](glm::vec2 pos, GLfloat z, const char * letters) {
    //    glm::vec2 p = pos;
    //    for (const char * itr = letters; *itr != '\0'; ++ itr) {
    //        if ('\n' == *itr) {
    //            p.x = pos.x;
    //            p.y += (font.font_height());
    //        } else {
				//p.x += gfx::render_letter(r, font, p, z, *itr);
    //        }
    //    }
    //};

	gfx::QuadArray<gfx::TexVert> quads = tpaper.elements().add_quads(80 * 20);

	GLfloat w = 400;

	std::size_t c;
    c = gfx::write_string(quads, font, glm::vec2(0, 160), z, w, "I IJ dreamed I saw a wacker wicky");

	c = gfx::write_string(quads.subset(c), font, glm::vec2(0, 195), z, w, "III   I I I ! !");

	c = gfx::write_string(quads.subset(c), font, glm::vec2(0, 230), z, w, "Do you want to eat more crackers? Sure we all do. []");

	c = gfx::write_string(quads.subset(c), font, glm::vec2(0, 255), z, w, "All aboard the doo-doo train!?!$#@#*&(*)&");

	c = gfx::write_string(quads.subset(c), font, glm::vec2(0, 280), z, w, "^_^  o_o  Wearily crossing the ippy ick road.");

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

        window.render(std::ref(tpaper));


		LP3_LOG_DEBUG("FPS: %d\tAvg FPS: %d",
                      frame_timer.get_fps(),
                      frame_timer.get_average_fps());
        frame_timer.next_frame();

        return !quit;
    });
}

int _main(core::PlatformLoop & loop) {
    sdl::SDL2 sdl2(SDL_INIT_VIDEO);
    TTF_Init();

    core::LogSystem log;

    LP3_LOG_DEBUG("Font Saver");

        if (loop.command_line_args().size() < 3) {
        std::cerr << "Usage: "
                  << (loop.command_line_args().size() > 1
                        ? loop.command_line_args()[0]
                        : "prog")
                  << " input-file output-file [run]\n";
        return 1;
    }

    const auto input_file_path = loop.command_line_args()[1];
    const auto output_file_path = loop.command_line_args()[2];

    const bool run = loop.command_line_args().size() > 3
                     && loop.command_line_args()[3] == "run";


    sdl::Window window = SDL_CreateWindow(
        "SDL2",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480,
        SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE
    );

    LP3_LOG_DEBUG("Creating renderer...");
    sdl::Renderer renderer
        = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


	const int font_size = 28;

	std::cout << "Loading <-- " << input_file_path << "\n";
	std::cout << "Saving  --> " << output_file_path << "\n";
	{
		auto i = SDL_RWFromFile(input_file_path.c_str(), "rb");
		if (!i) {
			std::cerr << "Error loading file.\n";
			return 1;
		}
		sdl::RWops input_file{ i };

		auto o = SDL_RWFromFile(output_file_path.c_str(), "wb");
		if (!o) {
			std::cerr << "Error creating save file.\n";
			return 1;
		}
		sdl::RWops out_file{ o };
		save_file(input_file, font_size, out_file);
	}

	// Reopen file.
	sdl::RWops out_file{ SDL_RWFromFile(output_file_path.c_str(), "rb") };
    return load(loop, out_file);
}

LP3_MAIN(_main)
