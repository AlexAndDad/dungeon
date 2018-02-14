//
// Created by rhodges on 13/02/18.
//

#include "window.hpp"
#include "desktop.hpp"

namespace glfw {


    window::window(desktop const &dt, int width, int height, const char *title)
        : window_observer(get_service().construct_on_desktop(width, height, title))
    {
    }

}