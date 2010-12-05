#pragma once

#include <string>
#include <stdexcept>

#pragma warning(push)
#pragma warning(disable:4996)


namespace mio {

class system_error : std::runtime_error 
{
public:
    static std::string errno_string(int err)
    {
        return std::string(strerror(err));
    }

    system_error(int err, const std::string& msg) : std::runtime_error(msg + ": " + errno_string(err)) {}
};

} //namespace mio

#pragma warning(pop)