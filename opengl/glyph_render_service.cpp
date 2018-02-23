//
// Created by rhodges on 23/02/18.
//

#include "glyph_render_service.hpp"
#include "context.hpp"

namespace opengl {

    glyph_render_service::glyph_render_service(context &owner)
    : implement_context_service<glyph_render_service>(owner)
        , resource_loader_(get_context().use<resource_loader_service>())
    , glyph_vao_()
    , glyph_program_(make_glyph_program())
    {


    }


    auto glyph_render_service::make_glyph_program() -> program
    {
        return program(vertex_shader(resource_loader_.load_as_string("system/glyph_vertex_shader.glsl")),
                       fragment_shader(resource_loader_.load_as_string("system/glyph_fragment_shader.glsl")));

    }
}
