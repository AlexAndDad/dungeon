//
// Created by Richard Hodges on 14/02/2018.
//

#pragma once
#include "config.hpp"
#include "basic_resource_object.hpp"
#include "error.hpp"

namespace opengl
{


    struct vertex_array_service : basic_resource_service<vertex_array_service, GLuint>
    {
        static auto construct() -> implementation_type
        {
            implementation_type result;
            glGenVertexArrays(1, std::addressof(result));
            check_errors("glGenVertexArrays");
            return result;
        }

        static auto destroy(implementation_type& impl) -> void
        {
            if (impl) {
                glDeleteVertexArrays(1, std::addressof(impl));
                impl = 0;
            }
        }
    };

    struct vertex_array : basic_resource_object<vertex_array_service>
    {
        using inherited = basic_resource_object<vertex_array_service>;

        vertex_array() : inherited(std::piecewise_construct)
        {

        }

    };
}



