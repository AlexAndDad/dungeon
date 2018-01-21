//
// Created by Richard Hodges on 21/01/2018.
//

#pragma once

#include "service.hpp"
#include <memory>
#include <vector>
#include "utility/non_copyable.hpp"

namespace game_engine {

    namespace detail {
        template<class ServiceType>
        struct service_tag {};
    }

    struct executor
    : utility::move_assignable
    {

        executor();
        executor(executor&&) = default;
        ~executor();

        template<class ServiceType>
        friend auto use_service(executor &owner) -> ServiceType&;

    private:
        struct implementation;
        using implementation_type = std::unique_ptr<implementation>;

        // private friends interface

        template<class ServiceType>
        auto use_service(detail::service_tag<ServiceType> tag) -> ServiceType&;

        void shutdown_services();

        implementation_type impl_;
    };

    struct executor::implementation : utility::non_copyable
    {
        void shutdown_services();


        std::vector<std::unique_ptr<service>> services;
    };

    template<class ServiceType> ServiceType& use_service(executor& owner)
    {
        return owner.use_service(detail::service_tag<ServiceType>());
    }

    template<class ServiceType>
    auto executor::use_service(detail::service_tag<ServiceType> tag) -> ServiceType&
    {
        auto& services = impl_->services;
        auto matches_identity = [](auto&& service_ptr)
        {
            return service_ptr->get_identity() == std::addressof(ServiceType::ident);
        };
        auto i = find_if(begin(services), end(services), matches_identity);
        if (i == end(services)) {
            i = services.insert(end(services), std::make_unique<ServiceType>(*this));
        }
        return static_cast<ServiceType&>(**i);
    }


}



