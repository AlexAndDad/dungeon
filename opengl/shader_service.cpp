//
// Created by rhodges on 12/02/18.
//

#include "shader_service.hpp"

namespace opengl
{
    auto shader_service::construct(shader_type type) -> implementation_type
    {
        auto ident = glCreateShader(static_cast<GLenum>(type));
        if (not ident) {
            check_errors("glCreateShader");
            assert(!"should not get here");
        }
        return ident;
    }


    auto shader_service::destroy(implementation_type &impl) -> void
    {
        if (impl)
            glDeleteShader(impl);
        impl = 0;
    }

}