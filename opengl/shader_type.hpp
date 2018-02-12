//
// Created by rhodges on 12/02/18.
//

#ifndef DUNGEON_SHADER_TYPE_H
#define DUNGEON_SHADER_TYPE_H
#pragma once

#include <GL/glew.h>
#include <string>

namespace opengl {
    enum class shader_type : GLenum {
        vertex = GL_VERTEX_SHADER,
        fragment = GL_FRAGMENT_SHADER,
        geometry = GL_GEOMETRY_SHADER
    };

    auto to_string(shader_type x) -> std::string;

    auto operator<<(std::ostream &os, shader_type x) -> std::ostream&;
}

#endif //DUNGEON_SHADER_TYPE_H
