//
// Created by Richard Hodges on 27/01/2018.
//

#pragma once

#include "error.hpp"
#include <type_traits>
#include "resource.hpp"

namespace opengl {
    namespace {
        const GLchar *to_gl_char(std::string const &str)
        {
            return str.c_str();
        }

        template<std::size_t N>
        constexpr const GLchar *to_gl_char(const char(&str)[N])
        {
            assert(str);
            return str;
        }

        constexpr const GLchar *to_gl_char(const char *str)
        {
            assert(str);
            return str;
        }

        GLsizei get_gl_string_length(std::string const &str)
        {
            return str.size();
        }

        GLsizei get_gl_string_length(const char *str)
        {
            assert(str);
            return std::strlen(str);
        }

        template<std::size_t N>
        constexpr GLsizei get_gl_string_length(const char(&str)[N])
        {
            return N;
        }

    }
    struct shader_defs
    {
        enum class type
        {
            vertex = GL_VERTEX_SHADER,
            fragment = GL_FRAGMENT_SHADER
        };
    };

    struct shader : shader_defs, resource<shader>
    {
        shader(type shader_type)
            : resource(glCreateShader(static_cast<GLenum>(shader_type)))
        {
            if (empty()) check_errors("glCreateShader");
        }

        template<class...Sources>
        shader(type shader_type, Sources &&...sources)
            : shader(shader_type)
        {
            constexpr auto count = sizeof...(sources);
            const GLchar *sz_sources[] =
                {
                    to_gl_char(sources)...
                };
            const GLint lengths[] = {
                get_gl_string_length(sources)...
            };
            glShaderSource(get_id(), count, sz_sources, lengths);
            check_errors("glShaderSource");
            glCompileShader(get_id());
            check_errors("glCompileShader");
        }

        static void destroy(GLuint id)
        {
            glDeleteShader(id);
        }

    };

    struct fragment_shader : shader
    {
        template
            <
                class String,
                std::enable_if_t
                    <
                        not std::is_base_of<shader, std::decay_t<String>>::value
                    > * = nullptr
            >
        fragment_shader(String &&str)
            : shader(type::fragment, std::forward<String>(str))
        {

        }
    };

    struct vertex_shader : shader
    {
        template
            <
                class String,
                std::enable_if_t
                    <
                        not std::is_base_of<shader, std::decay_t<String>>::value
                    > * = nullptr
            >
        vertex_shader(String &&str)
            : shader(type::vertex, std::forward<String>(str))
        {}

    };

}


