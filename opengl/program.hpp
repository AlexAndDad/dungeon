//
// Created by Richard Hodges on 27/01/2018.
//

#pragma once

#include "program_service.hpp"
#include "shader.hpp"
#include <vector>
#include <array>
#include <algorithm>
#include <boost/iterator/zip_iterator.hpp>
#include <cstdint>
#include <cstddef>
#include <initializer_list>

namespace opengl
{

    struct program : basic_resource_object<program_service>
    {
        struct binary;

        program()
            : basic_resource_object<program_service>(std::piecewise_construct)
        {}


        template<class...Shaders>
        program(shader const& shader0, Shaders&&...shaderN)
            : program()
        {
            attach_shaders(shader0, std::forward<Shaders>(shaderN)...);
            glLinkProgram(get_implementation());
            check_errors("glLinkProgram");
        }

        template<class...Shaders>
            void attach_shaders(Shaders&&...shaders)
        {
            using expand = int[];
            void(expand{0,
                        (glAttachShader(native_handle(), shaders.native_handle()),0)...
            });
            check_errors("glAttachShader");
        }

        void use()
        {
            glUseProgram(get_implementation());
            opengl::check_errors("glUseProgram");
        }

        auto get_binary() const -> binary;

    };

    struct program::binary
    {
        struct prepare_args
        {
            GLenum* format_ptr;
            std::uint8_t* buffer_ptr;
            GLsizei size;
        };

        binary();

        auto prepare(std::size_t size) -> prepare_args;

        void report(std::ostream& os) const;

    private:
        GLenum format_;
        std::vector<std::uint8_t> data_;
    };
}


