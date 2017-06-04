#ifndef FILE_LP3_GFX_GEOMETRY2_HPP
#define FILE_LP3_GFX_GEOMETRY2_HPP

#include "geometry.hpp"
#include "pixels.hpp"

namespace lp3 { namespace gfx {

struct NonTexturedQuad
{
    PolyPoint a;
    VertexPixel ca;
    PolyPoint b;
    VertexPixel cb;
    PolyPoint c;
    VertexPixel cc;
    PolyPoint d;
    VertexPixel cd;
};

struct NonTexturedFlatQuad
{
    ScreenPoint a;
    VertexPixel ca;
    ScreenPoint b;
    VertexPixel cb;
    ScreenPoint c;
    VertexPixel cc;
    ScreenPoint d;
    VertexPixel cd;
    float z;
};

struct Vertex
{
    PolyPoint p;
    TexturedPoint t;
    VertexPixel c;
};


} }

#endif
