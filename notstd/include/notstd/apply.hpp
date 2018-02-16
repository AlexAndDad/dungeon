//
// Created by Richard Hodges on 15/02/2018.
//

#pragma once

#include <notstd/config.hpp>
#include <notstd/invoke.hpp>
#include <tuple>

namespace notstd {

    namespace detail {
        template<class F, class Tuple, size_t... I>
        constexpr decltype(auto) apply_impl(F &&f, Tuple &&t, std::index_sequence<I...>)
        {
            return invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...);
        }
    }  // namespace detail

    template<class F, class Tuple>
    constexpr decltype(auto) apply(F &&f, Tuple &&t)
    {
        return detail::apply_impl(
            std::forward<F>(f), std::forward<Tuple>(t),
            std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value > {});
    }
}