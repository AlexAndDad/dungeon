//
// Created by Richard Hodges on 16/02/2018.
//

#pragma once

#include "config.hpp"
#include "glew_initialisation.hpp"
#include "context_service.hpp"
#include <memory>
#include <unordered_map>

namespace opengl {


    /// A polymorphic context base. Any window systems' window/screen object may derive from a context.
    /// In doing so, it is advertising that it suports an opengl rendering context. opengl assets will
    /// be created in this rendering context when it is the current one
    struct context
    {
        context()
        : glew_init_()
        {
        }

        /// Make this context object the current rendering context
        void select();

        bool operator==(context const& other) const;

        /// Test whether this rendering context is the current one
        bool is_current() const;

        virtual ~context() = default;

        void GenBuffers(GLsizei n, GLuint* buffers)
        {
            glGenBuffers(n, buffers);
        }

        template<class Service>
            auto use() -> Service&
        {
            auto i = services_.find(Service::service_id());
            if (i == std::end(services_))
            {
                i = services_.emplace(Service::service_id(), std::make_unique<Service>(*this)).first;
            }
            return static_cast<Service&>(*(i->second));
        }

    private:

        glew_initialisation glew_init_;

        using context_service_ptr = std::unique_ptr<context_service>;
        using service_map = std::unordered_map<context_service::identifier, context_service_ptr>;
        service_map services_;

        virtual bool is_equal(context const& other) const = 0;
        virtual void handle_select() = 0;

        static context* current_context_;
    };
}