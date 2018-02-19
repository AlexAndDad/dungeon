//
// Created by rhodges on 19/02/18.
//

#include "glew_initialisation.hpp"

namespace opengl
{
    bool glew_initialisation::init_flag;


    glew_initialisation::glew_initialisation()
    {
        static bool initalised = init();
    }

    bool glew_initialisation::init()
    {
        if (not init_flag)
        {
            if (glewInit() != GLEW_OK)
            {
                throw glew_init_failed();
            }
            init_flag = true;
        }
        return init_flag;
    }

}
