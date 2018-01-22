//
// Created by Richard Hodges on 17/01/2018.
//

#include "lich.hpp"
#include "goblin.hpp"
#include <typeindex>
#include <algorithm>

namespace {
    const auto things_that_liches_hate = std::vector<std::type_index> {
            typeid(Goblin)
    };
}

bool hates_other(Lich const &me, Monster const &other) {

    auto i = std::find(begin(things_that_liches_hate), end(things_that_liches_hate),
                       other.as_concept().get_type());
    return i != end(things_that_liches_hate);
}

std::string const & monster_type_name(Lich const&)
{
    static const std::string _ = "lich";
    return _;
}
