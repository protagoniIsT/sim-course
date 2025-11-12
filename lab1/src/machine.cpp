#include "machine.h"

Machine::Machine(std::size_t mem_size) 
    : executor(),
      memory(mem_size),
      cpu(0, executor, memory) {}

std::size_t Machine::load_bin(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        throw std::runtime_error("[Machine] Cannot open file '" + filename + "'");
    }
    return memory.load_bin(in);
}

void Machine::reset(word_t pc) {
    cpu.reset(pc);
    memory.reset();
}

ExecStatus Machine::do_cycle() {
    return cpu.do_cycle();
}

void Machine::dump() const {
    for (std::size_t i = 0; i < cpu::NUM_REGS; ++i) {
        std::cout << "X" << i << ": " << cpu.get_reg(i) << std::endl;
    }
    std::cout << "PC: " << cpu.get_pc() << std::endl;
}