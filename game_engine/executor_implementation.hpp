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
#include <unordered_map>

namespace game_engine {

    struct executor;
    struct pselect_service;


    struct executor_implementation : utility::non_copyable
    {
        using completion_handler_sig = void();
        using completion_handler = std::function<completion_handler_sig>;

        executor_implementation();

        template<class ServiceType>
        auto use_service(executor &owner_handle, detail::service_tag <ServiceType>)-> ServiceType &;

        template<class ServiceType>
        auto use_service_ptr(executor &owner_handle, detail::service_tag <ServiceType>)-> ServiceType *;

        void use_core_services(executor &owner_handle);

        std::size_t poll_one();

        template<class CompletionHandler>
        void post(CompletionHandler &&handler)
        {
            pending_closures_.emplace_back(std::forward<CompletionHandler>(handler));
        }

        void shutdown_services();

        void add_work() noexcept;

        void reduce_work() noexcept;

        std::size_t run_one();

        bool stopped() const
        { return stopped_; }


    private:

        using closure_type = std::function<void()>;

        void stop_check();

        std::unordered_map<service::identifier, std::unique_ptr<service>> services_;
        std::unordered_map<service::identifier, service::identifier> service_dependency_map_;

        std::deque<closure_type> pending_closures_;
        std::size_t work_count_;
        bool stopped_;
        pselect_service *pselect_service_;
    };


    template<class ServiceType>
    auto
    executor_implementation::use_service(executor &owner_handle, detail::service_tag <ServiceType> tag) -> ServiceType &
    {
        auto ident = service_identifier(tag);
        /// @todo service dependency ordering
        auto &candidate = services_[ident];
        if (not candidate) {
            candidate = std::make_unique<ServiceType>(owner_handle);
        }
        return static_cast<ServiceType &>(*candidate);
    }

    template<class ServiceType>
    auto executor_implementation::use_service_ptr(executor &owner_handle,
                                                  detail::service_tag <ServiceType> tag) -> ServiceType *
    {
        return std::addressof(use_service(owner_handle, tag));
    }

}