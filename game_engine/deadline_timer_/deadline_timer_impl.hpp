//
// Created by Richard Hodges on 22/01/2018.
//

#pragma once

#include <chrono>
#include <system_error>
#include <functional>
#include <vector>

namespace game_engine {

    struct deadline_timer_impl
    {
        using clock_type = std::chrono::system_clock;
        using time_point = clock_type::time_point;

        using timeout_handler_sig =void(std::error_code);
        using timeout_handler_function = std::function<timeout_handler_sig>;

        deadline_timer_impl();

        time_point expiry() const { return expiry_time_; }

        void set_expiry(time_point when)
        {
            expiry_time_ = when;
        }

        template<class F>
        void add_signal(F&& func);

        bool armed() const;

        /// Invoke all active signals with the given error code.
        /// @param ec is the error code to pass to all signal handlers
        /// @post the timer impl is left inactive
        void invoke(std::error_code ec);

    private:
        time_point expiry_time_;
        std::vector <timeout_handler_function> active_signals_;
    };


    template<class F>
    void deadline_timer_impl::add_signal(F&& func)
    {
        active_signals_.emplace(end(active_signals_), std::forward<F>(func));
    }

}