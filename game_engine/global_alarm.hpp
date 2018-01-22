//
// Created by Richard Hodges on 21/01/2018.
//

#pragma once

#include <map>
#include <chrono>
#include <functional>
#include <vector>
#include <system_error>
#include <list>
#include <cassert>

namespace game_engine {

    struct global_alarm_impl
    {
        using alarm_handler_sig = void(std::error_code);
        using alarm_handler_function = std::function<alarm_handler_sig>;

        using clock_type = std::chrono::system_clock;
        using time_point = clock_type::time_point;

        bool armed() const
        {
            return bool(signal_);
        }

        void set_deadline(time_point when)
        {
            when_ = when;
        }

        template<class Handler>
        void arm(time_point when, Handler &&handler)
        {
            assert(not armed());
            when_ = when;
            signal_ = alarm_handler_function(std::forward<Handler>(handler));
        }

        time_point const &when() const
        {
            return when_;
        }

        alarm_handler_function disarm()
        {
            assert(armed());
            auto result = std::move(signal_);
            signal_ = nullptr;
            return result;
        }

        time_point when_;
        alarm_handler_function signal_;
    };

    struct global_alarm_service
    {
        using clock_type = global_alarm_impl::clock_type;
        using time_point = global_alarm_impl::time_point;
        using implementation_class = global_alarm_impl;
        using alarm_handler_function = implementation_class::alarm_handler_function;

        struct deleter
        {
            deleter(global_alarm_service *service) : service_(service)
            {}

            void operator()(global_alarm_impl *p) const noexcept
            {
                service_->on_destroy(*p);
                delete p;
            }

            global_alarm_service *service_;
        };

        using implementation_type = std::unique_ptr<implementation_class, deleter>;
        using implementation_observer = implementation_class *;


        implementation_type construct()
        {
            return implementation_type {
                new implementation_class,
                deleter(this)
            };
        }


        void on_destroy(implementation_class &impl)
        {
            cancel(impl);
        }

        void set_deadline(implementation_type &impl, time_point when)
        {
            impl->set_deadline(when);
            if (impl->armed()) {
                reset_alarm_signal();
            }
        }

        template<class Handler>
        void async_wait(global_alarm_impl &impl, time_point when, Handler &&handler)
        {
        }

        void cancel(implementation_class &impl)
        {

        }

        static time_point now()
        {
            return clock_type::now();
        }

        /// notify the alarm service as to what time it is
        /// This allows the alarm service to fire any required alarm clock notifications
        void notify(time_point time_now);

        auto calc_time_to_run() const -> std::chrono::milliseconds;

        static bool valid_duration(std::chrono::milliseconds const& d)
        {
            return d != std::chrono::milliseconds::max();
        }

    private:


        void notify_signal_buffer(std::error_code ec);

        void reset_alarm_signal();

        std::vector<implementation_observer> active_alarms_;
        std::vector<alarm_handler_function> signal_buffer_;
    };

    struct global_alarm_service_init
    {
        global_alarm_service_init();

        ~global_alarm_service_init();
    };

    extern global_alarm_service &alarm_service;
    static global_alarm_service_init alarm_service_init;

    struct global_alarm
    {
        using service_type = global_alarm_service;
        using implementation_type = service_type::implementation_type;

        using time_point = service_type::time_point;

        global_alarm()
            : impl_(get_service().construct())
        {}

        void set_deadline(time_point when)
        {
            get_service().set_deadline(get_implementation(), when);
        }

        template<class Handler>
        void async_wait(Handler &&handler)
        {
            alarm_service.async_wait(impl_, std::forward<Handler>(handler));
        }

        void cancel()
        {
            alarm_service.cancel(*impl_);
        }

        implementation_type &get_implementation()
        {
            return impl_;
        }

        static auto get_service() -> global_alarm_service &
        {
            return alarm_service;
        }

        implementation_type impl_;
    };


}

