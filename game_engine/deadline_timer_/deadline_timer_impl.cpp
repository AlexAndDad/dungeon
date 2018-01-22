//
// Created by Richard Hodges on 22/01/2018.
//

#include "deadline_timer_impl.hpp"

namespace game_engine {

    deadline_timer_impl::deadline_timer_impl()
        : expiry_time_(time_point::max())
        , active_signals_()
    {


    }

    bool deadline_timer_impl::armed() const
    {
        return not active_signals_.empty();
    }


    void deadline_timer_impl::invoke(std::error_code ec)
    {
        auto sig_set = std::move(active_signals_);
        active_signals_.clear();
        for(auto&& sig : sig_set)
            sig(ec);
    }
}