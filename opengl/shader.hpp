//
// Created by Richard Hodges on 27/01/2018.
//

#pragma once

#include "error.hpp"
#include "shader_type.hpp"
#include <type_traits>
#include "resource.hpp"
#include <cstring>
#include "detail/gl_string.hpp"
#include "shader_service.hpp"

namespace opengl {


    /// The representation of some kind of shader
    struct shader : resource_object<shader_service>
    {
        shader(shader_type type)
            : resource_object<shader_service>(std::piecewise_construct, type)
        {
        }

        template<class...Sources>
        shader(shader_type type, Sources &&...sources)
            : shader(type)
        {
            constexpr auto count = sizeof...(sources);

            const GLchar *sz_sources[] =
                {
                    detail::to_gl_char(sources)...
                };

            const GLint lengths[] = {
                detail::get_gl_string_length(sources)...
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
            : shader(shader_type::fragment, std::forward<String>(str))
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
            : shader(shader_type::vertex, std::forward<String>(str))
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
            : shader(shader_type::vertex, std::forward<String>(str))
        {}

    };

}


