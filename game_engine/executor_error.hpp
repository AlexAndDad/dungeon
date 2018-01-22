//
// Created by Richard Hodges on 21/01/2018.
//

#pragma once

#include <system_error>

namespace game_engine {

    enum class executor_error
    {
        nothing_to_wait_for = 1,        // pselect has nothing to wait for
        operation_aborted = 2,          // async operation was aborted by cancel()

    };

    struct impl_executor_category : std::error_category
    {
        virtual auto name() const noexcept -> const char * override;

        virtual auto message(int code) const -> std::string override;
    };

    auto executor_category() -> impl_executor_category &;

    inline auto make_error_code(executor_error err) {
        return std::error_code(static_cast<int>(err), executor_category());
    }

}

namespace std
{
    template<>
    struct is_error_code_enum<::game_engine::executor_error> : std::true_type {};
}