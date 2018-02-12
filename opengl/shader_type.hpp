//
// Created by rhodges on 12/02/18.
//

#ifndef DUNGEON_SHADER_TYPE_H
#define DUNGEON_SHADER_TYPE_H
#pragma once

#include <GL/glew.h>
#include <string>

namespace opengl {

    /// Enumeration representing legal GL shader types
    enum class shader_type : GLenum
    {
        vertex = GL_VERTEX_SHADER,          ///< vertex shader

        fragment = GL_FRAGMENT_SHADER,      ///< fragment shader

        geometry = GL_GEOMETRY_SHADER       ///< geometry shader
    };

    /// represent a shader type as a human-readable string
    /// @param x is the shader type
    /// @return a human-readable string
    ///
    auto to_string(shader_type x) -> std::string;

    /// stream a human-readable representation of the shader type to an ostream
    /// @param os is a reference to the destination stream
    /// @param x is the shader type
    /// @return a reference to os
    ///
    auto operator<<(std::ostream &os, shader_type x) -> std::ostream &;
}

#endif //DUNGEON_SHADER_TYPE_H
