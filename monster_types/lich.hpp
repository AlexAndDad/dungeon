//
// Created by Richard Hodges on 17/01/2018.
//

#pragma once

#include "monster.hpp"

struct Lich
{
    Lich(std::string&& str)
            : name_("Lich Lord " + str)
    {}

    std::string const& name() const {
        return name_;
    }
private:
    std::string name_;
};

bool hates_other(Lich const& me, Monster const& other);

std::string const & monster_type_name(Lich const&);

inline std::string const& name_of(Lich const& me) {
    return me.name();
}