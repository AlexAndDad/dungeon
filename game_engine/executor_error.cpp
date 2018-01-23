//
// Created by Richard Hodges on 21/01/2018.
//

#include "executor_error.hpp"
#include <string>

namespace game_engine {

    auto executor_category() -> impl_executor_category &
    {
        static impl_executor_category impl;
        return impl;
    }

    auto impl_executor_category::name() const noexcept -> const char *
    {
        return "executor_error";
    }

    auto impl_executor_category::message(int code) const -> std::string
    {
        switch (static_cast<executor_error>(code)) {
            case executor_error::nothing_to_wait_for:
                return "nothing to wait for";
            case executor_error::operation_aborted:
                return "operation aborted";
            case executor_error ::not_started:
                return "not started";
        }
        return "unknown code: " + std::to_string(code);
    }

}