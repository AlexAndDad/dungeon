/*
#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>
#include <memory>
#include <thread>

#include "monster_types/all.hpp"
#include "game_engine/game_engine.hpp"

#include "text_vanisher.hpp"

struct counted_interval : std::enable_shared_from_this<counted_interval>
{
    using duration = std::chrono::milliseconds;

    counted_interval(game_engine::executor &exec, duration interval, std::size_t times, std::function<void()> f)
        : executor_(exec)
        , timer_(executor_)
        , interval_(interval)
        , count_(times)
        , handler_(std::move(f))
        , current_time_(std::chrono::system_clock::now())
    {}

    void run()
    {
        prime_next();
    }

    void prime_next()
    {
        if (count_) {
            --count_;
            current_time_ += interval_;
            timer_.expires_at(current_time_);
            timer_.async_wait([self = this->shared_from_this()](std::error_code ec) {
                if (not ec) {
                    self->handler_();
                    self->prime_next();
                }
            });
        }
    }


    game_engine::executor &executor_;
    game_engine::deadline_timer timer_;
    duration interval_;
    std::size_t count_;
    std::function<void()> handler_;
    std::chrono::system_clock::time_point current_time_;
};

void do_every_so_often(game_engine::executor &exec, counted_interval::duration interval, std::size_t times,
                       std::function<void()> f)
{
    std::make_shared<counted_interval>(exec, interval, times, f)->run();
}


int main()
{
    using namespace std::literals;

    game_engine::executor executor{};

    assert(not executor.stopped());

    auto larry = Monster(Lich("Larry"));
    auto raiding_party = std::vector<Monster>();
    build_party(raiding_party,
                Goblin("Gobbo"),
                Goblin("Grunt"),
                Goblin("Goober"),
                Lich("Traitorous Bastard"));
//    executor.post([&] { handle_encounter(larry, raiding_party); });

//    do_every_so_often(executor, 500ms, 5, []() {
//        std::cout << "monster rethinks path\n";
//    });
//
//    do_every_so_often(executor, 1000ms, 3, [&larry, &raiding_party]() {
//        handle_encounter(larry, raiding_party);
//        raiding_party.push_back(Goblin("Another Fucking Goblin"));
//    });

    auto tv = text_vanisher(executor, "Welcome to My Cool Game", 40, 2s);
    tv.set_output([](std::string const &buffer) { std::cout << buffer << std::endl; });
    tv.async_run([](std::error_code err) { std::cout << "done!\n"; });

    while (not executor.stopped()) {
        executor.run();
    }

    return 0;
}
 */
#include <GLFW/glfw3.h>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
