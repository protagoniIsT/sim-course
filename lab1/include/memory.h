#pragma once
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cstddef>

namespace memory {
    constexpr int MEM_SIZE = 1024;
}

class Memory {
public:
    explicit Memory(std::size_t size);

    int32_t load_word(int32_t addr) const;

    void store_word(int32_t addr, int32_t value);
    
    void load_bin(std::ifstream& in);

    void reset();

    std::size_t size() const;

private:
    std::vector<uint8_t> mem;
};