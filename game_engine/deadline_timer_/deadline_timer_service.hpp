//
// Created by Richard Hodges on 22/01/2018.
//

#pragma once

#include "game_engine/executor_error.hpp"
#include "game_engine/service.hpp"
#include "game_engine/deadline_timer_/deadline_timer_impl.hpp"
#include "game_engine/deadline_timer_/deadline_timer_impl_observer.hpp"

#include <unordered_set>

namespace game_engine {
    struct deadline_timer_service : unique_impl_service<deadline_timer_service, deadline_timer_impl>
    {
        using base_class = unique_impl_service<deadline_timer_service, deadline_timer_impl>;

        using clock_type = implementation_class::clock_type;
        using time_point = implementation_class::time_point;
        using duration = clock_type ::duration;
        using timeout_handler_function = implementation_class::timeout_handler_function;

        deadline_timer_service(executor &owner)
            : base_class(owner)
        {}

        // io_object interface

        implementation_type construct()
        {
            auto impl = base_class::construct();
            active_impls_.emplace(impl);
            return impl;
        }

        void on_destroy(implementation_class& impl)
        {
            active_impls_.erase(impl);
            cancel(impl);
        }

        // general purpose interface

        static auto now() -> time_point
        {
            return clock_type::now();
        }

        // client interface

        void cancel(implementation_class& impl)
        {
            impl.invoke(executor_error::operation_aborted);
        }

        void expires_at(implementation_class & impl, time_point when)
        {
            impl.set_expiry(when);
        }

        template<class CompletionHandler>
        void async_wait_until(implementation_class &impl, time_point deadline, CompletionHandler &&handler)
        {
            impl.add_signal(std::forward<CompletionHandler>(handler));
            impl.set_expiry(deadline);
        }

        template<class CompletionHandler>
        void async_wait(implementation_class &impl, CompletionHandler &&handler)
        {
            impl.add_signal(std::forward<CompletionHandler>(handler));
        }

        // inter-service interface

        /// @return a tuple<bool, duration> in which the bool is false if there is no deadline active.
        /// If the bool is true, then the duration element is valid and indicates the number of clock-ticks
        /// until the deadline.
        /// This function will not return a negative duration. return values are clipped to 0 on the lower
        /// bound
        auto time_to_earliest_deadline() const -> std::tuple<bool, duration>;

        void notify_time(time_point now_time = now());

    private:

        std::unordered_set<deadline_timer_impl_observer> active_impls_;
    };

}
