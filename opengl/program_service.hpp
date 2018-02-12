//
// Created by rhodges on 12/02/18.
//

#ifndef DUNGEON_PROGRAM_SERVICE_H
#define DUNGEON_PROGRAM_SERVICE_H
#pragma once

#include <GL/glew.h>
#include "resource.hpp"

namespace opengl {

    /// A service object which provides the construct and destroy specialisations for GL program objects
    struct program_service : basic_resource_service<program_service, GLuint>
    {
        /// Construct a program object in the current GL context
        static auto construct() -> implementation_type;

        /// Destroy a program object in the current GL context
        /// @param is a reference to the program's ID.
        /// @post id will be zero
        /// @note if id is zero at the point of the call, this operation is a NOP
        static void destroy(implementation_type &id);
    };

}

#endif //DUNGEON_PROGRAM_SERVICE_H
