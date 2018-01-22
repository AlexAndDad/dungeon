//
// Created by Richard Hodges on 21/01/2018.
//

#include "deadline_timer.hpp"
#include <cassert>

namespace game_engine {

    void deadline_timer_impl::add_pending_timeout(pending_timeout_ident ident)
    {
        assert(std::find(begin(registered_idents_), end(registered_idents_), ident) == end(registered_idents_));
        registered_idents_.push_back(ident);
    }

    void deadline_timer_impl::remove_pending_timeout(pending_timeout_ident ident)
    {
        assert(std::find(begin(registered_idents_), end(registered_idents_), ident) != end(registered_idents_));
        registered_idents_.erase(std::remove(begin(registered_idents_),
                                             end(registered_idents_), ident),
                                 end(registered_idents_));
    }

}