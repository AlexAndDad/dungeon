//
// Created by rhodges on 12/02/18.
//

#ifndef DUNGEON_GL_STRING_HPP
#define DUNGEON_GL_STRING_HPP

#pragma once
#include <GL/gl.h>
#include <string>

namespace opengl {
    namespace detail {


        inline const GLchar *to_gl_char(std::string const &str) {
            return str.c_str();
        }

        template<std::size_t N>
        inline constexpr const GLchar *to_gl_char(const char(&str)[N]) {
            assert(str);
            return str;
        }

        inline constexpr const GLchar *to_gl_char(const char *str) {
            assert(str);
            return str;
        }

        inline GLint get_gl_string_length(std::string const &str) {
            return GLint(str.size());
        }

        inline GLint get_gl_string_length(const char *str) {
            assert(str);
            return GLint(std::strlen(str));
        }

        template<std::size_t N>
        constexpr GLint get_gl_string_length(const char(&str)[N]) {
            return GLint(N);
        }

    }
}

#endif //DUNGEON_GL_STRING_HPP
