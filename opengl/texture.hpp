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

    /// An enumeration type whose enumeration values map directly to opengl texture type values.
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

    /// Convert a texture_target to the native GLenum value for use in underlying appi calls
    inline constexpr auto to_native(texture_target in) -> GLenum
    {
        return static_cast<GLenum>(in);
    }


    /// Provide the services of managing a texture lifetime for a given texture type
    /// @tparam Texture is a texture_target value
    /// @note an object of a concret specialisation of this class manages exactly one texture of a fixed type
    ///
    template<texture_target Target>
        struct basic_texture_service
            : basic_resource_service<basic_texture_service<Target>, GLenum>
        {
            using inherited = basic_resource_service<basic_texture_service<Target>, GLenum>;
            using implementation_type = typename inherited::implementation_type;

            static constexpr auto target() -> texture_target
            { return Target; }


            static auto construct() -> implementation_type
            {
                implementation_type impl = 0;
                glCreateTextures(to_native(target()), 1, &impl);
                check_errors("glCreateTextures");
                return impl;
            }

            static auto destroy(implementation_type &impl) -> void
            {
                if (impl)
                    glDeleteTextures(1, &impl);
            }
        };

    template<texture_target Target>
        struct basic_texture : basic_resource_object<basic_texture_service<Target>>
        {
            using inherited = basic_resource_object<basic_texture_service<Target>>;
            using service_type  = typename inherited::service_type;

            basic_texture()
                : inherited(std::piecewise_construct)
            { }


        };

    using texture_1d = basic_texture<texture_target::dim1>;
    using texture_2d = basic_texture<texture_target::dim2>;
    using texture_3d = basic_texture<texture_target::dim3>;
    // etc

}


#endif //DUNGEON_TEXTURE_HPP