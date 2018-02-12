//
// Created by Richard Hodges on 27/01/2018.
//

#pragma once

#include "config.hpp"
#include <utility>
#include <vector>

namespace opengl {

    /// Resource Service base implementation
    /// When a resource_object manages its resources (e.g. copy, move, construction, destruction etc) it will
    /// defer to its service object. The service object will be a concrete class derived from this class.
    /// The reason for this is that the service can then handle the details around creating and deleting
    /// GL handle objects and managing their lifetimes.
    /// @tparam DataType is the type used to store the underlying GL handle or collection of handles
    /// @tparam Derived is the concrete service class derived from this class
    template<class Derived, class DataType>
    struct basic_resource_service
    {
        using implementation_type = DataType;

        /// Perform a move-construction of an underlying implementation
        auto move_construct(implementation_type &other_impl) const -> implementation_type
        {
            auto ident = other_impl;
            other_impl = 0;
            return ident;
        }

        /// Perform a move-assignment an underlying implementation
        auto move_assign(implementation_type &my_impl, implementation_type &other_impl) const -> void
        {
            static_cast<Derived const*>(this)->destroy(my_impl);
            my_impl = other_impl;
            other_impl = 0;
        }

        /// Determine whether the implementation is empty,i.e. does not represent a GL handle.
        bool empty(implementation_type const &impl) const
        {
            return not impl;
        }
    };

    /// A specialisation of basic_resource_service which handles a vector of GL handles
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


    /// The base class of any GL resource manager object.
    /// @tparam ServiceType is the concrete service class that provides native handle management for this object type
    template<class ServiceType>
    struct basic_resource_object
    {
        /// The type of service object that manages the underlying handle(s)
        using service_type = ServiceType;

        /// The type of object which stores any implementation data (e.g. native handle[s])
        using implementation_type = typename service_type::implementation_type;

        /// Construct from an arbitrary set of arguments, provided the service_type::construct() call handles them
        template<class...Args>
        basic_resource_object(std::piecewise_construct_t, Args &&...args)
            : impl_(get_service().construct(std::forward<Args>(args)...))
        {

        }

        /// Copies are disallowed
        basic_resource_object(basic_resource_object const &other) = delete;

        /// re-assignment is disallowed
        basic_resource_object &operator=(basic_resource_object const &other) = delete;

        /// move-construct
        basic_resource_object(basic_resource_object &&other)
            : impl_(get_service().move_construct(other.get_implementation()))
        {}

        /// move-assign
        basic_resource_object &operator=(basic_resource_object &&other)
        {
            get_service().move_assign(get_implementation(), other.get_implementation());
        }

        /// Destructors destroy the underlying resource via the service if the native handle is valid
        ~basic_resource_object()
        {
            get_service().destroy(get_implementation());
        }

        /// Provide a means to check whether the underlying handle is valid
        /// @return bool yes or no
        auto empty() const -> bool
        {
            return get_service().empty(get_implementation());
        }

        /// Return a read-only reference to the navtive handle
        auto native_handle() const -> implementation_type const&
        {
            return get_implementation();
        }

    protected:

        /// Provide acces to the underlying handle(s) only to derived classes
        auto get_implementation() & -> implementation_type &
        { return impl_; }

        /// Provide acces to the underlying handle(s) only to derived classes
        auto get_implementation() const & -> implementation_type const &
        { return impl_; }

        /// Provide acces to service type to derived classes
        static auto get_service()
        {
            return service_type();
        }

    private:
        implementation_type impl_;
    };





}
