//
// Created by Richard Hodges on 22/01/2018.
//

#pragma once

#include "game_engine/game_engine.hpp"
#include <sstream>
#include <iomanip>
#include <functional>
#include <cassert>
#include <map>
#include <vector>

struct text_vanisher_impl
{
    using clock_type = std::chrono::system_clock;
    using duration = clock_type::duration;
    using time_point = clock_type::time_point;

    text_vanisher_impl(game_engine::executor &executor, std::string const &initial_text, std::size_t window_width,
                       duration total_duration);

    static std::string build_buffer(std::string result, std::size_t buffer_width);

    template<class F>
    void set_output_function(F &&f)
    {
        emit_function_ = std::forward<F>(f);
    }

    template<class CompletionHandler>
    void set_completion_handler(CompletionHandler &&handler)
    {
        this->completion_handler_ = std::forward<CompletionHandler>(handler);
        consider_completion_handler();
    }

    void start();

    void cancel();

    void consider_completion_handler();

private:

    void cancel_while_running();

    void fire_completion_handler();

    void setup(time_point);
    void tick(time_point);

    void invoke_output(std::string const& buffer);

    game_engine::executor &executor_;
    game_engine::deadline_timer timer_;
    std::string buffer_;
    duration total_duration_;
    std::function<void(std::string const &buffer)> emit_function_ = nullptr;
    std::function<void(std::error_code)> completion_handler_ = nullptr;
    std::error_code last_error_ = game_engine::executor_error::not_started;
    enum state_type
    {
        not_started,
        running,
        complete,
    } state_ = not_started;

    using buffer_map = std::map<time_point, std::string>;
    buffer_map buffer_map_;
    time_point last_output_time_ = time_point::min();
};

struct text_vanisher
{
    using duration = text_vanisher_impl::duration;

    text_vanisher(game_engine::executor &executor);

    text_vanisher(game_engine::executor &executor, std::string const &initial_text, std::size_t window_width,
                  duration d)
        : text_vanisher(executor)
    {
        construct(executor, initial_text, window_width, d);
    }

    text_vanisher(text_vanisher &&other) noexcept;

    text_vanisher &operator=(text_vanisher &&other) noexcept;

    ~text_vanisher();

    void swap(text_vanisher &other) noexcept;

    template<class OutputFunction>
    void set_output(OutputFunction &&func)
    {
        assert(impl_);
        auto outfunc = game_engine::make_async_callback(*executor_,
                                                        std::forward<OutputFunction>(func));
        impl_->set_output_function(std::move(outfunc));
    }

    template<class CompletionHandler>
    void async_run(CompletionHandler &&handler)
    {
        assert(impl_);
        auto completion_handler = game_engine::make_async_completion_handler(*executor_,
                                                                             std::forward<CompletionHandler>(handler));
        impl_->set_completion_handler(std::move(completion_handler));
        impl_->start();
    }

private:
    using implementation_type = text_vanisher_impl *;

    auto
    construct(game_engine::executor &executor, std::string const &initial_text, std::size_t window_width, duration d)
    -> void;

    void destroy() noexcept;

    game_engine::executor *executor_;
    implementation_type impl_;
};

