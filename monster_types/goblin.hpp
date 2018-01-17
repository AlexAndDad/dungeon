//
// Created by Richard Hodges on 17/01/2018.
//

#pragma once
#include "monster.hpp"

// now let's make some models
// note that they don't derive from anything

struct Goblin
{
    Goblin(std::string&& str)
            : name_(std::move(str))
    {}

    std::string const& name() const {
        return name_;
    }
private:
    std::string name_;
};

bool hates_other(Goblin const&, Monster const&);

std::string const & monster_type_name(Goblin const&);


inline std::string const& name_of(Goblin const& me) {
    return me.name();
}