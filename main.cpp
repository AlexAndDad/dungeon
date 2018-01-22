#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>

#include "monster_types/all.hpp"
#include "game_engine/game_engine.hpp"


int main() {


    game_engine::executor executor{};

    assert(not executor.stopped());

    auto larry = Monster(Lich("Larry"));
    auto raiding_party = std::vector<Monster>();
    build_party(raiding_party,
                Goblin("Gobbo"),
                Goblin("Grunt"),
                Goblin("Goober"),
                Lich("Traitorous Bastard"));
    executor.post([&] { handle_encounter(larry, raiding_party); });

    auto timer = game_engine::deadline_timer(executor);
    timer.async_wait_for(std::chrono::seconds(1), [](std::error_code ec)
    {
       std::cout << "timeout\n";
    });

    while (not executor.stopped())
    {
        auto ran = executor.run_one();
        std::cout << "ran: " << ran << '\n';

    }

    auto keypress_listener = game_engine::keypress_listener(executor);
    auto deadline_timer = game_engine::deadline_timer(executor);

    return 0;
}