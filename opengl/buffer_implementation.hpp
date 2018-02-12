//
// Created by rhodges on 12/02/18.
//

#ifndef DUNGEON_BUFFER_IMPLEMENTATION_HPP
#define DUNGEON_BUFFER_IMPLEMENTATION_HPP

#pragma once

#include "config.hpp"
#include <vector>

namespace opengl
{
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



    /// A class which holds sufficient information to manage buffer objects and their associated GL target names
    struct buffer_implementation
    {
        buffer_implementation(std::size_t N);


        auto empty() const { return idents.empty(); }

        friend void clear(buffer_implementation& impl);

        friend void swap(buffer_implementation& l, buffer_implementation& r) noexcept;

        std::vector<GLuint> idents;
        std::vector<buffer_target> targets;
    };



}

#endif //DUNGEON_BUFFER_IMPLEMENTATION_HPP
