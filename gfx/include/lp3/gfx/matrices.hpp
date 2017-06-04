#ifndef LP3_GFX_MATRICES_HPP
#define LP3_GFX_MATRICES_HPP
#pragma once

#include "config.hpp"
#include <glm/glm.hpp>

namespace lp3 { namespace gfx {

// Given the previous matrix, returns a new matrix which creates a 2D screen.
// Positive X aims right, positive Y aims down, and X and Y begin at 0,0 in
// the upper left corner.
LP3_GFX_API
glm::mat4 create_2d_screen(const glm::mat4 & previous,
                           const glm::ivec2 resolution);

}   }

#endif
