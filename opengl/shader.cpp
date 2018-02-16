//
// Created by Richard Hodges on 27/01/2018.
//

#include "shader.hpp"


namespace opengl {

    auto shader::compiled() const -> bool
    {
        GLint result = GL_FALSE;
        glGetShaderiv(native_handle(), GL_COMPILE_STATUS, &result);
        return result == GL_TRUE;
    }

    /// Return the log text associated with this shader
    auto shader::log() const -> std::string
    {
        return get_service().log(native_handle());
    }


    auto shader::source() const -> std::string
    {
        return get_service().source(native_handle());
    }

    auto shader::type() const -> shader_type
    {
        GLint type_code = 0;
        glGetShaderiv(native_handle(), GL_SHADER_TYPE, &type_code);
        check_errors("glgetShaderiv");
        return static_cast<shader_type>(type_code);
    }

}