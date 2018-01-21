//
// Created by Richard Hodges on 21/01/2018.
//

#include "executor.hpp"
#include <iterator>

namespace game_engine {
    executor::executor()
    : impl_{std::make_unique<implementation>()}
    {}

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





}