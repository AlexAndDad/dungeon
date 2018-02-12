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
    struct shader_service : basic_resource_service<shader_service, GLuint> {

        static auto construct(shader_type type) -> implementation_type;

        static auto destroy(implementation_type &impl) -> void;

    };

}

#endif //DUNGEON_SHADER_SERVICE_HPP
