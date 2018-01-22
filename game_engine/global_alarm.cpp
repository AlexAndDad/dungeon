//
// Created by Richard Hodges on 21/01/2018.
//

#include "global_alarm.hpp"
#include <memory>
#include <type_traits>
#include <cassert>
#include <iterator>
#include <numeric>
#include <sys/time.h>


namespace game_engine {
    namespace {
        using storage_type = std::aligned_storage_t<sizeof(global_alarm_service), alignof(global_alarm_service)>;
        int counter;
        storage_type storage;
    }

    global_alarm_service &alarm_service = reinterpret_cast<global_alarm_service &>(storage);

    global_alarm_service_init::global_alarm_service_init()
    {
        if (++counter == 0) {
            new(reinterpret_cast<global_alarm_service *>(std::addressof(storage))) global_alarm_service();
        }
    }

    global_alarm_service_init::~global_alarm_service_init()
    {
        if (--counter == 0) {
            alarm_service.~global_alarm_service();
        }
    }


    void global_alarm_service::notify(time_point time_now)
    {
        assert(signal_buffer_.empty());

        auto later_than_now = [time_now](implementation_observer observer) {
            return observer->when() > time_now;
        };

        auto first = begin(active_alarms_), last = end(active_alarms_);
        auto partition_point = std::stable_partition(first, last, later_than_now);
        for (auto i = partition_point; i != last; ++i) {
            signal_buffer_.push_back((*i)->disarm());
        }
        active_alarms_.erase(partition_point, last);

        notify_signal_buffer(std::error_code());
    }

    void global_alarm_service::notify_signal_buffer(std::error_code ec)
    {
        for (auto &&closure : signal_buffer_)
            closure(std::error_code());
        signal_buffer_.clear();
    }

    auto global_alarm_service::calc_time_to_run() const -> std::chrono::milliseconds
    {
        time_point to_set = time_point::max();

        auto min_if_armed = [](time_point const &x, implementation_observer observer) {
            if (observer->armed())
                return std::min(x, observer->when());
            else
                return x;
        };
        auto when = std::accumulate(begin(active_alarms_), end(active_alarms_), time_point::max(), min_if_armed);
        if (when == time_point::max())
        {
            return std::chrono::milliseconds::max();
        }
        else
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(when - clock_type::now());
        }
    }

    void global_alarm_service::reset_alarm_signal()
    {
        time_point to_set = time_point::max();

        auto min_if_armed = [](time_point const &x, implementation_observer observer) {
            if (observer->armed())
                return std::min(x, observer->when());
            else
                return x;
        };

        struct itimerval new_setting = {};
        auto when = std::accumulate(begin(active_alarms_), end(active_alarms_), time_point::max(), min_if_armed);
        if (when != time_point::max())
        {
            auto diff = std::chrono::duration_cast<std::chrono::microseconds>(to_set - clock_type::now());
            new_setting.it_interval.tv_sec = 0;
            new_setting.it_interval.tv_usec = 0;
            new_setting.it_value.tv_sec = time_t(diff.count() / 1000000);
            new_setting.it_value.tv_usec = suseconds_t (diff.count() % 1000000);
        }
        setitimer(ITIMER_REAL, &new_setting, nullptr);
    }


    // alarm


}