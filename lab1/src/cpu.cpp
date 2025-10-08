#include "cpu.h"

CPU::CPU(int32_t pc_, Memory& memory_) : pc(pc_), memory(memory_) {}

void CPU::load_bin(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        throw std::runtime_error("[CPU] Cannot open file '" + filename + "'");
    }
    memory.load_bin(in);
}

bool CPU::do_cycle() {
    uint32_t insn = memory.load_word(pc);
    isa::DecodedInstr di{};
    decoder.decode(insn, di);
    executor.execute(di, regs, memory, pc);
    if (!di.changes_pc) pc += 4;
    return regs[8] != SyscallCode::EXIT;
}

int32_t CPU::get_reg(std::size_t index) {
    return regs[index];
}

void CPU::set_reg(std::size_t index, int32_t value) {
    regs[index] = value;
}

int32_t CPU::get_pc() {
    return pc;
}

void CPU::set_pc(int32_t pc_) {
    pc = pc_;
}

void CPU::reset(int32_t pc_) {
    regs.fill(0);
    pc = pc_;
}

void CPU::dump() {
    for (std::size_t i = 0; i < cpu::NUM_REGS; ++i) {
        std::cout << "X" << i << ": " << regs[i] << std::endl;
    }
    std::cout << "PC: " << pc << std::endl;
}
