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

        bind(glyph_vao_);

        glyph_program_.use();



    }


    auto glyph_render_service::make_glyph_program() -> program
    {
        auto vertex_mapping = resource_loader_.load_as_mapped_file("system/glyph_vertex_shader.glsl");
        auto fragment_mapping = resource_loader_.load_as_mapped_file("system/glyph_fragment_shader.glsl");
        return program(vertex_shader(vertex_mapping.as_string_view()),
                       fragment_shader(fragment_mapping.as_string_view()));

    }
}
