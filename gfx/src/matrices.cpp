#define LP3_GFX_API_CREATE
#include <lp3/gfx/matrices.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace lp3 { namespace gfx {

LP3_GFX_API
glm::mat4 create_2d_screen(const glm::mat4 & previous,
                           const glm::ivec2 resolution) {
    // TODO: should I make positive Z near instead? Near is currently -1, not 1.
    // The smart correct way to do this would appear to be the following
    // commented out line, but that doesn't work.
    //glm::mat4 ortho = glm::ortho(0, resolution.x, resolution.y, 0, -1, 1);

    // Reminder: OpenGL's normal coordinate system is like this:

    // (-1.0,  1.0)  |   (1.0,  1.0)
    //               |
    // --------------|--------------
    //               |
    // (-1.0, -1.0)  |   (1.0, -1.0)

    // This turns it into a "TPaper" coordinate system which I've been used to
    // since time immemorial (width and height are the resolution):

    // (0.0, 0.0)      |   (width,  0.0)
    //                 |
    // ----------------|----------------
    //                 |
    // ( 0.0, height)  | (width, height)

    // The matrix has to map the screen cordinates we want *back* to OpenGL's
    // system. Step one is to scale the width and height of what we're doing to
    // essentially shrink it, and reverse the Y. Next we move the center to
    // the upper left corner.

    const glm::mat4 scale = glm::scale(
        previous, glm::vec3(2.0f/resolution.x, -2.0f/resolution.y, 1.0f));
    const glm::mat4 translate = glm::translate(
        scale, glm::vec3(-0.5f * resolution.x, -0.5f * resolution.y, 0.0f));
    glm::mat4 ortho = translate;
    return ortho;
}

}	}
