//
// Created by Richard Hodges on 21/01/2018.
//

#pragma once

#include "io_object.hpp"
#include <functional>

namespace game_engine {

    struct keypress_listener_implementation
    {

    };

    struct keypress_listener_service : unique_impl_service<keypress_listener_service, keypress_listener_implementation>
    {
        using unique_impl_service<keypress_listener_service, keypress_listener_implementation>::unique_impl_service;

        void on_destroy(implementation_class & impl);

    };

    struct keypress_listener : io_object<keypress_listener_service>
    {
        using io_object<keypress_listener_service>::io_object;

    };


}