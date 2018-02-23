//
// Created by Richard Hodges on 14/02/2018.
//

#include "vertex_array_object.hpp"

namespace opengl
{
    void vertex_array_object_service::bind(native_handle_type const &impl)
    {
        glBindVertexArray(impl);
        check_errors("glBindVertexArray");
    }

    void vertex_array_object::bind() const
    {
        get_service().bind(native_handle());
    }

    /// Bind a vertex array object to the current context
    auto bind(vertex_array_object const& va) -> void
    {
        va.bind();
    }

}
