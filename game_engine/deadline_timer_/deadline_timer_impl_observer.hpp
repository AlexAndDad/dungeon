//
// Created by Richard Hodges on 22/01/2018.
//

#pragma once

#include "deadline_timer_impl.hpp"
#include <functional>
#include <memory>

namespace game_engine {
    /// Maintain an observer reference to a deadline_timer_impl
    struct deadline_timer_impl_observer
    {
        deadline_timer_impl_observer(deadline_timer_impl &impl)
            : ptr_(std::addressof(impl))
        {}

        template<class Deleter>
        deadline_timer_impl_observer(std::unique_ptr<deadline_timer_impl, Deleter> const &impl_ptr)
            : ptr_(impl_ptr.get())
        {}

        bool operator==(deadline_timer_impl_observer const &other) const
        {
            return this == std::addressof(other) or ptr_ == other.ptr_;
        }

        friend std::size_t hash_value(deadline_timer_impl_observer const& arg)
        {
            auto hasher = std::hash<deadline_timer_impl *>();
            return hasher(arg.ptr_);
        }

        bool armed() const {
            return ptr_->armed();
        }

        auto deadline() const -> deadline_timer_impl::time_point
        {
            return ptr_->expiry();
        }

        void invoke(std::error_code ec) const
        {
            ptr_->invoke(ec);
        }

        deadline_timer_impl *ptr_;
    };
}

namespace std
{
    template<>
    struct hash<::game_engine::deadline_timer_impl_observer>
    {
        std::size_t operator()(::game_engine::deadline_timer_impl_observer const& arg) const
        {
            return hash_value(arg);
        }
    };
}