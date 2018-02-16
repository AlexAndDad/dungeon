//
// Created by rhodges on 12/02/18.
//

#include "shader_service.hpp"

namespace opengl {
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
    }

    std::size_t shader_service::source_length(implementation_type const &impl)
    {
        GLint length = 0;
        glGetShaderiv(impl, GL_SHADER_SOURCE_LENGTH, &length);
        check_errors("glGetShaderiv");
        return std::size_t(length);
    }

    std::string shader_service::source(implementation_type const& impl)
    {
        std::string result;
        if(auto length = source_length(impl))
        {
            result.resize(length-1);
            glGetShaderSource(impl, length, nullptr, &result[0]);
            check_errors("glGetShaderSource");
        }
        return result;

    }

    std::size_t shader_service::log_length(implementation_type const &impl)
    {
        GLint length = 0;
        glGetShaderiv(impl, GL_INFO_LOG_LENGTH, &length);
        check_errors("glGetShaderiv");
        return std::size_t(length);
    }

    std::string shader_service::log(implementation_type const &impl)
    {
        std::string result;
        if(auto length = log_length(impl))
        {
            result.resize(length-1);
            glGetShaderInfoLog(impl, length, nullptr, &result[0]);
            check_errors("glGetShaderInfoLog");
        }
        return result;
    }


}