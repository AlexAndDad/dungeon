//
// Created by Richard Hodges on 28/01/2018.
//

#pragma once

#include "opengl/config.hpp"
#include "opengl/error.hpp"
#include "resource.hpp"
#include <vector>
#include <initializer_list>

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

    struct buffers_service : basic_resource_service<buffers_service, std::vector<GLuint>>
    {
        auto construct(std::size_t N) const -> implementation_type
        {
            auto result = implementation_type(N);
            if (not result.empty()) {
                glGenBuffers(N, result.data());
                check_errors("glGenBuffers");
            }
            return result;
        }

        void destroy(implementation_type &impl) const noexcept
        {
            if (not empty(impl)) {
                glDeleteBuffers(impl.size(), impl.data());
            }

        }
    };

    struct buffers : resource_object<buffers_service>
    {
        buffers(std::size_t N);

        buffers(std::initializer_list<buffer_init> il);
    };

}


