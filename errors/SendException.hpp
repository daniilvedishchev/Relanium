#pragma once

#include <stdexcept>
#include <string>

class SendException : public std::runtime_error {
    public:
        explicit SendException (const std::string& message) : std::runtime_error(message) {}
};