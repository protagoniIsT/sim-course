#pragma once
#include <stdexcept>
#include <string>

class MemoryRegionAccessFault : public std::runtime_error {
public:
    explicit MemoryRegionAccessFault(const std::string& msg)
        : std::runtime_error(msg) {}
};
