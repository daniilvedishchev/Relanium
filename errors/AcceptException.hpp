#pragma once

#include <stdexcept>
#include <string>

class AcceptException : public std::runtime_error {
    public:
        explicit AcceptException (const std::string& message) : std::runtime_error(message) {}
};