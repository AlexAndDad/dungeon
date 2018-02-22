//
// Created by rhodges on 13/02/18.
//

#ifndef DUNGEON_BUFFERS_SERVICE_HPP
#define DUNGEON_BUFFERS_SERVICE_HPP

#pragma once

#include "buffer_implementation.hpp"
#include "basic_resource_object.hpp"

namespace opengl {

    struct buffers_service : notstd::stateless_service<buffers_service>
    {
        using native_handle_type = buffer_implementation;
        using implementation_type = native_handle_type;

        static auto construct(std::size_t N) -> implementation_type;

        static void destroy(implementation_type &impl) noexcept;

        static bool empty(implementation_type const &impl);

        static void invalidate(implementation_type& impl) noexcept;

    };

}


#endif //DUNGEON_BUFFERS_SERVICE_HPP
