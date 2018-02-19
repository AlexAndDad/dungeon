//
// Created by rhodges on 19/02/18.
//

#pragma once

#include "config.hpp"
#include <stdexcept>

namespace opengl
{
    struct glew_init_failed : std::runtime_error
    {
        glew_init_failed();
    };


    struct glew_initialisation
    {

        glew_initialisation();

    private:
        static bool init();

        static bool init_flag;
    };
}