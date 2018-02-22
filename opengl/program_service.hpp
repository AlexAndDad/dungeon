//
// Created by rhodges on 12/02/18.
//

#ifndef DUNGEON_PROGRAM_SERVICE_H
#define DUNGEON_PROGRAM_SERVICE_H
#pragma once

#include <GL/glew.h>
#include <glm/fwd.hpp>

#include "basic_resource_object.hpp"

namespace opengl {

    struct program_compilation_failed : std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };


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

        static auto compiled(implementation_type const &impl) -> bool;

        static std::size_t log_length(implementation_type const &impl);

        static std::string log(implementation_type const &impl);

        static void set_uniform(native_handle_type& impl, unsigned location, glm::vec4 const& vec);
        static unsigned get_uniform_index(native_handle_type const& impl, const char* name);

    };

}

#endif //DUNGEON_PROGRAM_SERVICE_H
