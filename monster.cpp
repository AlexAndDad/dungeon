//
// Created by Richard Hodges on 17/01/2018.
//

#include "monster.hpp"
#include <iostream>
#include <iomanip>
#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <sstream>

namespace {
    using monster_ref_vector = std::vector<std::reference_wrapper<Monster>>;
    using monsters_ref_map = std::unordered_map<std::string, monster_ref_vector>;

    monsters_ref_map
    collate(std::vector<Monster> &monsters) {
        monsters_ref_map result;
        for (auto &&m : monsters) {
            result[m.type_name()].push_back(m);
        }
        return result;
    };

    const char *is_are(std::size_t s) {
        if (s == 1) return "is";
        return "are";
    }

    std::string pluralise(std::string s, std::size_t n) {
        if (n != 1)
            s += 's';
        return s;
    }

    template<class Container, class F>
    void list_out(std::ostream &os, Container &&cont, F &&func) {
        auto size = cont.size();
        auto current = cont.begin();
        for (std::size_t i = 0; i < size; ++i, ++current) {
            if (i == 0) os << " ";
            else if (i == size - 1) os << " and ";
            else os << ", ";
            func(os, *current);
        }
    };
}

void handle_encounter(Monster &subject, std::vector<Monster> &the_others) {
    auto collated = collate(the_others);
    std::ostringstream chat;
    auto size = collated.size();

    chat << "fuck, here " << is_are(size);
    list_out(chat, collated, [](std::ostream &os, auto &&entry) {
        auto n = entry.second.size();
        os << n;
        os << " " << pluralise(entry.first, n);
    });
    chat << '.';
    std::vector<std::reference_wrapper<Monster>> liked, disliked;
    for (auto &&other : the_others) {
        if (subject.hates(other))
            disliked.push_back(other);
        else
            liked.push_back(other);
    }

    if (not liked.empty()) {
        chat << " I like";
        list_out(chat, liked, [](std::ostream& os, Monster const& m) {
            os << std::quoted(m.name(), '\'') << " (who is a " << m.type_name() << ")";
        });
        chat << ", but";
    }
    if (not disliked.empty()) {
        chat << " I hate";
        list_out(chat, disliked, [](std::ostream& os, Monster const& m) {
            os << std::quoted(m.name(), '\'') << " (who is a " << m.type_name() << ")";
        });
    }
    chat << '.';

    if (liked.size() < disliked.size())
    {
        chat << " So I will attack.";
    }
    else {
        chat << " So I will stay my hand.";
    }

    std::cout << subject.name() << " says: " << std::quoted(chat.str()) << std::endl;

}
