#define LP3_GFX_API_CREATE
#include <lp3/gfx/management.hpp>
#include <lp3/gl.hpp>

namespace lp3 { namespace gfx {

LP3_GFX_API
Texture::Texture(gsl::owner<SDL_Surface *> surface, const gsl::span<glm::ivec3> color_keys)
:   surface(nullptr),
    texture()
{
    #ifdef LP3_COMPILE_WITH_DEBUGGING
        if (nullptr == surface) {
            LP3_LOG_ERROR("Error with texture: %s", SDL_GetError());
        }
    #endif
    LP3_ASSERT(nullptr != surface);

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

    SDL_Surface * new_surface = SDL_CreateRGBSurface(
        0,  // flags, unusued
        surface->w,
        surface->h,
        32, // depth
        rmask, gmask, bmask, amask);
    LP3_ASSERT(new_surface);

    int fr_result =
        SDL_FillRect(new_surface, nullptr,
                     SDL_MapRGBA(new_surface->format, 0, 0, 0, 0));
    LP3_ASSERT(0 == fr_result);

    // Set up color key stuff
    // Make the key pink
    for (const glm::ivec3 & pixel : color_keys) {
        Uint32 key = SDL_MapRGB(surface->format, pixel.r, pixel.g, pixel.b);
        SDL_SetColorKey(surface, SDL_TRUE, key);
    }
    //SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);
    // {
    //  Uint32 key = SDL_MapRGB(new_surface->format, 255, 0, 220);
    //  SDL_SetColorKey(new_surface, SDL_TRUE, key);
    // }

    // blit!

    const int result = SDL_BlitSurface(surface, nullptr, new_surface, NULL);
    if (0 != result) {
        LP3_LOG_ERROR("SDL error : %s ]]", SDL_GetError());
        LP3_THROW2(lp3::core::Exception, "Couldn't copy surface.");
    }

    auto & old_surface = surface;
    this->surface = new_surface;
    SDL_FreeSurface(old_surface);

    restore();
}

LP3_GFX_API
Texture::~Texture() {
    SDL_FreeSurface(surface);
}

LP3_GFX_API
void Texture::restore() {
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLuint gl_id;
    glGenTextures(1, &gl_id);
    texture = gl::TextureIDOwner(gl_id);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    LP3_LOG_INFO("Surface bytes per pixel=%d", (int)surface->format->BytesPerPixel);
    const int format = (surface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;


    glTexImage2D(
        GL_TEXTURE_2D,    // Generate a new texture.
        0,                // Mip level for this texture (none).
        format,           // # of color components- R, G, B, and A.
        surface->w,
        surface->h,
        0,                // Border.
        format,           // Describe color format to OpenGL.
        GL_UNSIGNED_BYTE, // Data type used for each pixel.
        surface->pixels
    );
}

} }
