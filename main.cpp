#include <iostream>
#include <sstream>
#include <vector>

#include "monster_types/all.hpp"
#include "game_engine/game_engine.hpp"


int main() {

    auto larry = Monster(Lich("Larry"));

    auto raiding_party = std::vector<Monster>();
    build_party(raiding_party,
                Goblin("Gobbo"),
                Goblin("Grunt"),
                Goblin("Goober"),
                Lich("Traitorous Bastard"));

    handle_encounter(larry, raiding_party);

    auto executor = game_engine::executor();

    auto keypress_listener = game_engine::keypress_listener(executor);
    auto deadline_timer = game_engine::deadline_timer(executor);

    return 0;
}