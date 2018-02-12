//
// Created by Richard Hodges on 27/01/2018.
//

#pragma once

#include "error.hpp"
#include "shader_type.hpp"
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

        GLint get_gl_string_length(std::string const &str)
        {
            return GLint(str.size());
        }

        GLint get_gl_string_length(const char *str)
        {
            assert(str);
            return GLint(std::strlen(str));
        }

        template<std::size_t N>
        constexpr GLint get_gl_string_length(const char(&str)[N])
        {
            return GLint(N);
        }

    }



    struct shader_defs
    {
        using type = shader_type;
    };


    /// An object which provides resource management services to a shader object
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


    /// The representation of some kind of shader
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

        /// Return the source code for this shader object, if it has any
        auto source() const -> std::string;

        /// Return the shader type
        auto type() const -> shader_type;

        /// Check whether the shader has compiled
        auto compiled() const -> bool;

        /// Return the log text associated with this shader
        auto log() const -> std::string;
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

    struct geometry_shader : shader
    {
        template
                <
                        class String,
                        std::enable_if_t
                                <
                                        not std::is_base_of<shader, std::decay_t<String>>::value
                                > * = nullptr
                >
        geometry_shader(String &&str)
                : shader(type::vertex, std::forward<String>(str))
        {}

    };

}


