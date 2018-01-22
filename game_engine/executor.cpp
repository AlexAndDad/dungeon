//
// Created by Richard Hodges on 21/01/2018.
//

#include "executor.hpp"
#include <iterator>

namespace game_engine {
    executor::executor()
    : impl_{std::make_unique<implementation_class>()}
    {
        impl_->use_core_services(*this);
    }

    executor::~executor()
    {
        if (impl_) {
            shutdown_services();
        }
    }

    void executor::shutdown_services()
    {
        impl_->shutdown_services();
    }

    bool executor::stopped() const
    {
        return impl_->stopped();
    }



    std::size_t executor::poll_one()
    {
        return impl_->poll_one();
    }

    std::size_t executor::run_one()
    {
        return impl_->run_one();
    }

    std::size_t executor::run()
    {
        std::size_t result = 0;
        while (not stopped())
            result += impl_->run_one();
        return result;
    }

    auto make_work(executor& owner) -> executor::work
    {
        return executor::work(owner);
    }


}