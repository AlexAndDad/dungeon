//
// Created by Richard Hodges on 16/02/2018.
//

#pragma once

#include "config.hpp"

namespace opengl {

    /// A polymorphic context base. Any window systems' window/screen object may derive from a context.
    /// In doing so, it is advertising that it suports an opengl rendering context. opengl assets will
    /// be created in this rendering context when it is the current one
    struct context
    {
        /// Make this context object the current rendering context
        void select();

        bool operator==(context const& other) const;

        /// Test whether this rendering context is the current one
        bool is_current() const;

        virtual ~context() = default;

    private:

        virtual bool is_equal(context const& other) const = 0;
        virtual void handle_select() = 0;

        static context* current_context_;
    };
}