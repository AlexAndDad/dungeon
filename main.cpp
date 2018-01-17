#include <iostream>
#include <sstream>
#include <vector>

#include "monster_types/all.hpp"

int main() {

    auto larry = Monster(Lich("Larry"));

    std::vector<Monster> raiding_party;
    build_party(raiding_party,
                Goblin("Gobbo"),
                Goblin("Grunt"),
                Goblin("Goober"),
                Lich("Traitorous Bastard"));

    handle_encounter(larry, raiding_party);

    return 0;
}