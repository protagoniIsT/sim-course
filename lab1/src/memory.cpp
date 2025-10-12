#include "memory.h"

Memory::Memory(std::size_t size) : mem(size, 0) {}

word_t Memory::load_word(word_t addr) const {
    if (addr + 3 >= mem.size()) {
        std::cerr << "[Memory] Invalid read address: " << addr
                  << " (size=" << mem.size() << ")\n";
        std::abort();
    }
    return mem[addr] |
          (mem[addr + 1] << 8)  |
          (mem[addr + 2] << 16) |
          (mem[addr + 3] << 24);
}

void Memory::store_word(word_t addr, int64_t value) {
    mem[addr] = (uint8_t)(value);
    mem[addr + 1] = (uint8_t)(value >> 8);
    mem[addr + 2] = (uint8_t)(value >> 16);
    mem[addr + 3] = (uint8_t)(value >> 24);
}

void Memory::load_bin(std::ifstream& in) {
    in.read(reinterpret_cast<char*>(mem.data()), mem.size());
    if (!in && !in.eof()) {
        throw std::runtime_error("[Memory] Error reading file.");
    }
}

void Memory::reset() {
    std::fill(mem.begin(), mem.end(), 0);
}

std::size_t Memory::size() const {
    return mem.size();
}  
