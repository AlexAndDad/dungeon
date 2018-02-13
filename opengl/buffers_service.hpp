//
// Created by rhodges on 13/02/18.
//

#ifndef DUNGEON_BUFFERS_SERVICE_HPP
#define DUNGEON_BUFFERS_SERVICE_HPP

#pragma once

#include "buffer_implementation.hpp"

namespace opengl {

    struct buffers_service
    {
        using implementation_type = buffer_implementation;

        static auto construct(std::size_t N) -> implementation_type;

        static void destroy(implementation_type &impl) noexcept;

        static auto move_construct(implementation_type &other_impl) noexcept -> implementation_type;

        static auto move_assign(implementation_type &my_impl, implementation_type &other_impl) noexcept -> void;

        static bool empty(implementation_type const &impl);
    };

}


#endif //DUNGEON_BUFFERS_SERVICE_HPP
