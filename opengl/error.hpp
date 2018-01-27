//
// Created by Richard Hodges on 27/01/2018.
//

#pragma once

#include "config.hpp"
#include <vector>

namespace opengl
{
    enum class error
    {
        invalid_value  = GL_INVALID_VALUE,
        invalid_operation = GL_INVALID_OPERATION,

        custom_errors = 10000000,
        shader_creation_error
    };




    auto opengl_category() -> system_namespace::error_category const&;

    auto make_error_code(error err) -> system_namespace::error_code;

    struct api_refused : std::exception
    {
        api_refused(const char* context, std::vector<GLenum>&& codes);

        const char* what() const noexcept;

    private:
        void generate_what() const noexcept;
        static std::string initial_what();
        static thread_local api_refused const* what_last_;
        static thread_local std::string what_string_;
        std::string context_;
        std::vector<GLuint> codes_;
    };

    void check_errors(const char* context);
}

OPEN_SYSTEM_NAMESPACE

    template<>
    struct is_error_code_enum<::opengl::error> : true_type {};

CLOSE_SYSTEM_NAMESPACE

