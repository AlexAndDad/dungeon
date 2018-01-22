//
// Created by Richard Hodges on 21/01/2018.
//

#include "executor.hpp"
#include "executor_implementation.hpp"
#include "pselect_service.hpp"

namespace game_engine {

    executor_implementation::executor_implementation()
        : services_()
        , pending_closures_()
        , work_count_(0)
        , stopped_(false)
        , pselect_service_(nullptr)
    {

    }

    void executor_implementation::use_core_services(executor &owner_handle)
    {
        pselect_service_ = use_service_ptr(owner_handle, detail::service_tag<pselect_service>());
    }

    void executor_implementation::shutdown_services()
    {
        /// @todo add dependency ordering
        for (auto &&entry : services_)
            entry.second->shutdown_service();
    }

    std::size_t executor_implementation::poll_one()
    {
        std::size_t result = 0;

        if (not stopped_) {
            if (not pending_closures_.empty()) {
                auto closure = std::move(pending_closures_.front());
                pending_closures_.pop_front();
                closure();
                ++result;
            }
            stop_check();
        }
        return result;
    }

    std::size_t executor_implementation::run_one()
    {
        std::size_t result = 0;
        if (not stopped()) {
            if (pending_closures_.empty()) {
                pselect_service_->block_and_wait();
            }
            result = poll_one();
        }
        return result;
    }

    void executor_implementation::stop_check()
    {
        if (pending_closures_.empty() and work_count_ == 0)
            stopped_ = true;
    }

    void executor_implementation::add_work() noexcept
    {
        ++work_count_;
    }

    void executor_implementation::reduce_work() noexcept
    {
        --work_count_;
    }

}