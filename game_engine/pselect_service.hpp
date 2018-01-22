//
// Created by Richard Hodges on 21/01/2018.
//

#pragma once

#include "executor.hpp"
#include <system_error>
#include <functional>
#include <list>
#include <chrono>
#include "game_engine/utility/non_copyable.hpp"
#include "game_engine/io_object.hpp"

namespace game_engine {
    struct pselect_service : service_base<pselect_service>
    {
        using service_base<pselect_service>::service_base;
        using clock_type = std::chrono::system_clock;
        using time_point = clock_type::time_point;

        enum fd_state
        {
            read = 0x01,
            write = 0x02,
            except = 0x03
        };

        struct fd_listener_implementaton : utility::non_copyable
        {
            using fd_state = pselect_service::fd_state ;
            using closure_type = std::function<void(std::error_code, int , fd_state)>;

            fd_listener_implementaton(closure_type&& closure) : closure_(closure) {}

            int fd_;
            fd_state state_mask_;
            closure_type closure_;
        };

        struct timeout_listener_implementation
        {

        };

        struct deleter
        {
            deleter(pselect_service *service_observer)
                : service_observer_(service_observer)
            {

            }

            template<class T>
            void operator()(T *p) const noexcept
            {
                service_observer_->notify_destroy(*p);
                delete p;
            }

            pselect_service *service_observer_;
        };


        using fd_listener_implementaton_type = std::unique_ptr<fd_listener_implementaton, deleter>;
        using timeout_listener_implementaton_type = std::unique_ptr<fd_listener_implementaton, deleter>;


        auto construct_fd_listener(int fd, fd_state state_mask) -> fd_listener_implementaton_type ;
        auto construct_timeout_listener(time_point when) -> fd_listener_implementaton_type ;


        using fd_state_closure = std::function<void(std::error_code, int fd, fd_state reasons)>;

        struct fd_state_action
        {
            int fd;
            fd_state state_mask;
            fd_state_closure closure;
        };

        struct clock_action
        {


        };

        void block_and_wait();

    private:
        friend deleter;

        void notify_destroy(fd_listener_implementaton &impl);

        void notify_destroy(timeout_listener_implementation &impl);

        std::list<fd_state_action> pending_fd_actions_;

    };

    struct pselect_fd_listener_impl
    {
        pselect_fd_listener_impl(int fd, pselect_service::fd_state state_mask)
            : fd_(fd), state_mask_(state_mask)
        {}


        int fd_;
        pselect_service::fd_state state_mask_;
    };

    struct pselect_fd_listener_service : unique_impl_service<pselect_fd_listener_service, pselect_fd_listener_impl>
    {
        pselect_fd_listener_service(executor& owner)
            : unique_impl_service<pselect_fd_listener_service, pselect_fd_listener_impl>::unique_impl_service(owner)
            , pselect_service_(use_service<pselect_service>(owner))
        {}

        pselect_service& pselect_service_;
    };

    struct pselect_fd_listener : io_object<pselect_fd_listener_service>
    {
        pselect_fd_listener(executor& owner, int fd, pselect_service::fd_state state_mask)
            : io_object<pselect_fd_listener_service>::io_object(owner, fd, state_mask)
        {}

    };

}


