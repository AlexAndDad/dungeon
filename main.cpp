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
#include <iostream>
#include "triangle_program.hpp"
#include "opengl/buffers.hpp"
#include <boost/asio.hpp>

static constexpr double PI = 3.141592;



const xy_colour vertices[] =
    {
        {{-0.6f, -0.6f}, {1.f, 0.f, 0.f}},
        {{+0.6f, +0.0f}, {0.f, 1.f, 0.f}},
        {{-0.6f, +0.6f},  {0.f, 0.f, 1.f}}
    };

const xy_colour vertices2[] =
    {
        {{-0.6f, -0.6f}, {1.f, 1.f, 0.f}},
        {{-0.6f, +0.6f}, {0.f, 1.f, 0.f}},
        {{+0.6f, +0.6f},  {0.f, 0.f, 1.f}},

        {{-0.6f, -0.6f}, {1.f, 1.f, 0.f}},
        {{+0.6f, -0.6f}, {1.f, 0.f, 1.f}},
        {{+0.6f, +0.6f},  {0.f, 0.f, 1.f}},

    };

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

inline void mat4x4_ortho(glm::mat4 &M, float l, float r, float b, float t, float n, float f)
{
    M[0][0] = 2.f / (r - l);
    M[0][1] = M[0][2] = M[0][3] = 0.f;

    M[1][1] = 2.f / (t - b);
    M[1][0] = M[1][2] = M[1][3] = 0.f;

    M[2][2] = -2.f / (f - n);
    M[2][0] = M[2][1] = M[2][3] = 0.f;

    M[3][0] = -(r + l) / (r - l);
    M[3][1] = -(t + b) / (t - b);
    M[3][2] = -(f + n) / (f - n);
    M[3][3] = 1.f;
}

inline glm::mat4 orthogonal_matrix(float l, float r, float b, float t, float n, float f)
{
    auto result = glm::mat4(1.0);
    mat4x4_ortho(result, l, r, b, t, n, f);
    return result;
}

auto make_vertex_shader()
{
    return opengl::shader(opengl::shader::type::vertex, shaders::vertex_shader_glsl);
}

auto make_fragment_shader()
{
    return opengl::shader(opengl::shader::type::fragment, shaders::fragment_shader_glsl);
}

namespace asio {
    using namespace boost::asio;
    using namespace boost::system;
}

bool flip = false;

void start_flipper(asio::system_timer &timer)
{
    using namespace std::literals;
    timer.expires_from_now(1s);
    timer.async_wait([&timer](asio::error_code const &ec) {
        if (ec == asio::error_code()) {
            flip = not flip;
            start_flipper(timer);
        }
    });
}

void run()
{
    GLFWwindow *window;
    GLuint vertex_buffer;
//    GLint mvp_location, vpos_location, vcol_location;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    glewInit();
    glfwSwapInterval(1);  //1

    asio::io_context executor;
    asio::system_timer timer(executor);
    start_flipper(timer);


    // NOTE: OpenGL error checks have been omitted for brevity
    auto vertex_buf = triangle_buffers(vertices);
    auto vertex_buf2 = triangle_buffers(vertices2);
//    glGenBuffers(1, &vertex_buffer);
//    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    auto vertex_shader = make_vertex_shader();
    auto fragment_shader = make_fragment_shader();

    auto program = triangle_program();

    while (!glfwWindowShouldClose(window)) {
        float ratio;
        int width, height;
        using matrix = glm::mat4;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        program.set_z_rotation(float(std::fmod(glfwGetTime(), PI * 2)));
        program.run(orthogonal_matrix(-ratio, ratio, -1.f, 1.f, 1.f, -1.f),
                    flip ? vertex_buf2 : vertex_buf);

//        program.set_z_rotation(float(std::fmod(glfwGetTime(), PI * 2 + 0.6)));
//        program.run(orthogonal_matrix(-ratio, ratio, -1.f, 1.f, 1.f, -1.f),
//                    flip ? vertex_buf : vertex_buf2);

        glfwSwapBuffers(window);
        executor.poll();
        glfwPollEvents();
    }
    timer.cancel();
    glfwDestroyWindow(window);
    glfwTerminate();
}

// prints the explanatory string of an exception. If the exception is nested,
// recurses to print the explanatory of the exception it holds
void print_exception(const std::exception &e, int level = 0)
{
    std::cerr << std::string(level, ' ') << "exception: " << e.what() << '\n';
    try {
        std::rethrow_if_nested(e);
    } catch (const std::exception &e) {
        print_exception(e, level + 1);
    } catch (...) {}
}

int main(void)
{
    try {
        run();
    }
    catch (std::exception &e) {
        print_exception(e);
    }
    exit(EXIT_SUCCESS);
}
