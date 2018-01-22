//
// Created by Richard Hodges on 21/01/2018.
//

#pragma once

#include "service.hpp"
#include "utility/non_copyable.hpp"
#include "executor_implementation.hpp"
#include <memory>
#include <vector>
#include <functional>
#include <deque>

namespace game_engine {

    struct executor_implementation;

    struct executor
        : utility::non_copyable
    {
        using completion_handler_sig = void();

        struct work;

        executor();

        ~executor();

        std::size_t poll_one();

        /// Post a completion handler to the executor.
        /// It will be executed duriing a call to poll_one, run_one, poll or run()
        /// @tparam CompletionHandler is a type which models completion_handler_sig
        /// @param handler a CompletionHandler to post
        ///
        template<class CompletionHandler>
        void post(CompletionHandler &&handler);

        /// Run a closure if there is one waiting. If not, wait until there is a closure available.
        /// If the executor is stopped and no work is indicated, stop the executor
        /// @returns number of closures that were run
        std::size_t run_one();

        /// Run closures and async events until the executor has no more work
        /// @return the number of closures executed.
        std::size_t run();

        /// Indicate whether the executor has stopped as a result of running out of work;
        /// @returns bool true if stopped, otherwise false
        bool stopped() const;

    private:
        template<class ServiceType>
        friend auto use_service(executor &owner) -> ServiceType &;

        friend work;

        using implementation_class = executor_implementation;
        using implementation_type = std::unique_ptr<implementation_class>;

        // private friends interface

        template<class ServiceType>
        auto use_service(detail::service_tag<ServiceType> tag) -> ServiceType &;

        void shutdown_services();

        implementation_type impl_;
    };

    struct executor::work
    {
        work(executor& owner) noexcept
            : impl_(&owner)
        {
            get_executor().impl_->add_work();
        }

        work(work const& other) noexcept
            : impl_(other.impl_)
        {
            get_executor().impl_->add_work();
        }


        ~work()
        {
            get_executor().impl_->reduce_work();
        }

        executor& get_executor() const {
            return *impl_;
        }

        using implementation_type = executor*;

        implementation_type impl_;
    };

    auto make_work(executor& exec) -> executor::work;


    template<class ServiceType>
    ServiceType &use_service(executor &owner)
    {
        return owner.use_service(detail::service_tag<ServiceType>());
    }


    template<class CompletionHandler>
    void executor::post(CompletionHandler &&handler)
    {
        using completion_handler = executor_implementation::completion_handler;
        static_assert(std::is_constructible<completion_handler, CompletionHandler>(),
                      "not a completion_handler_sig model");
        impl_->post(std::function<completion_handler_sig>(std::forward<CompletionHandler>(handler)));
    }

    template<class ServiceType>
    auto executor::use_service(detail::service_tag<ServiceType> tag) -> ServiceType &
    {
        return impl_->use_service(*this, tag);
    }


}



