//
// Created by rhodges on 13/02/18.
//

#ifndef DUNGEON_TEXTURE_HPP
#define DUNGEON_TEXTURE_HPP

#pragma once

#include "config.hpp"
#include "basic_resource_object.hpp"
#include "error.hpp"

namespace opengl {

    enum class texture_target : GLenum
    {
        dim1 = GL_TEXTURE_1D,
        dim2 = GL_TEXTURE_2D,
        dim3 = GL_TEXTURE_3D,
        dim1_array = GL_TEXTURE_1D_ARRAY,
        sim2_array = GL_TEXTURE_2D_ARRAY,
        rectangle = GL_TEXTURE_RECTANGLE,
        cube_map = GL_TEXTURE_CUBE_MAP,
        cube_map_array = GL_TEXTURE_CUBE_MAP_ARRAY,
        buffer = GL_TEXTURE_BUFFER,
        dim2_multisample = GL_TEXTURE_2D_MULTISAMPLE,
        dim2_multisample_array = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
    };

    inline constexpr auto to_native(texture_target in) -> GLenum
    {
        return static_cast<GLenum>(in);
    }


    template<texture_target Target>
        struct basic_texture_service
            : basic_resource_service<basic_texture_service<Target>, GLenum>
        {
            using implementation_type = typename basic_resource_service<basic_texture_service<Target>, GLenum>::implementation_type;

            static constexpr auto target() -> texture_target
            { return Target; }


            static auto construct() -> implementation_type
            {
                implementation_type impl = 0;
                glCreateTextures(to_native(target()), 1, &impl);
                check_errors("glCreateTextures");
                return impl;
            }

            static auto destroy(implementation_type& impl) -> void
            {
                if (impl)
                    glDeleteTextures(1, &impl);
            }

        };

}


#endif //DUNGEON_TEXTURE_HPP
