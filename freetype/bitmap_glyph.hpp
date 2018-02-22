//
// Created by rhodges on 22/02/18.
//

#pragma once

#include "config.hpp"
#include <ostream>
#include <iomanip>

namespace freetype {

    struct bitmap_glyph
    {
        bitmap_glyph(FT_GlyphSlot impl) : impl_(impl)
        { }

        int x() const
        {
            return impl_->bitmap_left;
        }

        int y() const
        {
            return impl_->bitmap_top;
        }

        unsigned width() const
        {
            return impl_->bitmap.width;
        }

        unsigned height() const
        {
            return impl_->bitmap.rows;
        }

        unsigned char const* data() const
        {
            return impl_->bitmap.buffer;
        }

    private:
        FT_GlyphSlot impl_;
    };

    inline
    std::ostream &operator<<(std::ostream &os, bitmap_glyph const &arg)
    {
        os << "x : " << arg.x();
        os << "\ny : " << arg.y();
        os << "\nwidth : " << arg.width();
        os << "\nheight : " << arg.height();
        os << "\ndata:\n";
        os << std::hex;

        const char* linesep = "";
        for (int y = 0 ; y < arg.height() ; ++y)
        {
            os << linesep;
            linesep = "\n";
            const char* sep = "";
            for (int x = 0; x < arg.width(); ++x)
            {
                os << sep << std::setw(2) << std::setfill('0') << unsigned(arg.data()[y * arg.width() + x]);
                sep = " ";
            }
        }
        return os;
    }

}

