#pragma once
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cstddef>
#include "defs.h"

namespace memory {
    constexpr int MEM_SIZE = 1024;
}

class Memory {
public:
    explicit Memory(std::size_t size);

    word_t load_word(word_t addr) const;

    void store_word(word_t addr, int64_t value);
    
    void load_bin(std::ifstream& in);

    void reset();

    std::size_t size() const;

private:
    std::vector<uint8_t> mem;
};