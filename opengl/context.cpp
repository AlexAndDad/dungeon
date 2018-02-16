//
// Created by Richard Hodges on 16/02/2018.
//

#include "context.hpp"

namespace opengl
{
    context* context::current_context_ = nullptr;

    bool context::is_current() const
    {
        if (current_context_)
        {
            return *this == *current_context_;
        }
        else
        {
            return false;
        }
    }

    bool context::operator==(context const& other) const
    {
        return is_equal(other);
    }

    void context::select()
    {
        if (not is_current())
        {
            handle_select();
        }
    }


}
