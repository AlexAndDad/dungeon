//
// Created by Richard Hodges on 21/01/2018.
//

#include "executor.hpp"

namespace game_engine {

    void executor::implementation::shutdown_services()
    {
        auto first = rbegin(services);
        auto last = rend(services);
        for(auto i = first ; i != last ; ++i)
        {
            (*i)->shutdown_service();
        }
    }

}