//
// Created by Richard Hodges on 27/01/2018.
//

#include "shader.hpp"


namespace opengl
{

    auto shader::compiled() const -> bool
    {
        GLint result = GL_FALSE;
        glGetShaderiv(get_implementation(), GL_COMPILE_STATUS, & result);
        return result == GL_TRUE;
    }

    /// Return the log text associated with this shader
    auto shader::log() const -> std::string
    {
        GLint length = 0;
        glGetShaderiv(get_implementation(), GL_INFO_LOG_LENGTH, &length);
        auto result = std::string(length, '\0');
        if (length) {
            glGetShaderInfoLog(get_implementation(), length, nullptr, &result[0]);
        }
        check_errors("glGetShaderInfoLog ");
        return result;
    }


    auto shader::source() const -> std::string
    {
        GLint length = 0;
        glGetShaderiv(get_implementation(), GL_SHADER_SOURCE_LENGTH, &length);
        auto result = std::string(length, '\0');
        glGetShaderSource(get_implementation(), length, nullptr, &result[0]);
        check_errors("glGetShaderSource");
        return result;
    }

    auto shader::type() const -> shader_type
    {
        GLint type_code = 0;
        glGetShaderiv(get_implementation(), GL_SHADER_TYPE, &type_code);
        check_errors("glgetShaderiv");
        return static_cast<shader_type>(type_code);
    }

}