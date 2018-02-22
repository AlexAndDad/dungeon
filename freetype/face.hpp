//
// Created by rhodges on 22/02/18.
//

#pragma once

#include "service_base.hpp"

#include <notstd/handle.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <boost/filesystem.hpp>

namespace freetype {

    struct library;

    struct face_service : service_base, notstd::stateful_service<face_service>
    {
        using native_handle_type = FT_Face;
        using path = boost::filesystem::path;

        face_service(library &owner);

        native_handle_type construct(path const &pathname);

        void destroy(native_handle_type &impl) noexcept;

        void invalidate(native_handle_type &impl) noexcept;

        bool empty(native_handle_type const &impl) noexcept;

    private:

        library &owner_;
    };

    struct face : notstd::unique_handle<face_service>
    {
        using path = service_type ::path;

        face(library &lib, path const &pathname);

    private:
        using inherited = notstd::unique_handle<face_service>;
    };

}
