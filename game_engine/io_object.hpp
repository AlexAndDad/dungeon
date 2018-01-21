//
// Created by Richard Hodges on 21/01/2018.
//

#pragma once

#include "executor.hpp"

namespace game_engine
{
    template<class ServiceType>
    struct io_object
    {
        using service_type = ServiceType;
        using implementation_type = typename service_type::implementation_type;

        template<class...Args>
        io_object(executor& owner, Args&&...args)
        : service_(use_service<service_type>(owner))
        , impl_(get_service().create(std::forward<Args>(args)...))
        {

        }


        service_type& get_service() const {
            return service_.get();
        }
    private:

        std::reference_wrapper<service_type> service_;
        implementation_type impl_;
    };
}