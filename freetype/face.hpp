//
// Created by rhodges on 22/02/18.
//

#pragma once

#include "config.hpp"
#include "service_base.hpp"
#include "bitmap_glyph.hpp"

#include <notstd/handle.hpp>
#include <boost/filesystem.hpp>
#include <ostream>


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

        void report(native_handle_type const &impl, std::ostream &os);

        void set_pixel_size(native_handle_type& impl, int pixel_size);

        bitmap_glyph get_bitmap(native_handle_type& impl, char ch);
    private:

        library &owner_;
    };

    struct face : notstd::unique_handle<face_service>
    {
        using path = service_type::path;

        face(library &lib, path const &pathname);
        face(library &lib, path const &pathname, int point_size);

        void report(std::ostream &os) const;

        bitmap_glyph get_bitmap(char ch);

    private:
        using inherited = notstd::unique_handle<face_service>;
    };

    auto operator<<(std::ostream &os, face const &arg) -> std::ostream &;

}
