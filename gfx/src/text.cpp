#define LP3_GFX_API_CREATE
#include <lp3/gfx/text.hpp>

namespace lp3 { namespace gfx {

namespace sdl = lp3::sdl;

namespace {

    gsl::owner<SDL_Surface *> read_file(sdl::RWops & file,
		                                GLfloat & font_height,
										std::uint8_t & first_letter,
										std::uint8_t & last_letter,
										std::vector<Glyph> & glyphs) {
		char header[8];
		file.read(header, 8);
		LP3_ASSERT(0 == strncmp(header, "lp3_font", 8));
		file.read(header, 8);
		LP3_ASSERT(0 == strncmp(header, "glyphs..", 8));

		file.read(font_height);

		file.read(first_letter);
		file.read(last_letter);

        std::uint64_t glyph_count;
        file.read(glyph_count);
        glyphs.reserve(glyph_count);
        for(int i = 0; i < glyph_count; ++ i) {
            Glyph g;
            file.read(g.x);
            file.read(g.y);
            file.read(g.x2);
            file.read(g.y2);
            file.read(g.width);
            file.read(g.advance);
            glyphs.push_back(g);

            LP3_LOG_DEBUG("  %i (%i, %i)-(%i, %i) -> %i",
                          i, g.x, g.y, g.x2, g.y2, g.advance);
        }

        return IMG_LoadTyped_RW(file, 0, "PNG");
    }
}

LP3_GFX_API
Font::Font(lp3::sdl::RWops & file)
:   _font_height(0),
	first_letter(0),
	last_letter(0),
    glyphs(),
    font_image(read_file(file, _font_height, first_letter, last_letter, glyphs))
{
}






}   }
