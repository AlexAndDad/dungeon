//
// Created by Richard Hodges on 14/02/2018.
//

#pragma once
#include "config.hpp"
#include "basic_resource_object.hpp"
#include "error.hpp"
#include <notstd/handle.hpp>

namespace opengl
{


    struct vertex_array_service : basic_resource_service<vertex_array_service, GLuint>
    {
        static auto construct() -> native_handle_type
        {
            implementation_type result;
            glGenVertexArrays(1, std::addressof(result));
            check_errors("glGenVertexArrays");
            return result;
        }

        static auto destroy(native_handle_type & impl) -> void
        {
            if (impl) {
                glDeleteVertexArrays(1, std::addressof(impl));
            }
        }
    };

    struct vertex_array : notstd::unique_handle<vertex_array_service>
    {
        using inherited = notstd::unique_handle<vertex_array_service>;

        vertex_array() : inherited(std::make_tuple())
        {

        }

    };

    /// Bind a vertex array object to the current context
    auto bind(vertex_array const& va) -> void;
}



