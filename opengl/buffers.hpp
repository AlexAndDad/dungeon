//
// Created by Richard Hodges on 28/01/2018.
//

#pragma once

#include "opengl/config.hpp"
#include "opengl/error.hpp"
#include "resource.hpp"
#include <vector>
#include <initializer_list>
#include <boost/iterator/zip_iterator.hpp>

namespace opengl {

    enum class buffer_usage : GLenum
    {
        static_draw = GL_STATIC_DRAW
    };

    enum class buffer_target : GLenum
    {
        array = GL_ARRAY_BUFFER
    };

    struct buffer_init
    {
        buffer_target target;
        std::size_t size;
        void const *data;
        buffer_usage usage;
    };

    template<class T, std::size_t N>
    constexpr auto array_buffer(const T (&data)[N], buffer_usage usage) -> buffer_init
    {
        return buffer_init {buffer_target::array,
                            sizeof(data),
                            data,
                            usage};
    };



    struct buffer_implementation
    {
        buffer_implementation(std::size_t N)
            : idents(N)
            , targets(N)
        {}

        std::vector<GLuint> idents;
        std::vector<buffer_target> targets;

        auto empty() const { return idents.empty(); }

        friend void clear(buffer_implementation& impl)
        {
            impl.idents.clear();
            impl.targets.clear();
        }

        friend void swap(buffer_implementation& l, buffer_implementation& r) noexcept
        {
            using std::swap;
            swap(l.idents, r.idents);
            swap(l.targets, r.targets);
        }

    };


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
    struct buffers : resource_object<buffers_service>
    {
        buffers(std::size_t N);

        buffers(std::initializer_list<buffer_init> il);
        buffers(buffer_init const& bi);


        void bind() const;
        void unbind() const;

        std::size_t size() const;
    };

}


