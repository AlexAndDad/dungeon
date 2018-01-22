//
// Created by Richard Hodges on 22/01/2018.
//

#include "deadline_timer_service.hpp"

namespace game_engine {

    auto deadline_timer_service::time_to_earliest_deadline() const -> std::tuple<bool, duration>
    {
        bool valid = false;
        time_point when = time_point::max();

        for (auto &&observer : active_impls_) {
            if (observer.armed()) {
                valid = true;
                when = std::min(when, observer.deadline());
            }
        }

        auto time_to_run = std::max(duration(0), when - now());
        return {valid, time_to_run};
    }

    void deadline_timer_service::notify_time(time_point now_time)
    {
        std::vector<deadline_timer_impl_observer> candidates;
        candidates.reserve(active_impls_.size());

        auto has_expired = [now_time](deadline_timer_impl_observer const& candidate)
        {
            return candidate.armed() and candidate.deadline() <= now_time;
        };

        std::copy_if(begin(active_impls_), end(active_impls_),
                     back_inserter(candidates),
                     has_expired);

        for (auto&& candidate : candidates)
        {
            candidate.invoke(std::error_code());
        }

    }

}