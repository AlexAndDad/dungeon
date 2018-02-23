//
// Created by rhodges on 23/02/18.
//

#pragma once

#include "config.hpp"
#include "glyph_render_impl.hpp"
#include "vertex_array_object.hpp"
#include "program.hpp"
#include "context_service.hpp"
#include "resource_loader_service.hpp"

namespace opengl {


    // Manage
    struct glyph_render_service : implement_context_service<glyph_render_service>
    {
        using implementation_class = glyph_render_impl;
        using implementation_type = implementation_class*;

        glyph_render_service(context &owner);


        struct program_uniforms
        {

        };

        struct program_args
        {

        };

    private:
        auto make_glyph_program() -> program;

        resource_loader_service& resource_loader_;
        vertex_array_object glyph_vao_;
        program glyph_program_;
    };

}