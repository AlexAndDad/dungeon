//
// Created by Richard Hodges on 27/01/2018.
//

#pragma once

#include "config.hpp"
#include <utility>
#include <vector>

namespace opengl {

    template<class Derived, class DataType>
    struct basic_resource_service
    {
        using implementation_type = DataType;

        auto move_construct(implementation_type &other_impl) const -> implementation_type
        {
            auto ident = other_impl;
            other_impl = 0;
            return ident;
        }

        auto move_assign(implementation_type &my_impl, implementation_type &other_impl) const -> void
        {
            static_cast<Derived const*>(this)->destroy(my_impl);
            my_impl = other_impl;
            other_impl = 0;
        }

        bool empty(implementation_type const &impl) const
        {
            return not impl;
        }
    };

    template<class Derived, class DataType>
    struct basic_resource_service<Derived, std::vector<DataType>>
    {
        using implementation_type = std::vector<DataType>;

        auto move_construct(implementation_type &other_impl) const -> implementation_type
        {
            auto ident = std::move(other_impl);
            other_impl.clear();
            return ident;
        }

        auto move_assign(implementation_type &my_impl, implementation_type &other_impl) const -> void
        {
            static_cast<Derived const*>(this)->destroy(my_impl);
            my_impl = std::move(other_impl);
            other_impl.clear();
        }

        bool empty(implementation_type const &impl) const
        {
            return not impl.empty();
        }
    };


    template<class ServiceType>
    struct resource_object
    {
        using service_type = ServiceType;
        using implementation_type = typename service_type::implementation_type;

        template<class...Args>
        resource_object(std::piecewise_construct_t, Args &&...args)
            : impl_(get_service().construct(std::forward<Args>(args)...))
        {

        }

        resource_object(resource_object const &other) = delete;

        resource_object &operator=(resource_object const &other) = delete;

        resource_object(resource_object &&other)
            : impl_(get_service().move_construct(other.get_implementation()))
        {}

        resource_object &operator=(resource_object &&other)
        {
            get_service().move_assign(get_implementation(), other.get_implementation());
        }

        ~resource_object()
        {
            get_service().destroy(get_implementation());
        }

        auto empty() const -> bool
        {
            return get_service().empty(get_implementation());
        }

        auto get_implementation() & -> implementation_type &
        { return impl_; }

        auto get_implementation() const & -> implementation_type const &
        { return impl_; }

        static auto get_service()
        {
            return service_type();
        }

    private:
        implementation_type impl_;
    };





}
