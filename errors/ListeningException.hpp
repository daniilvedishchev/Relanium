#pragma once

#include <stdexcept>
#include <string>

class ListeningException : public std::runtime_error {
    public:
        explicit ListeningException (const std::string& message) : std::runtime_error(message) {}
};