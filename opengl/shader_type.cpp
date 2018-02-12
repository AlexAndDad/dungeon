//
// Created by rhodges on 12/02/18.
//

#include "shader_type.hpp"
#include <cassert>

namespace opengl {

    auto to_string(shader_type x) -> std::string
    {
        switch(x)
        {
            case shader_type ::fragment: return "fragment";
            case shader_type ::geometry: return "geometry";
            case shader_type ::vertex: return "vertex";
        }
        assert(!"unknown shader type");
        return "unknown shader type: " + std::to_string(static_cast<GLenum>(x));
    }

    auto operator<<(std::ostream &os, shader_type x) -> std::ostream&
    {
        return os << to_string(x);
    }

}