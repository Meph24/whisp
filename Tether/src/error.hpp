#ifndef ERROR_HPP
#define ERROR_HPP

#include <exception>
#include <stdexcept>

using std::runtime_error;

class input_format_error : public runtime_error
{
    using runtime_error::runtime_error;
    using runtime_error::operator=;
};

#endif // ERROR_HPP
