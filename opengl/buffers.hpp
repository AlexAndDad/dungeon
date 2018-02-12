//
// Created by Richard Hodges on 28/01/2018.
//

#pragma once

#include "opengl/config.hpp"
#include "opengl/error.hpp"
#include "basic_resource_object.hpp"
#include <vector>
#include <initializer_list>
#include <boost/iterator/zip_iterator.hpp>
#include "buffer_implementation.hpp"

namespace opengl {

    struct buffers_service
    {
        using implementation_type = buffer_implementation;

        auto construct(std::size_t N) const -> implementation_type
        {
            auto result = implementation_type(N);
            if (not result.empty()) {
                glGenBuffers(N, result.idents.data());
                check_errors("glGenBuffers");
            }
            return result;
        }

        void destroy(implementation_type &impl) const noexcept
        {
            if (not empty(impl)) {
                glDeleteBuffers(impl.idents.size(), impl.idents.data());
            }

        }

        auto move_construct(implementation_type &other_impl) const noexcept -> implementation_type
        {
            auto result = std::move(other_impl);
            clear(other_impl);
            return result;
        }

        auto move_assign(implementation_type &my_impl, implementation_type &other_impl) const noexcept -> void
        {
            swap(my_impl, other_impl);
            destroy(other_impl);
        }

        bool empty(implementation_type const &impl) const
        {
            return impl.empty();
        }



    };

    /// @todo misunderstanding of bound buffers
    struct buffers : basic_resource_object<buffers_service>
    {
        buffers(std::size_t N);

        buffers(std::initializer_list<buffer_init> il);
        buffers(buffer_init const& bi);


        void bind() const;
        void unbind() const;

        std::size_t size() const;
    };

}


