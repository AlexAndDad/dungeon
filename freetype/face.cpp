//
// Created by rhodges on 22/02/18.
//

#include "face.hpp"
#include "library.hpp"
#include <array>

namespace freetype {
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

    namespace {
        template<
            class FlagInt,
            class StringType, std::size_t N>
        std::ostream &
        accumulate_flags(std::ostream &os, FlagInt flags, std::pair<FlagInt, StringType> const (&xlate)[N])
        {
            bool empty = true;

            for (auto &&entry : xlate)
            {
                auto &&mask = std::get<0>(entry);
                auto &&str = std::get<1>(entry);
                if ((flags & mask) == mask)
                {
                    if (not empty) os << ", ";
                    else empty = false;
                    os << str;
                }
            }

            if (empty)
            {
                os << "none";
            }

            return os;
        };

        constexpr std::pair<FT_Long, const char *> face_flags[] =
            {
                {FT_FACE_FLAG_SCALABLE,         "scalable"},
                {FT_FACE_FLAG_FIXED_SIZES,      "fixed_sizes"},
                {FT_FACE_FLAG_FIXED_WIDTH,      "fixed_width"},
                {FT_FACE_FLAG_SFNT,             "sfnt"},
                {FT_FACE_FLAG_HORIZONTAL,       "horizontal"},
                {FT_FACE_FLAG_KERNING,          "kerning"},
                {FT_FACE_FLAG_FAST_GLYPHS,      "fast_glyphs"},
                {FT_FACE_FLAG_MULTIPLE_MASTERS, "multiple_masters"},
                {FT_FACE_FLAG_GLYPH_NAMES,      "glyph_names"},
                {FT_FACE_FLAG_EXTERNAL_STREAM,  "external_stream"},
                {FT_FACE_FLAG_HINTER,           "hinter"},
                {FT_FACE_FLAG_CID_KEYED,        "cid_keyed"},
                {FT_FACE_FLAG_TRICKY,           "tricky"},
                {FT_FACE_FLAG_COLOR,            "color"},
            };

        constexpr std::pair<FT_Long, const char *> style_flags[] =
            {
                {FT_STYLE_FLAG_BOLD,   "bold"},
                {FT_STYLE_FLAG_ITALIC, "italic"},
            };
    }

    void face_service::report(native_handle_type const &impl, std::ostream &os)
    {
        os << "face count: " << impl->num_faces;
        os << "\nface index: " << impl->face_index;
        os << "\nface flags: ";
        accumulate_flags(os, impl->face_flags, face_flags);
        os << "\nstyle flags: ";
        accumulate_flags(os, impl->style_flags, style_flags);
    }

    void face_service::set_pixel_size(native_handle_type& impl, int pixel_size)
    {
        if (auto err = FT_Set_Pixel_Sizes(impl, 0, pixel_size))
        {
            throw std::runtime_error("face_service : set_pixel_size failed : " + std::to_string(err));
        }
    }

    bitmap_glyph face_service::get_bitmap(native_handle_type& impl, char ch)
    {
        if (auto err = FT_Load_Char(impl, ch, FT_LOAD_RENDER))
        {
            throw std::runtime_error("failed to load render glyph for char code " + std::to_string(unsigned(ch)) + " error : " + std::to_string(err));
        }
        return bitmap_glyph(impl->glyph);
    }


    //

    face::face(library &lib, path const &pathname)
        : inherited(lib.use<service_type>(), std::tie(pathname))
    { }

    face::face(library &lib, path const &pathname, int pixel_size)
        : inherited(lib.use<service_type>(), std::tie(pathname))
    {
        get_service().set_pixel_size(mutable_native_handle(), pixel_size);
    }

    void face::report(std::ostream &os) const
    {
        get_service().report(native_handle(), os);
    }

    bitmap_glyph face::get_bitmap(char ch)
    {
        return get_service().get_bitmap(mutable_native_handle(), ch);
    }

    auto operator<<(std::ostream &os, face const &arg) -> std::ostream &
    {
        arg.report(os);
        return os;
    }

}