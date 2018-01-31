//
// Created by Richard Hodges on 27/01/2018.
//

#pragma once

#include "error.hpp"
#include <type_traits>
#include "resource.hpp"
#include <cstring>

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


    struct shader_service : shader_defs, basic_resource_service<shader_service, GLuint>
    {

        static auto construct(type shader_type) -> implementation_type
        {
            auto ident = glCreateShader(static_cast<GLenum>(shader_type));
            if (not ident) {
                check_errors("glCreateShader");
                assert(!"should not get here");
            }
            return ident;
        }


        static auto destroy(implementation_type &impl) -> void
        {
            if (impl)
                glDeleteShader(impl);
            impl = 0;
        }

    };


    struct shader : shader_defs, resource_object<shader_service>
    {
        shader(type shader_type)
            : resource_object<shader_service>(std::piecewise_construct, shader_type)
        {
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
            glShaderSource(get_implementation(), count, sz_sources, lengths);
            check_errors("glShaderSource");
            glCompileShader(get_implementation());
            check_errors("glCompileShader");
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


