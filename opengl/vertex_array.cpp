//
// Created by Richard Hodges on 14/02/2018.
//

#include "vertex_array.hpp"

namespace opengl
{

    /// Bind a vertex array object to the current context
    auto bind(vertex_array const& va) -> void
    {
        glBindVertexArray(va.native_handle());
    }

}
