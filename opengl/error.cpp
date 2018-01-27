//
// Created by Richard Hodges on 27/01/2018.
//

#include "error.hpp"
#include <sstream>
#include <iomanip>

namespace opengl {
    struct category_type : system_namespace::error_category
    {
        const char *name() const noexcept override
        { return "opengl"; }

        std::string message(int code) const override;
    };

    std::string category_type::message(int code) const
    {
        switch (static_cast<error>(code)) {
            case error::custom_errors:
                return "custom_errors - you should not see this";
            case error::invalid_operation:
                return "invalid operation";
            case error::invalid_value:
                return "invalid value";
            case error::shader_creation_error:
                return "shader creation error";
        }
        assert(!"incomplete error table");
        return "unknown";
    }

    auto opengl_category() -> system_namespace::error_category const &
    {
        static category_type const _{};
        return _;
    }

    auto make_error_code(error err) -> system_namespace::error_code
    {
        return system_namespace::error_code(static_cast<int>(err), opengl_category());
    }


    thread_local api_refused const *api_refused::what_last_ = nullptr;
    thread_local std::string api_refused::what_string_ = initial_what();

    api_refused::api_refused(const char *context, std::vector<GLenum> &&codes)
        : context_(context)
        , codes_(std::move(codes))
    {
    }

    const char *api_refused::what() const noexcept
    {
        if (what_last_ != this) {
            generate_what();
            what_last_ = this;
        }
        return what_string_.c_str();

    }

    std::string api_refused::initial_what()
    {
        return std::string(256, '*');
    }

    void api_refused::generate_what() const noexcept
    {
        auto safe_str = [](GLubyte const *str) {
            if (str)
                return reinterpret_cast<const char *>(str);
            else
                return "null";
        };

        try {
            std::ostringstream os;
            os << "opengl errors: context=" << context_ << " : ";
            const char *sep = "";
            for (auto &&val : codes_) {
                os << sep << std::hex << std::showbase << std::setw(5) << std::setfill('0') << val;
                os << " - " << safe_str(glewGetErrorString(val));
                sep = ", ";
            }
            auto &&s = os.str();
            what_string_.assign(std::begin(s), std::end(s));
        }
        catch (std::exception &hmm) {
            what_string_.assign(hmm.what());
        }
        catch (...) {
            what_string_.assign("failed to generate what. fatal.");
        }
    }

    static thread_local std::string what_string_;
    std::vector<GLuint> codes_;

    void check_errors(const char* context)
    {
        auto e0 = glGetError();
        if (not e0) return;

        std::vector<GLenum> codes { e0 };
        while(auto en = glGetError())
            codes.push_back(en);

        throw api_refused(context, std::move(codes));
    }

}