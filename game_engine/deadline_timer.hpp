//
// Created by Richard Hodges on 21/01/2018.
//

#pragma once

#include <chrono>
#include "io_object.hpp"
#include "executor_error.hpp"
#include "pselect_service.hpp"
#include <map>

#include "deadline_timer_/deadline_timer_service.hpp"


namespace game_engine {

    struct deadline_timer : io_object<deadline_timer_service>
    {
        using clock_type = service_type::clock_type;
        using time_point = service_type::time_point;

        deadline_timer(executor &owner)
            : io_object<deadline_timer_service>::io_object(owner)
        {}

        void expires_at(time_point when)
        {
            get_service().expires_at(get_impl(), when);
        }

        template<class Duration>
        void expires_from_now(Duration d)
        {
            auto when = get_service().now() + d;
            get_service().expires_at(get_impl(), when);
        }

        template<class Duration, class CompletionHandler>
        void async_wait_for(Duration d, CompletionHandler &&handler)
        {
            auto &exec = get_executor();
            auto my_handler =
                [
                    work = make_work(exec),
                    client_handler = std::forward<CompletionHandler>(handler)
                ]
                    (std::error_code ec) {
                    work.get_executor().post([client_handler = std::move(client_handler), ec] {
                        client_handler(ec);
                    });

                };
            auto deadline = get_service().now() + d;
            get_service().async_wait_until(get_impl(), deadline, std::move(my_handler));
        };

        template<class CompletionHandler>
        void async_wait(CompletionHandler &&handler)
        {
            auto &exec = get_executor();
            auto my_handler =
                [
                    work = make_work(exec),
                    client_handler = std::forward<CompletionHandler>(handler)
                ]
                    (std::error_code ec) {
                    work.get_executor().post([client_handler = std::move(client_handler), ec] {
                        client_handler(ec);
                    });

                };
            get_service().async_wait(get_impl(), std::move(my_handler));
        };
    };


}