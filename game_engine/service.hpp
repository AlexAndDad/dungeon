//
// Created by Richard Hodges on 21/01/2018.
//

#pragma once

#include <memory>

namespace game_engine {

    struct executor;

    struct service
    {

        struct id;
        using identity_type = id *;

        service(executor& owner);

        virtual void shutdown_service() = 0;

        virtual identity_type get_identity() const = 0;

        executor& get_executor() const {
            return owner_.get();
        }

        std::reference_wrapper<executor> owner_;
    };


    struct service::id
    {
        id()
        {};

        id(id const &) = delete;

        id &operator=(id const &) = delete;
    };

    namespace detail {
        template<class ServiceType>
        struct service_id : service::id
        {

        };
    }

    template<class ServiceType>
    struct service_base : service
    {
        using service_type = ServiceType;

        service_base(executor& owner) : service(owner) {}

        virtual void shutdown_service() override
        {

        }

        virtual identity_type get_identity() const override
        {
            return std::addressof(ident);
        }

        static detail::service_id<service_type> ident;
    };

    template<class ServiceType>
    typename detail::service_id<ServiceType> service_base<ServiceType>::ident;


    template<class ServiceType, class ImplType>
    struct unique_impl_service : service_base<ServiceType>
    {
        using service_type = ServiceType;
        using implementation_class = ImplType;

        unique_impl_service(executor& owner)
            : service_base<service_type>::service_base(owner)
        {}

        struct deleter
        {
            deleter(service_type &service)
                : service_(service)
            {}

            void operator()(implementation_class *pimpl) const noexcept
            {
                service_.get().on_destroy(*pimpl);
                delete pimpl;
            }

            std::reference_wrapper<service_type> service_;
        };

        using implementation_type = std::unique_ptr<implementation_class, deleter>;

        template<class...Args>
        auto create(Args &&...args) -> implementation_type
        {
            auto result = implementation_type
                {
                    new implementation_class(std::forward<Args>(args)...),
                    deleter(static_cast<service_type &>(*this))
                };
            return result;
        }

        void on_destroy(implementation_class &impl)
        {

        }
    };
}


