//
// Created by Richard Hodges on 21/01/2018.
//

#pragma once
#include "service.hpp"

#include <vector>
#include <list>
#include <memory>
#include <deque>
#include <functional>

namespace game_engine
{

    struct executor;
    struct pselect_service;



    struct executor_implementation : utility::non_copyable
    {
        using completion_handler_sig = void();
        using completion_handler = std::function<completion_handler_sig >;

        executor_implementation();

        void use_core_services(executor& owner_handle);

        std::size_t poll_one();

        template<class CompletionHandler>
        void post(CompletionHandler&& handler)
        {
            pending_closures_.emplace_back(std::forward<CompletionHandler>(handler));
        }

        void shutdown_services();

        void add_work() noexcept;
        void reduce_work() noexcept;

        std::size_t run_one();

        bool stopped() const { return stopped_; }

        std::list<std::unique_ptr<service>> services;

    private:

        using closure_type = std::function<void()>;

        void stop_check();

        std::deque<closure_type> pending_closures_;
        std::size_t work_count_;
        bool stopped_;
        pselect_service* pselect_service_;
    };

}