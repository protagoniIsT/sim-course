#pragma once
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cstddef>
#include "defs.h"
#include "MemoryRegionAccessFault.h"

enum class AccessType { 
    EXEC, 
    READ 
};

class Memory {
public:
    explicit Memory(std::size_t size);

    word_t load_word(word_t addr, AccessType acc_type) const;

    void store_word(word_t addr, int64_t value);
    
    std::size_t load_bin(std::ifstream& in);

    void reset();

    std::size_t size() const;

    std::size_t get_executable_end() { return executable_end; };
    void set_executable_end(std::size_t end) { executable_end = end; }

private:
    std::vector<uint8_t> mem;
    std::size_t executable_end;
};
