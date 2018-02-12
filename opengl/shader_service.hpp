//
// Created by rhodges on 12/02/18.
//

#ifndef DUNGEON_SHADER_SERVICE_HPP
#define DUNGEON_SHADER_SERVICE_HPP

#pragma once

#include "resource.hpp"
#include "shader_type.hpp"
#include "error.hpp"

namespace opengl {
    /// An object which provides resource management services to a shader object
    struct shader_service : basic_resource_service<shader_service, GLuint>
    {
        /// Construct a shader identity of a given type
        /// @param type is a gl shader type enum
        /// @return the gl id of a new shader
        ///
        static auto construct(shader_type type) -> implementation_type;

        /// Destroy a gl shader object if not zero
        /// @param impl is a reference to a shader id
        /// @pre impl contains either a valid shader object id or 0
        /// @post impl shall contain 0
        ///
        static auto destroy(implementation_type &impl) -> void;
    };

}

#endif //DUNGEON_SHADER_SERVICE_HPP
