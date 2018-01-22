//
// Created by Richard Hodges on 21/01/2018.
//

#include "pselect_service.hpp"

#include "game_engine/utility/non_copyable.hpp"
#include "executor_error.hpp"
#include "game_engine/global_alarm.hpp"

#include <sys/select.h>
#include <csignal>
#include <vector>
#include <functional>

namespace game_engine {

    namespace {

        struct only_destruct
        {
            template<class T>
            void operator()(T *p) const noexcept
            {
                p->~T();
            }
        };

        template<class Derived, class Struct>
        struct optional_c_struct : utility::non_copyable
        {
            using struct_type = Struct;
            using implementation_type = std::unique_ptr<struct_type, only_destruct>;


            struct_type *to_ptr()
            {
                return ptr_.get();
            }

            struct_type const *to_ptr() const
            {
                return ptr_.get();
            }

            struct_type &acquire()
            {
                return *acquire_ptr();
            }

            struct_type *acquire_ptr()
            {
                auto p = ptr_.get();
                if (not p) {
                    p = new(raw_address()) struct_type{};
                    ptr_.reset(p);
                    static_cast<Derived &>(*this).init_struct(p);
                }
                return p;
            }

        private:

            struct_type *raw_address()
            {
                return std::addressof(reinterpret_cast<struct_type &>(storage_));
            }


        private:
            implementation_type ptr_;
            using storage_type = std::aligned_storage_t<sizeof(struct_type), alignof(struct_type)>;
            storage_type storage_;
        };

        struct designated_fd_set : optional_c_struct<designated_fd_set, fd_set>
        {
            designated_fd_set(pselect_service::fd_state which)
                : which_event_(which)
            {
            }

            bool add(int fd, pselect_service::fd_state valid_mask)
            {
                auto p = acquire_ptr();
                if (valid_mask & which_event_ and not FD_ISSET(fd, p)) {
                    FD_SET(fd, p);
                    return true;
                } else {
                    return false;
                }
            }

            static void init_struct(struct_type *impl)
            {
                FD_ZERO(impl);
            }

            pselect_service::fd_state which_event_;
        };


        struct optional_timespec :
            optional_c_struct<optional_timespec, struct timespec>
        {
            static void init_struct(struct_type *)
            {}

            void set(std::chrono::milliseconds const& d)
            {
                auto& ts = acquire();
                ts.tv_sec = long(d.count() / 1000000);
                ts.tv_nsec = long((d.count() % 1000000) * 1000);
            }
        };

        struct optional_sigmask : optional_c_struct<optional_sigmask, sigset_t>
        {
            void add(int sig)
            {
                sigaddset(acquire_ptr(), sig);
            }

            static void init_struct(struct_type *p)
            {
                sigemptyset(p);
            }
        };

        struct pselect_call
        {
            using clock_type = std::chrono::system_clock;

            struct result
            {
                result(std::error_code err)
                    : indicator_count_(-1)
                    , error_(err)
                    , when_(now())
                {}

                result(int pselect_ret_code)
                    : indicator_count_(pselect_ret_code)
                    , error_(deduce_error(pselect_ret_code))
                    , when_(now())
                {}

                bool interrupted() const
                {
                    return error_ == std::errc::interrupted;
                }

                bool has_io() const {
                    return indicator_count_ > 0;
                }

            private:

                static auto now() -> clock_type::time_point
                {
                    return clock_type::now();
                }

                static std::error_code deduce_error(int retcode)
                {
                    if (retcode < 0)
                        return std::error_code(errno, std::system_category());
                    else
                        return std::error_code();
                }

                int indicator_count_ = 0;
                std::error_code error_;
                std::chrono::system_clock::time_point when_;
            };

            pselect_call()
                : highest_fd_(0)
                , reads_{pselect_service::fd_state::read}
                , writes_{pselect_service::fd_state::write}
                , excepts_{pselect_service::fd_state::except}
                , timeout_{}
                , sigmask_{}
            {
            }

            result operator()()
            {
                auto rp = reads_.to_ptr()
                , wp = writes_.to_ptr()
                , ep = excepts_.to_ptr();
                auto tp = timeout_.to_ptr();

                if (rp or wp or ep or tp) {
                    return result {
                        pselect(highest_fd_ + 1, reads_.to_ptr(), writes_.to_ptr(), excepts_.to_ptr(),
                                timeout_.to_ptr(),
                                sigmask_.to_ptr())
                    };
                } else {
                    return result(std::error_code(executor_error::nothing_to_wait_for));
                }

            }

            void add_fd(int fd, pselect_service::fd_state state_mask)
            {
                auto added = reads_.add(fd, state_mask) or writes_.add(fd, state_mask) or excepts_.add(fd, state_mask);
                if (added) {
                    highest_fd_ = std::max(highest_fd_, fd);
                }
            }

            void set_timeout(std::chrono::milliseconds d)
            {
                timeout_.set(d);
            }

            int highest_fd_;
            designated_fd_set reads_, writes_, excepts_;
            optional_timespec timeout_;
            optional_sigmask sigmask_;

            // result fields;
            int result_count_;
            std::error_code error_;
            std::chrono::system_clock time_now_;
        };
    }

    void pselect_service::block_and_wait()
    {
        pselect_call call_site;
        for (auto &&entry : pending_fd_actions_) {
            call_site.add_fd(entry.fd, entry.state_mask);
        }

        auto ttr = alarm_service.calc_time_to_run();
        if (alarm_service.valid_duration(ttr))
        {
            call_site.set_timeout(ttr);
        }

        auto result = call_site();

        if (result.interrupted() or result.has_io()) {
            alarm_service.notify(alarm_service.now());
        }

    }
}
