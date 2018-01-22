//
// Created by Richard Hodges on 21/01/2018.
//

#pragma once

#include <chrono>
#include "io_object.hpp"
#include "executor_error.hpp"
#include "pselect_service.hpp"
#include <map>
#include "global_alarm.hpp"

namespace game_engine {

    struct deadline_timer_impl
    {
        using clock_type = std::chrono::system_clock;
        using time_point = clock_type::time_point;

        using timeout_handler_sig = void(std::error_code);
        using timeout_handler_function = std::function<timeout_handler_sig>;

        using pending_timeout_map = std::multimap<time_point, timeout_handler_function>;

        using pending_timeout_ident = pending_timeout_map::iterator;

        void add_pending_timeout(pending_timeout_ident ident);

        void remove_pending_timeout(pending_timeout_ident ident);

    private:
        std::vector<pending_timeout_ident> registered_idents_;
    };

    struct deadline_timer_service : unique_impl_service<deadline_timer_service, deadline_timer_impl>
    {
        using clock_type = implementation_class::clock_type;
        using time_point = implementation_class::time_point;
        using timeout_handler_function = implementation_class::timeout_handler_function;
        using pending_timeout_map = implementation_class::pending_timeout_map;

        deadline_timer_service(executor &owner)
            : unique_impl_service<deadline_timer_service, deadline_timer_impl>::unique_impl_service(owner)
            , pselect_service_(use_service<pselect_service>(owner))
        {}

        static auto now() -> time_point
        {
            return clock_type::now();
        }

        template<class CompletionHandler>
        void async_wait_until(implementation_class &impl, time_point deadline, CompletionHandler &&handler)
        {
            auto i = pending_timeouts_.emplace(deadline, std::forward<CompletionHandler>(handler));
            impl.add_pending_timeout(i);
        }

        pselect_service &pselect_service_;
        std::multimap<time_point, timeout_handler_function> pending_timeouts_;
    };

    struct deadline_timer : io_object<deadline_timer_service>
    {
        using clock_type = service_type::clock_type;
        using time_point = service_type::time_point;

        deadline_timer(executor &owner)
            : io_object<deadline_timer_service>::io_object(owner)
        {}

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
    };


}