#define LP3_GFX_API_CREATE
#include <lp3/gfx/tiles.hpp>

#include <algorithm>

namespace lp3 { namespace gfx {

LP3_GFX_API
TileMap::TileMap(const glm::ivec2 & tile_size, const glm::ivec2 & map_size)
:   tile_size(tile_size),
    map_size(map_size),
	tiles(map_size.x * map_size.y, 0)
{
}

LP3_GFX_API

TileMap::~TileMap() {
}

LP3_GFX_API
void TileMap::fill(const glm::ivec2 & from, const glm::ivec2 & size,
                    std::uint8_t with) {
    LP3_ASSERT(0 <= size.x);
    LP3_ASSERT(0 <= from.x);
    LP3_ASSERT(from.x < map_size.x);
    LP3_ASSERT(from.x + size.x <= map_size.x);
    LP3_ASSERT(0 <= size.y);
    LP3_ASSERT(0 <= from.y);
    LP3_ASSERT(from.y < map_size.y);
    LP3_ASSERT(from.y + size.x <= map_size.y);

    if (size.x == map_size.x) {
        LP3_ASSERT(from.x == 0);
        std::fill(tiles.begin() + (from.y * map_size.x),
                  tiles.begin() + ((from.y + size.y) * map_size.x),
                  with);
    } else {
        for (int y = from.y; y < size.y; ++y) {
			const auto start = tiles.begin() + (y * map_size.x);
            std::fill(start, start + size.x, with);
        }
    }
}

LP3_GFX_API
std::size_t TileMap::tile_count() const {
    return map_size.x * map_size.y;
}

LP3_GFX_API
void TileMap::write(const glm::ivec2 & pos, const std::uint8_t text) {
	LP3_ASSERT(0 <= pos.x);
	LP3_ASSERT(pos.x < map_size.x);
	LP3_ASSERT(0 <= pos.y);
	LP3_ASSERT(pos.y < map_size.y);
	tiles[(pos.y * map_size.x) + pos.x] = text;
}

LP3_GFX_API
void TileMap::write(const glm::ivec2 & pos, 
	                const gsl::span<const std::uint8_t> text,
                    const bool word_wrap,
					const int index_adjust) {
    LP3_ASSERT(0 <= pos.x);
    LP3_ASSERT(pos.x < map_size.x);
    LP3_ASSERT(0 <= pos.y);
    LP3_ASSERT(pos.y < map_size.y);


    glm::ivec2 itr = pos;
    for (auto i = 0; i < text.size(); ++ i) {
        if (itr.x >= map_size.x) {
            if (!word_wrap) {
                return;
            }
            itr.x = pos.x;
            ++ itr.y;
            if (itr.y >= map_size.y) {
                return;
            }
        }
		tiles[(itr.y * map_size.x) + itr.x] = text[i] + index_adjust;
		++ itr.x;
    }
}

void TileMap::write_box(
	const BoxTiles & bt, const glm::ivec2 & pos, const glm::ivec2 & size)
{
	// Create border around stuff.
	const glm::ivec2 pos2 = pos + (size - glm::ivec2{ 1, 1 });
	write(pos, bt.ul);
	for (int x = pos.x + 1; x < pos2.x; ++x) {
		write({ x, pos.y }, bt.u);
	}
	write({ pos2.x, pos.y }, bt.ur);

	for (int y = pos.y + 1; y < pos2.y; ++y) {
		write({ pos.x, y }, bt.l);
		write({ pos2.x, y }, bt.r);
	}

	write({ pos.x, pos2.y }, bt.bl);
	for (int x = pos.x + 1; x < pos2.x; ++x) {
		write({ x, pos2.y }, bt.b);
	}
	write({ pos2.x, pos2.y }, bt.br);
}

}   }
