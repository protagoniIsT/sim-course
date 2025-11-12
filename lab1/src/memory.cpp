#include "memory.h"

Memory::Memory(std::size_t size) : mem(size, 0) {}

word_t Memory::load_word(word_t addr, AccessType acc_type) const {
    if (addr + sizeof(word_t) > mem.size()) {
        std::cerr << "[Memory] Invalid read address: " << addr
                  << " (size=" << mem.size() << ")\n";
        std::abort();
    }

    if (acc_type == AccessType::EXEC && addr >= executable_end)
        throw MemoryRegionAccessFault("[Memory] Execution in non-executable region at addr=" + std::to_string(addr));

    word_t word = 0;
    for (std::size_t i = 0; i < sizeof(word_t); ++i) {
        word |= (mem[addr + i] << (8 * i));
    }
    return word;
}

void Memory::store_word(word_t addr, int64_t value) {
    if (addr + sizeof(word_t) > mem.size()) {
        std::cerr << "[Memory] Invalid write address: " << addr
                  << " (size=" << mem.size() << ")\n";
        std::abort();
    }
    
    for (std::size_t i = 0; i < sizeof(word_t); ++i) {
        mem[addr + i] = (uint8_t)(value >> (8 * i));
    } 
}

std::size_t Memory::load_bin(std::ifstream& in) {
    in.read(reinterpret_cast<char*>(mem.data()), mem.size());
    if (!in && !in.eof()) {
        throw std::runtime_error("[Memory] Error reading file.");
    }
    return in.gcount();
}

void Memory::reset() {
    std::fill(mem.begin(), mem.end(), 0);
}

std::size_t Memory::size() const {
    return mem.size();
}  
