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

    struct program_service : basic_resource_service<program_service, GLuint>
    {
        static auto construct() -> implementation_type
        {
            auto result = glCreateProgram();
            check_errors("glCreateProgram");
            return result;
        }

        static void destroy(implementation_type& id)
        {
            glDeleteProgram(id);
            id = 0;
        }


    };

    struct program : resource_object<program_service>
    {

        program()
            : resource_object<program_service>(std::piecewise_construct)
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
                        (glAttachShader(get_implementation(), shaders.get_implementation()),0)...
            });
            check_errors("glAttachShader");
        }

        void use()
        {
            glUseProgram(get_implementation());
            opengl::check_errors("glUseProgram");
        }

    };

}


