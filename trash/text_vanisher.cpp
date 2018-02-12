//
// Created by Richard Hodges on 22/01/2018.
//

#include "text_vanisher.hpp"
#include <cassert>

text_vanisher_impl::text_vanisher_impl(game_engine::executor &executor, std::string const &initial_text,
                                       std::size_t window_width,
                                       duration total_duration)
    : executor_(executor)
    , timer_(executor)
    , buffer_(build_buffer(initial_text, window_width))
    , total_duration_(total_duration)
{}

std::string text_vanisher_impl::build_buffer(std::string result, std::size_t buffer_width)
{
    auto text_size = result.size();
    if (text_size < buffer_width) {
        auto total_padding = buffer_width - text_size;
        auto left_padding = total_padding / 2;
        auto right_padding = total_padding - left_padding;
        result.insert(0, left_padding, ' ');
        result.insert(result.size(), right_padding, ' ');
    }
    return result;
}

void text_vanisher_impl::cancel_while_running()
{
    assert(state_ == running);
    state_ = complete;
    last_error_ = game_engine::executor_error::operation_aborted;
    timer_.cancel();
    consider_completion_handler();
}

namespace {
    void encroach(std::string &buffer, std::size_t n)
    {
        auto first = std::begin(buffer);
        auto last = std::next(first, n);
        std::fill(first, last, '*');

        auto rfirst = std::rbegin(buffer);
        auto rlast = std::next(rfirst, n);
        std::fill(rfirst, rlast, '*');
    }
}

void text_vanisher_impl::setup(time_point now)
{
    auto steps = (buffer_.size() + 1) / 2;
    auto stop_time = now + total_duration_;

    for (std::size_t step = 0; step <= steps; ++step) {
        auto t = (total_duration_ * step) / steps;
        auto &this_buf = buffer_map_[now + t] = buffer_;
        encroach(this_buf, step);
    }
}

void text_vanisher_impl::start()
{
    switch (state_) {
        case not_started: {
            state_ = running;
            auto now = clock_type::now();
            setup(now);
            tick(now);
        }
            break;
        case running:
        case complete:
            assert(!"invalid state");
            break;
    }

}

void text_vanisher_impl::cancel()
{
    switch (state_) {
        case not_started:
            break;
        case running:
            cancel_while_running();
            break;
        case complete:
            break;
    }
}

void text_vanisher_impl::consider_completion_handler()
{
    switch (state_) {
        case not_started:
            break;
        case running:
            break;
        case complete:
            fire_completion_handler();
            break;
    }
}

void text_vanisher_impl::fire_completion_handler()
{
    auto completion = std::move(completion_handler_);
    completion_handler_ = nullptr;
    if (completion) {
        completion(last_error_);
    }
}

void text_vanisher_impl::tick(time_point now)
{
    assert(state_ == running);
    auto ianim = buffer_map_.upper_bound(now);

    auto out = [now, this](time_point when, std::string const &buffer) {
        if (when > last_output_time_) {
            last_output_time_ = when;
            invoke_output(buffer);
        }
    };

    if (ianim != std::begin(buffer_map_))
        ianim = std::prev(ianim);

    if (ianim != end(buffer_map_)) {
        out(ianim->first, ianim->second);
        ianim = next(ianim);
    }

    if (ianim == end(buffer_map_)) {
        state_ = complete;
        consider_completion_handler();
    } else {
        timer_.expires_at(ianim->first);
        timer_.async_wait([this](std::error_code ec) {
            if (not ec) {
                this->tick(clock_type::now());
            }
        });
    }
}

void text_vanisher_impl::invoke_output(std::string const &buffer)
{
    if (emit_function_) {
        emit_function_(buffer);
    }
}


text_vanisher::text_vanisher(game_engine::executor &executor)
    : executor_(std::addressof(executor))
    , impl_(nullptr)
{}

text_vanisher::text_vanisher(text_vanisher &&other) noexcept
    : text_vanisher(*other.executor_)
{
    std::swap(impl_, other.impl_);
}

text_vanisher &text_vanisher::operator=(text_vanisher &&other) noexcept
{
    auto tmp = std::move(other);
    swap(tmp);
    return *this;
}

text_vanisher::~text_vanisher()
{
    destroy();
}

void text_vanisher::swap(text_vanisher &other) noexcept
{
    using std::swap;
    swap(executor_, other.executor_);
    swap(impl_, other.impl_);
}


auto
text_vanisher::construct(game_engine::executor &executor, std::string const &initial_text, std::size_t window_width,
                         duration d)
-> void
{
    impl_ = new text_vanisher_impl(executor, initial_text, window_width, d);
}

void text_vanisher::destroy() noexcept
{
    if (impl_) {
        impl_->cancel();
        delete impl_;
        impl_ = nullptr;
    }
}