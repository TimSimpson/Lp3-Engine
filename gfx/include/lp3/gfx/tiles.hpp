// ----------------------------------------------------------------------------
// Tile Maps
// =========
//     Tile maps are simple grids where each element is an index to a texture
//     containing a series of tiles.
// ----------------------------------------------------------------------------
// ~end-doc summary

#ifndef FILE_LP3_GFX_TILES_HPP
#define FILE_LP3_GFX_TILES_HPP

#include <cmath>
#include "config.hpp"
#include "buffers.hpp"

namespace lp3 { namespace gfx {

// ---------------------------------------------------------------------------
// struct BoxTiles
// ---------------------------------------------------------------------------
//     Represents a series of tile indices which can be used to create a
//     frame or border around text. For example, on the first row of the
//     border ``ul`` would appear once at the left, ``ur`` would appear once
//     at the right, and ``u`` would appear in the middle. For the bottom of
//     the border, it would be the same, only with ``bl``, ``b``, and ``br``,
//     and the left and right sides of the border would use ``l`` and ``r``.
// ---------------------------------------------------------------------------
struct
LP3_GFX_API
BoxTiles {
	std::uint8_t ul;
	std::uint8_t u;
	std::uint8_t ur;
	std::uint8_t l;
	std::uint8_t r;
	std::uint8_t bl;
	std::uint8_t b;
	std::uint8_t br;
};

// ---------------------------------------------------------------------------
// class TileMap
// ---------------------------------------------------------------------------
//     A grid of indices to a texture containing a series of images with the
//     same dimensions. When rendered, "tiles" from the texture will be
//     "mapped" to this so-called "TileMap." It's possible to also write out
//     text from fixed-width fonts if the letters can be found on the source
//     texture.
// ---------------------------------------------------------------------------
class
LP3_GFX_API
TileMap {
public:
    TileMap(const glm::ivec2 & tile_size, const glm::ivec2 & map_size);

    ~TileMap();

    template<typename Vertex>
    QuadArray<Vertex> create_quads(ElementWriter<Vertex> & writer) {
        return writer.add_quads(lp3::narrow<int>(this->tile_count()));
    }

    void fill(const glm::ivec2 & from, const glm::ivec2 & size,
              std::uint8_t with=0);

    glm::vec2 find_starting_texture_coordinates(
        glm::vec2 tile_resolution, const glm::vec2 texture_size,
        const std::uint8_t tile_index)
    {
		const float tiles_per_row = std::floor(texture_size.x / tile_resolution.x);

		const float ty = std::floor(tile_index / tiles_per_row);
		const float tx = tile_index - (ty * tiles_per_row);

		return glm::vec2{ tx * tile_resolution.x, ty * tile_resolution.y };
    }

    template<typename Vertex>
    void set_quads(glm::vec2 start_pos, float z, QuadArray<Vertex> & quads,
                   glm::vec2 tile_resolution, glm::vec2 texture_size) {
        LP3_ASSERT(this->tile_count() <= quads.size());
        glm::vec2 draw_pos = start_pos;
		const float max_width = map_size.x * tile_resolution.x;
        for (std::size_t i = 0; i < tile_count(); ++i) {
            const std::uint8_t c = tiles[i];
            glm::vec2 ta = find_starting_texture_coordinates(tile_resolution, texture_size, c);
            glm::vec2 tb = ta + tile_resolution;
            ta /= texture_size;
            tb /= texture_size;
            upright_quad(quads[i], draw_pos, draw_pos + tile_resolution,
                              z, ta, tb);
			draw_pos.x += tile_resolution.x;
			if (draw_pos.x >= max_width) {
				draw_pos.x = start_pos.x;
				draw_pos.y += tile_resolution.y;
			}
        }
    }

    std::size_t tile_count() const;

	void write(const glm::ivec2 & pos, const std::uint8_t text);

    void write(const glm::ivec2 & pos, const gsl::span<const std::uint8_t> text,
               const bool word_wrap=false, const int index_adjust=0);

    inline void write(const glm::ivec2 & pos, const char * text,
                      bool word_wrap = false) {
        auto span = gsl::make_span<const std::uint8_t>(
            reinterpret_cast<const std::uint8_t *>(text),
            strnlen(text, 1024));
        write(pos, span, word_wrap, -32);
    }

	void write_box(
		const BoxTiles & bt, const glm::ivec2 & pos, const glm::ivec2 & size);
private:
    const glm::ivec2 tile_size;
    const glm::ivec2 map_size;
    std::vector<std::uint8_t> tiles;
};
// ~end-doc

}   }   // end lp3::gfx

#endif
