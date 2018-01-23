//
// Created by Richard Hodges on 21/01/2018.
//

#include "deadline_timer.hpp"
#include <cassert>

namespace game_engine {


    void deadline_timer::cancel()
    {
        get_service().cancel(get_impl());
    }
}