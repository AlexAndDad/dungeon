//
// Created by Richard Hodges on 27/01/2018.
//

#pragma once

#include "config.hpp"

namespace opengl {

    template<class Outer>
    struct resource
    {
        resource(GLuint id = 0)
            : ident_(id)
        {}

        resource(resource &&other) noexcept
        : ident_(other.release())
        {
        }

        resource &operator=(resource &&other) noexcept
        {
            if (std::addressof(other) != this) {
                static_cast<Outer*>(this)->reset(other.release());
            }
            return *this;
        }

        void reset(GLuint id = 0) noexcept
        {
            std::swap(id, ident_);
            static_cast<Outer*>(this)->destroy(id);
        }


        ~resource() noexcept
        {
            static_cast<Outer*>(this)->destroy(release());
        }

        constexpr bool empty() const
        {
            return ident_ == 0;
        }

        GLuint release() noexcept
        {
            auto result = ident_;
            ident_ = 0;
            return result;
        }

        void swap(resource &other) noexcept
        {
            using std::swap;
            swap(ident_, other.ident_);
        }

        constexpr const GLuint& get_id() const
        {
            return ident_;
        }

    private:
        GLuint ident_;
    };


}
