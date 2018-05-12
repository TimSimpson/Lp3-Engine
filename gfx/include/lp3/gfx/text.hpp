#ifndef FILE_LP3_GFX_TEXT_HPP
#define FILE_LP3_GFX_TEXT_HPP

#include "config.hpp"
#include <glm/vec4.hpp>
#include <lp3/gl.hpp>
#include <lp3/sdl.hpp>
#include "management.hpp"
#include "elements.hpp"

namespace lp3 { namespace gfx {

// todo: this needs a better doc
// ~end-doc summary

// ---------------------------------------------------------------------------
// struct Glyph
// ---------------------------------------------------------------------------
//     Represents the texture coordinates, size and advance (how many pixels
//     to pad between the next letter) of a single letter from a variable
//     width font.
// ---------------------------------------------------------------------------
LP3_GFX_API
struct Glyph {
    // Normalized texture cordinates, upper left corner.
    GLfloat x;
    GLfloat y;
    // Normalized texture cordinates, lower right corner.
    GLfloat x2;
    GLfloat y2;
    // Width (in pixels, not normalized) of font
    GLfloat width;
    // Width (in pixels, not normalized) to advance
    GLfloat advance;

    // Upper left texture coordinates
    glm::vec2 ul() const {
        return glm::vec2{x, y};
    }

    // Bottom right texture coordinates
    glm::vec2 br() const {
        return glm::vec2{x2, y2};
    }
};



// ---------------------------------------------------------------------------
// class Font
// ---------------------------------------------------------------------------
//     Represents a texture and the associated variable width font data.
// ---------------------------------------------------------------------------
LP3_GFX_API
class Font {
public:
    // Load data from the given RWops.
    Font(sdl::RWops & file);

    Font(sdl::RWops && file) : Font(file) {
    }

	inline GLfloat font_height() const {
		return _font_height;
	}

    // Returns glyph info for the given letter.
	inline const Glyph & get_glyph(const char letter) const {
		const int index = letter - first_letter;
		return glyphs[index];
	}

	inline std::size_t glyph_count() const {
		return glyphs.size();
	}

	inline Texture & texture() {
		return font_image;
	}

	inline const Texture & texture() const {
		return font_image;
	}

private:
    GLfloat _font_height;        // Height of the given font.
	std::uint8_t first_letter;   // First available letter
	std::uint8_t last_letter;    // Last available letter
    std::vector<Glyph> glyphs;   // All available glyphs
    Texture font_image;          // Image containing letters.
};


// ---------------------------------------------------------------------------
// write_letter
// ---------------------------------------------------------------------------
//     Given a font, and a character, modifies a quad (which is expected to
//     already point to the appropriate texture) to show the character.
// ---------------------------------------------------------------------------
template<typename Quad>
GLfloat write_letter(Quad && quad, const Font & font,
                      const glm::vec2 position, const GLfloat z,
                      const char letter) {
    const Glyph & g = font.get_glyph(letter);
    glm::vec2 size{g.width, font.font_height()};
	upright_quad(quad, position, position + size, z, g.ul(), g.br());
    return g.advance;
}

// ---------------------------------------------------------------------------
// write_string
// ---------------------------------------------------------------------------
//     Given an array of quads to modify, a font, a starting position and z
//     coordinate, changes each quad to a letter in the array. Tries to word
//     wrap at the given ``width`` argument.
// ---------------------------------------------------------------------------
template<typename QuadArray>
std::size_t write_string(QuadArray && quads, const Font & font, glm::vec2 pos,
                         GLfloat z, GLfloat width,
                         const gsl::cstring_span<> & letters) {
    (void)width; // TODO: Use parameter
    glm::vec2 p = pos;
	int i = 0;
	for (const auto & letter : letters) {
        if ('\n' == letter) {
            p.x = pos.x;
            p.y += (font.font_height());
        } else {
            p.x += write_letter(quads[i++], font, p, z, letter);
        }
    }
	return letters.size();
}
// ~end-doc

} }

#endif
