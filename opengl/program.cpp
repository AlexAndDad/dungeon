//
// Created by Richard Hodges on 27/01/2018.
//

#include "program.hpp"
#include <iomanip>

namespace opengl
{

    auto program::get_binary() const -> binary
    {
        GLint length = 0;
        glGetProgramiv(get_implementation(), GL_PROGRAM_BINARY_LENGTH, &length);
        auto result = binary();
        auto prep = result.prepare(std::size_t(length));
        glGetProgramBinary(get_implementation(), prep.size, nullptr, prep.format_ptr, prep.buffer_ptr);
        check_errors("glGetProgramBinary");
        return result;
    }

    program::binary::binary()
    : format_(0)
    , data_()
    {}

    auto program::binary::prepare(std::size_t length) -> prepare_args
    {
        data_.resize(length);
        return {
                std::addressof(format_),
                data_.data(),
                GLsizei(length)
        };
    }

    void program::binary::report(std::ostream& os) const
    {
        os << "format: " << format_;
        os << "\nlength: " << data_.size();
        os << "\ndata: ";
        os << std::hex;
        for (auto&& b: data_)
        {
            os << std::setw(2) << std::setfill('0') << std::uint32_t(b);
        }
        os << '\n';
    }

}