
#include <iostream>
#include "triangle_program.hpp"
#include "opengl/opengl.hpp"
#include <boost/asio.hpp>
#include <boost/utility/string_view.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/noncopyable.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "glfw/glfw.hpp"
#include <GL/glx.h>
#include "freetype/freetype.hpp"
#include "resources/resource_locator.hpp"
#include <fstream>

static constexpr double PI = 3.141592;


const xy_colour vertices[] =
    {
        {{-0.6f, -0.6f}, {1.f, 0.f, 0.f}},
        {{+0.6f, +0.0f}, {0.f, 1.f, 0.f}},
        {{-0.6f, +0.6f}, {0.f, 0.f, 1.f}}
    };

const xy_colour vertices2[] =
    {
        {{-0.6f, -0.6f}, {1.f, 1.f, 0.f}},
        {{-0.6f, +0.6f}, {0.f, 1.f, 0.f}},
        {{+0.6f, +0.6f}, {0.f, 0.f, 1.f}},

        {{-0.6f, -0.6f}, {1.f, 1.f, 0.f}},
        {{+0.6f, -0.6f}, {1.f, 0.f, 1.f}},
        {{+0.6f, +0.6f}, {0.f, 0.f, 1.f}},

    };

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    auto win = glfw::window_observer(window);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        win.set_should_close();
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
    return opengl::shader(opengl::shader_type::vertex, shaders::vertex_shader_glsl);
}

auto make_fragment_shader()
{
    return opengl::shader(opengl::shader_type::fragment, shaders::fragment_shader_glsl);
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
    timer.async_wait([&timer](asio::error_code const &ec)
                     {
                         if (ec == asio::error_code()) {
                             flip = not flip;
                             start_flipper(timer);
                         }
                     });
}

struct program_name_service
{
    using path = boost::filesystem::path;

    void set_program_path(path p)
    {
        program_path_ = std::move(p);
        program_dir_ = program_path_.parent_path();
        program_name_ = program_path_.filename().stem().string();
    }

    path const &program_directory() const
    {
        return program_dir_;
    }

    const std::string &program_name() const
    {
        return program_name_;
    }

private:
    path program_path_;
    path program_dir_;
    std::string program_name_;
};

program_name_service application_globals;

auto load_as_string(boost::filesystem::path const& pathname) -> std::string
try
{
    using namespace std;
    string result;
    ifstream source(pathname.string(), ios::binary | ios::ate | ios::in);
    source.exceptions(ios::failbit);
    auto fileSize = source.tellg();
    source.seekg(0, ios::beg);
    result.resize(fileSize);
    source.read(&result[0], fileSize);
    return result;
}
catch(...)
{
    std::throw_with_nested(std::runtime_error("load_as_string : " + pathname.string()));
}

