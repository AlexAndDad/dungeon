//
// Created by rhodges on 13/02/18.
//

#ifndef DUNGEON_WINDOW_SERVICE_HPP
#define DUNGEON_WINDOW_SERVICE_HPP

#include "config.hpp"

namespace glfw {

    struct window_service
    {
        using implementation_type = GLFWwindow *;

        static auto construct_on_desktop(int width, int height, const char *title) -> implementation_type ;

        static void destroy(implementation_type& impl) noexcept ;

        struct per_window_data
        {
        };
    };

}
#endif //DUNGEON_WINDOW_SERVICE_HPP
