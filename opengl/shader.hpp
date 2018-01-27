//
// Created by Richard Hodges on 27/01/2018.
//

#pragma once

#include "error.hpp"

namespace opengl
{
    namespace {
        const GLchar* to_gl_char(std::string const& str)
        {
            return str.c_str();
        }

        template<std::size_t N>
        constexpr const GLchar* to_gl_char(const char(&str)[N])
        {
            assert(str);
            return str;
        }

        constexpr const GLchar* to_gl_char(const char* str)
        {
            assert(str);
            return str;
        }

        GLsizei get_gl_string_length(std::string const& str)
        {
            return str.size();
        }

        GLsizei get_gl_string_length(const char* str)
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

    struct shader : shader_defs
    {
        shader(type shader_type)
        :ident_{glCreateShader(static_cast<GLenum>(shader_type))}
        {
            if (not ident_) check_errors("glCreateShader");
        }

        template<class...Sources>
        shader(type shader_type, Sources&&...sources)
            : shader(shader_type)
        {
            constexpr auto count = sizeof...(sources);
            const GLchar * sz_sources[] =
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

        shader(shader&& other) noexcept
            : ident_(other.release())
        {
        }

        shader& operator=(shader&& other) noexcept
        {
            if (std::addressof(other) != this)
            {
                reset(other.release());
            }
            return *this;
        }

        void reset(GLuint id = 0) noexcept
        {
            glDeleteShader(ident_);
            ident_ = id;
        }

        GLuint release() noexcept {
            auto result = ident_;
            ident_ = 0;
            return result;
        }

        void swap(shader& other) noexcept
        {
            using std::swap;
            swap(ident_, other.ident_);
        }

        ~shader() noexcept
        {
            glDeleteShader(release());
        }

        GLuint get_id() const
        {
            return ident_;
        }


        GLuint ident_;
    };

}


