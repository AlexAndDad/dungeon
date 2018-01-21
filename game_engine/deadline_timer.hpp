//
// Created by Richard Hodges on 21/01/2018.
//

#pragma once

#include "io_object.hpp"

namespace game_engine {

    struct deadline_timer_impl
    {

    };

    struct deadline_timer_service : unique_impl_service<deadline_timer_service, deadline_timer_impl>
    {
        deadline_timer_service(executor &exec)
            : unique_impl_service<deadline_timer_service, deadline_timer_impl>::unique_impl_service(exec)
        {}
    };

    struct deadline_timer : io_object<deadline_timer_service>
    {
        deadline_timer(executor& owner)
            : io_object<deadline_timer_service>::io_object(owner)
        {}

    };


}