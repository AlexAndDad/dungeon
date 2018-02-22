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
#include <notstd/handle.hpp>
#include <glm/fwd.hpp>

namespace opengl
{

    struct program : notstd::unique_handle<program_service>
    {
        struct binary;

        program()
            : notstd::unique_handle<program_service>(std::make_tuple())
        {}


        template<class...Shaders>
        program(shader const& shader0, Shaders&&...shaderN)
            : program()
        {
            attach_shaders(shader0, std::forward<Shaders>(shaderN)...);
            glLinkProgram(native_handle());
            check_errors("glLinkProgram");
            if (not compiled())
                throw program_compilation_failed(log());
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
            glUseProgram(native_handle());
            opengl::check_errors("glUseProgram");
        }

        auto compiled() const -> bool;

        auto log() const -> std::string;

        auto get_binary() const -> binary;


        unsigned get_uniform_index(const char* name) const;

        template<typename Type>
        auto set_uniform(unsigned index, Type&& value)
        {
            return get_service().set_uniform(mutable_native_handle(), index, std::forward<Type>(value));
        }

        template<typename Type>
        auto set_uniform(const char* name, Type&& value)
        {
            return set_uniform(get_uniform_index(name), std::forward<Type>(value));
        }

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


