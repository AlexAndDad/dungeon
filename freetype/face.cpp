//
// Created by rhodges on 22/02/18.
//

#include "face.hpp"
#include "library.hpp"

namespace freetype
{
    face_service::face_service(library &owner)
    : owner_(owner)
    { }

    face_service::native_handle_type face_service::construct(path const &pathname)
    {
        native_handle_type result;

        auto err = FT_New_Face(owner_.native_handle(), pathname.c_str(), 0, std::addressof(result));
        if (err)
        {
            throw std::runtime_error("error creating face: [" + pathname.string() + "] : " + std::to_string(err));
        }

        return result;
    }

    void face_service::destroy(native_handle_type &impl) noexcept
    {
        if (not empty(impl))
        {
            FT_Done_Face(impl);
        }
    }

    bool face_service::empty(native_handle_type const &impl) noexcept
    {
        return impl == nullptr;
    }

    face::face(library& lib, path const &pathname)
    : inherited(lib.use<service_type>(), std::tie(pathname))
    { }

}