//
// Created by Richard Hodges on 21/01/2018.
//

#pragma once

namespace game_engine { namespace utility {

        struct non_copyable
        {
            non_copyable() = default;
            non_copyable(non_copyable const&) = delete;
            non_copyable& operator=(non_copyable const&) = delete;
            non_copyable(non_copyable &&) = delete;
            non_copyable& operator=(non_copyable &&) = delete;
        };

        struct move_assignable
        {
            move_assignable() = default;
            move_assignable(move_assignable const&) = delete;
            move_assignable& operator=(move_assignable const&) = delete;
            move_assignable(move_assignable &&) = default;
            move_assignable& operator=(move_assignable &&) = delete;
        };
    }}