//
// Created by rhodges on 12/02/18.
//

#include "program_service.hpp"
#include "error.hpp"

namespace opengl {
    auto program_service::construct() -> implementation_type
    {
        auto result = glCreateProgram();
        check_errors("glCreateProgram");
        return result;
    }

    void program_service::destroy(implementation_type &id)
    {
        if (id)
            glDeleteProgram(id);
    }

    auto program_service::compiled(implementation_type const &impl) -> bool
    {
        GLint result = GL_FALSE;
        glGetProgramiv(impl, GL_LINK_STATUS, &result);
        check_errors("glGetProgramiv");
        return result == GL_TRUE;
    }

    std::size_t program_service::log_length(implementation_type const &impl)
    {
        GLint length = 0;
        glGetProgramiv(impl, GL_INFO_LOG_LENGTH, &length);
        check_errors("glGetProgramiv");
        return std::size_t(length);
    }

    std::string program_service::log(implementation_type const &impl)
    {
        std::string result;
        if (auto length = log_length(impl)) {
            result.resize(length - 1);
            glGetProgramInfoLog(impl, length, nullptr, &result[0]);
            check_errors("glGetProgramInfoLog");
        }
        return result;
    }

}