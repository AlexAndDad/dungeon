//
// Created by Richard Hodges on 17/01/2018.
//

#include "goblin.hpp"


bool hates_other(Goblin const&, Monster const&)
{
    // goblins don't hate anyone
    return false;
}

std::string const & monster_type_name(Goblin const&)
{
    static const std::string _ = "goblin";
    return _;
}
