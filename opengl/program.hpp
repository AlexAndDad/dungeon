//
// Created by Richard Hodges on 27/01/2018.
//

#pragma once

#include "shader.hpp"

namespace opengl
{
    struct program : resource<program>
    {
        program()
            : resource(glCreateProgram())
        {
            check_errors("glCreateProgram");
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


