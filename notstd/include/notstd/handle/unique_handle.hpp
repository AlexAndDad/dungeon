//
// Created by Richard Hodges on 15/02/2018.
//

#ifndef NOTSTD_UNIQUE_HANDLE_HPP
#define NOTSTD_UNIQUE_HANDLE_HPP

#include <notstd/config.hpp>
#include <notstd/apply.hpp>
#include <utility>
#include <type_traits>


namespace notstd {

#ifdef NOTSTD_HANDLE_DOXYGEN_IGNORE
    /// @defgroup concepts Concepts
    ///

    /// Description of the HandleService Concept
    /// @ingroup concepts
    struct HandleService
    {
        /// The author of a concept will define the type of the underlying native handle. Usually an int or GLuint etc
        using native_handle_type = ...;

        /// Construct a native handle and return it.
        /// @param args is some set of arguments to use for construction - forwarded by a unique_handle constructor
        /// @tparam Args is a type pack of constructor arguments
        ///
        template<class...Args>
        auto construct(Args&&...args) -> native_handle_type;


    };

#else


#endif

    template<class HandleService>
        struct stateless_service
        {
            using service_reference = void;
        };

    template<class HandleService>
        struct stateful_service
        {
            using service_reference = HandleService *;
            constexpr static auto to_service_reference(HandleService& s) -> service_reference { return std::addressof(s); }
        };


    template<class HandleService>
        struct handle_service_traits
        {
            using service_reference = typename HandleService::service_reference;
        };

    template
        <
            typename HandleService,
            typename ServiceReference = typename HandleService::service_reference
        >
        struct handle_observer
        {
            using service_type = HandleService;
            using service_reference = ServiceReference;
            using native_handle_type = typename service_type::native_handle_type;

            handle_observer(service_type &service, native_handle_type handle)
                : service_(service_type::to_service_reference(service)), native_(handle)
            {

            }

            auto native_handle() const & -> native_handle_type const &
            { return native_; }

            constexpr auto get_service() const -> service_type &
            { return *service_; }

        protected:
            auto mutable_native_handle() -> native_handle_type &
            { return native_; }

        private:
            service_reference service_;
            native_handle_type native_;
        };

    template<class HandleService>
        struct handle_observer<HandleService, void>
        {
            using service_type = HandleService;
            using native_handle_type = typename service_type::native_handle_type;

            handle_observer(native_handle_type handle)
                : native_(handle)
            { }

            auto native_handle() const & -> native_handle_type const &
            { return native_; }

            constexpr static auto get_service() -> service_type
            { return service_type(); }

        protected:
            auto mutable_native_handle() -> native_handle_type &
            { return native_; }

        private:
            native_handle_type native_;
        };


    template
        <
            typename HandleService,
            typename ServiceReference = typename HandleService::service_reference
        >
        struct unique_handle : handle_observer<HandleService, ServiceReference>
        {
            using observer_type = handle_observer<HandleService, ServiceReference>;
            using service_type = typename observer_type::service_type;
            using service_reference = typename observer_type ::service_reference ;

            template<typename...HandleArguments>
            unique_handle(service_type& service, std::tuple<HandleArguments...> &&construct_args)
                :observer_type(service,
                               apply([&service](auto &&...args)
                                     {
                                         return service.construct(std::forward<decltype(args)>(args)...);
                                     },
                                     std::move(construct_args)))
            {

            }

            unique_handle(unique_handle &&other) noexcept
                : observer_type(other.get_service(), std::move(other.mutable_native_handle()))
            {
                this->get_service().invalidate(other.mutable_native_handle());
            }

            unique_handle &operator=(unique_handle &&other) noexcept
            {
                auto &l = this->mutable_native_handle();
                auto &r = other.mutable_native_handle();

                using std::swap;
                swap(l, r);

                constexpr auto service = this->get_service();
                service.destroy(r);
                service.invalidate(r);
            }

            ~unique_handle()
            {
                this->get_service().destroy(this->mutable_native_handle());
            }
        };


    template<class HandleService>
        struct unique_handle<HandleService, void> : handle_observer<HandleService, void>
        {
            using observer_type = handle_observer<HandleService, void>;

            template<typename...HandleArguments>
            unique_handle(std::tuple<HandleArguments...> &&construct_args)
                : observer_type(apply([this](auto &&...args)
                                      {
                                          return this->get_service().construct(std::forward<decltype(args)>(args)...);
                                      },
                                      std::move(construct_args)))
            {

            }

            unique_handle(unique_handle &&other) noexcept
                : observer_type(std::move(other.mutable_native_handle()))
            {
                this->get_service().invalidate(other.mutable_native_handle());
            }

            unique_handle &operator=(unique_handle &&other) noexcept
            {
                auto &l = this->mutable_native_handle();
                auto &r = other.mutable_native_handle();

                using std::swap;
                swap(l, r);

                constexpr auto service = this->get_service();
                service.destroy(r);
                service.invalidate(r);
            }

            ~unique_handle()
            {
                this->get_service().destroy(this->mutable_native_handle());
            }


        };


}
#endif //DUNGEON_UNIQUE_HANDLE_HPP
