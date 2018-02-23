//
// Created by rhodges on 23/02/18.
//

#pragma once

#include "config.hpp"
#include <typeindex>

namespace opengl {

    struct context;

    /// The base class of any service exported and owned by a context
    struct context_service
    {
        using identifier = std::type_index;

        context_service(context& owner);

        virtual void stop_service();
        virtual void shutdown_service();
        virtual ~context_service();

        virtual auto get_service_id() const -> identifier  = 0;

        /// Return a reference to the owning context
        auto get_context() const -> context& { return owner_; }

    protected:


    private:

        /// The context which controls the lifetime of this service
        context& owner_;
    };

    template<class Service>
    struct implement_context_service : context_service
    {
        implement_context_service(context& owner)
            : context_service(owner)
        {

        }

        static auto service_id() -> identifier
        {
            return typeid(Service);
        }

        auto get_service_id() const -> identifier override
        {
            return service_id();
        }

    };

}