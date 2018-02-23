//
// Created by rhodges on 23/02/18.
//

#include "glyph_renderer.hpp"
#include "context.hpp"

namespace opengl
{
    glyph_renderer_observer::glyph_renderer_observer(context &owner, char character, std::size_t pixel_height)
    : service_(std::addressof(owner.use<service_type>()))
    {

    }
}