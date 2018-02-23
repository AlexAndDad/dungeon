//
// Created by rhodges on 23/02/18.
//

#include "context_service.hpp"

namespace opengl
{
    context_service::context_service(context &owner)
    : owner_(owner)
    {

    }

    context_service::~context_service()
    {

    }

    void context_service::stop_service()
    { }

    void context_service::shutdown_service()
    { }

}