void run()
{
    auto ft = freetype::library();
    auto face = ft.acquire(resource_locator::fonts() / "A-Bebedera.ttf", 48);
    std::cout << "typeface details:\n" << face << "\n";

    auto g_bitmap = face.get_bitmap('G');
    std::cout << "G Bitmap:\n" << g_bitmap << '\n';

    glfw::library window_session;

    GLuint vertex_buffer;
//    GLint mvp_location, vpos_location, vcol_location;
    auto window = glfw::window(glfw::desktop(), 640, 480, "Simple Window");

    glfwSetKeyCallback(window, key_callback);
    auto&& context = opengl_context(window);
    context.select();

    auto v = opengl::get_version();
    std::cout << "version: " << v.major << ", " << v.minor << std::endl;

    // core profile bullshit
    auto va = opengl::vertex_array();
    bind(va);
    //

    glfwSwapInterval(1);  //1

    asio::io_context executor;
    asio::system_timer timer(executor);
    start_flipper(timer);


    // NOTE: OpenGL error checks have been omitted for brevity
    auto vertex_buf = triangle_buffers(vertices);
    auto vertex_buf2 = triangle_buffers(vertices2);

    auto vertex_shader = make_vertex_shader();
    std::cout << "vertex shader type: " << vertex_shader.type() << "\nsource:\n" << vertex_shader.source();
    std::cout << "\ncompiled: " << vertex_shader.compiled();
    std::cout << "\nlog:\n" << vertex_shader.log() << std::endl;

    auto fragment_shader = make_fragment_shader();

    auto program = triangle_program();

    program.shader_program_.get_binary().report(std::cout);

    auto glyph_vertex_shader = opengl::vertex_shader(load_as_string(resource_locator::fonts() / "vertex_shader.glsl"));
    auto glyph_fragment_shader = opengl::fragment_shader(load_as_string(resource_locator::fonts() / "fragment_shader.glsl"));
    auto glyph_program = opengl::program(std::move(glyph_vertex_shader), std::move(glyph_fragment_shader));
    std::cout << "\nglyph program:\n";
    glyph_program.get_binary().report(std::cout);
    glyph_program.use();

    glActiveTexture(GL_TEXTURE0);
    opengl::texture_2d g_texture;
    g_texture.bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // glyph vbp
    GLuint glyph_vbo;
    glGenBuffers(1, &glyph_vbo);
    auto glyph_attribute_coord = 0;
    glEnableVertexAttribArray(glyph_attribute_coord);
    glBindBuffer(GL_ARRAY_BUFFER, glyph_vbo);
    glVertexAttribPointer(glyph_attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        g_bitmap.width(),
        g_bitmap.height(),
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        g_bitmap.data()
    );
    opengl::check_errors("glTexImage2D");





    while (not window.should_close())
    {
        float ratio;
        int width, height;
        using matrix = glm::mat4;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glDisable(GL_BLEND);
        program.shader_program_.use();
        program.set_z_rotation(float(std::fmod(glfwGetTime(), PI * 2)));
        program.run(orthogonal_matrix(-ratio, ratio, -1.f, 1.f, 1.f, -1.f),
                    flip ? vertex_buf2 : vertex_buf);

//        // render a G
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glyph_program.use();
        glUniform4fv(glyph_program.get_uniform_index("color"), 1, value_ptr(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)));
        opengl::check_errors("glUniform1fv");
//
        float x = 0.5, y = -.5;
        float sx = 0.01, sy = 0.01;
//
        float x2 = x + g_bitmap.x() * sx;
        float y2 = -y - g_bitmap.y() * sy;
        float w = g_bitmap.width() * sx;
        float h = g_bitmap.height() * sy;
//
        GLfloat box[4][4] = {
            {x2,     -y2    , 0, 0},
            {x2 + w, -y2    , 1, 0},
            {x2,     -y2 - h, 0, 1},
            {x2 + w, -y2 - h, 1, 1},
        };
        glBindBuffer(GL_ARRAY_BUFFER, glyph_vbo);
        glEnableVertexAttribArray(glyph_attribute_coord);
        glVertexAttribPointer(glyph_attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            g_bitmap.width(),
            g_bitmap.height(),
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            g_bitmap.data()
        );
        opengl::check_errors("glTexImage2D");
//
        glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        opengl::check_errors("stuff");


        glfwSwapBuffers(window);
        executor.poll();
        glfwPollEvents();
    }
    timer.cancel();
}

// prints the explanatory string of an exception. If the exception is nested,
// recurses to print the explanatory of the exception it holds
void print_exception(const std::exception &e, int level = 0)
{
    std::cerr << std::string(level, ' ') << "exception: " << e.what() << '\n';
    try {
        std::rethrow_if_nested(e);
    }
    catch (const std::exception &e) {
        print_exception(e, level + 1);
    }
    catch (...) { }
}

int main(int argc, const char *argv[])
{
    application_globals.set_program_path(argv[0]);
    resource_locator::initialise(argv[0]);

    namespace po = boost::program_options;
    try {


        po::options_description desc("Command Line Options");
        desc.add_options()
            ("resources,R", po::value<std::string>(), "where to find resource files")
            ("help,?", "display this message");
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
        if (vm.count("help")) {
            std::cout << application_globals.program_name() << " : plays a dungeon game\n"
                "\nusage: " << application_globals.program_name() << " [options]\n\n";
            std::cout << desc << std::endl;
            std::exit(0);
        }

        po::notify(vm);
    }
    catch (std::exception &e) {
        print_exception(e);
        std::exit(100);
    }

    try {
        run();
    }
    catch (std::exception &e) {
        print_exception(e);
        std::exit(4);
    }

    std::exit(EXIT_SUCCESS);
}
