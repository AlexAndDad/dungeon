//
// Created by Richard Hodges on 27/01/2018.
//

#pragma once

#include "shader.hpp"
#include <vector>
#include <array>
#include <algorithm>
#include <boost/iterator/zip_iterator.hpp>

namespace opengl
{

    struct buffer_init
    {
        GLuint buffer_type;
        struct {

        };
        void* buffer_data;
    };

    struct buffer_item
    {
        GLuint buffer_type;
        GLuint buffer_ident;
    };

    struct buffers
    {
        template<class...BufferInit>
        buffers(std::initializer_list<buffer_init> inits)
            : buffers_(inits.size())
        {
            /*
            const auto size = buffers_.size();
            auto idents = generate_buffers(size);

            auto first = boost::make_zip_iterator(std::begin(inits), std::begin(std::begin(idents), std::begin(buffers_));
            auto last = boost::make_zip_iterator(std::end(inits), std::end(idents), std::end(buffers_));
            for( ; first != last ; ++first)
            {
                auto& init = boost::get<0>(*first);
                auto& ident = boost::get<1>(*first);
                auto& buffer = boost::get<2>(*first);
                buffer.buffer_ident = ident;
                buffer.buffer_type = init.buffer_type;
                glBindBuffer(buffer.buffer_type, ident);
                glBufferData(buffer.buffer_type, sizeof(vertices), vertices, GL_STATIC_DRAW);

            }
             */
        }


        auto generate_buffers(std::size_t N) -> std::vector<GLuint>
        {
            auto result = std::vector<GLuint>(N);
            glGenBuffers(N, result.data());
            opengl::check_errors("glGenBuffers");
            return result;
        }



        std::vector<buffer_item> buffers_;
    };

    struct program : resource<program>
    {
        program()
            : resource(glCreateProgram())
        {
            check_errors("glCreateProgram");
        }

        void use()
        {
            glUseProgram(get_id());
            opengl::check_errors("glUseProgram");
        }

        template<class...Shaders>
        program(shader const& shader0, Shaders&&...shaderN)
            : resource(glCreateProgram())
        {
            attach_shaders(shader0, std::forward<Shaders>(shaderN)...);
            glLinkProgram(get_id());
            check_errors("glLinkProgram");
        }

        template<class...Shaders>
            void attach_shaders(Shaders&&...shaders)
        {
            using expand = int[];
            void(expand{0,
                        (glAttachShader(get_id(), shaders.get_id()),0)...
            });
            check_errors("glAttachShader");
        }

        static void destroy(GLuint id)
        {
            glDeleteProgram(id);
        }
    };

}


