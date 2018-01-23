//
// Created by Richard Hodges on 22/01/2018.
//

#pragma once

#include "executor.hpp"
#include <utility>

namespace game_engine {

    namespace detail {

        template<class F, class Tuple, std::size_t...Is>
        decltype(auto) invoke_impl(F &&f, Tuple &&args, std::index_sequence<Is...>)
        {
            return f(std::get<Is>(std::forward<Tuple>(args))...);
        };
    }

    template<class F, class Tuple>
    decltype(auto) invoke(F &&f, Tuple &&args)
    {
        using tuple_type = std::decay_t<Tuple>;
        constexpr auto size = std::tuple_size<tuple_type>::value;
        return detail::invoke_impl(std::forward<F>(f), std::forward<Tuple>(args), std::make_index_sequence<size>());
    };

    template<class F, class...Args>
    decltype(auto) invoke(F &&f, std::tuple<Args...> &args)
    {
        return detail::invoke_impl(std::forward<F>(f), args);
    };

    template<class ClientHandler>
    auto make_async_completion_handler(executor &exec, ClientHandler &&handler)
    {
        auto wrapper = [work = make_work(exec), client_handler = std::forward<ClientHandler>(handler)](
            auto &&...args) mutable {
            work.get_executor().post([client_handler = std::move(client_handler),
                                         args = std::make_tuple(std::forward<decltype(args)>(args)...)]() mutable {
                invoke(client_handler, std::move(args));
            });
        };
        return wrapper;
    }

    template<class ClientHandler>
    auto make_async_callback(executor &exec, ClientHandler &&handler)
    {
        auto wrapper = [&exec, client_handler = std::forward<ClientHandler>(handler)](auto &&...args) mutable {
            exec.post([client_handler = std::move(client_handler),
                          args = std::make_tuple(std::forward<decltype(args)>(args)...)]() mutable {
                invoke(client_handler, std::move(args));
            });
        };
        return wrapper;
    }

};