#pragma once

#include <stdexcept>
#include <string>

class BindException : public std::runtime_error {
    public:
        explicit BindException (const std::string& message) : std::runtime_error(message) {}
};