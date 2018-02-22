//
// Created by rhodges on 21/02/18.
//

#include "library.hpp"
#include "face.hpp"
#include <stdexcept>

namespace freetype {

    auto library_service::construct() -> native_handle_type
    {
        native_handle_type result = nullptr;
        if (FT_Init_FreeType(&result))
        {
            throw std::runtime_error("freetype - Failed to initialise");
        }
        return result;
    }

    auto library_service::destroy(native_handle_type &impl) noexcept -> void
    {
        if (impl)
        {
            FT_Done_FreeType(impl);
        }
    }

    bool library_service::empty(native_handle_type &impl) noexcept
    {
        return not impl;
    }

    void library_service::invalidate(native_handle_type &impl) noexcept
    {
        impl = nullptr;
    }

    library::library()
        : inherited(std::make_tuple())
    {
    }


    auto library::acquire(path const &pathname) -> face
    {
        return face(*this, pathname);
    }

    auto library::acquire(path const &pathname, int pixel_height) -> face
    {
        return face(*this, pathname, pixel_height);
    }

}