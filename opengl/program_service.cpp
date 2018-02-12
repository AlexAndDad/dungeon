//
// Created by rhodges on 12/02/18.
//

#include "program_service.hpp"
#include "error.hpp"

namespace opengl
{
    auto program_service::construct() -> implementation_type
    {
        auto result = glCreateProgram();
        check_errors("glCreateProgram");
        return result;
    }

    void program_service::destroy(implementation_type& id)
    {
        glDeleteProgram(id);
        id = 0;
    }

}