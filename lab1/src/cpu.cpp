#include "cpu.h"

CPU::CPU(word_t pc_, Memory& memory_) : pc(pc_), memory(memory_) {}

void CPU::load_bin(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        throw std::runtime_error("[CPU] Cannot open file '" + filename + "'");
    }
    memory.load_bin(in);
}

bool CPU::do_cycle() {
    word_t insn = memory.load_word(pc);
    isa::DecodedInstr di{};
    decoder.decode(insn, di);
    bool status = executor.execute(di, *this);
    if (!di.changes_pc) pc += sizeof(word_t);
    return status;
}

int32_t CPU::get_reg(std::size_t index) {
    return regs[index];
}

void CPU::set_reg(std::size_t index, int32_t value) {
    regs[index] = value;
}

word_t CPU::get_pc() {
    return pc;
}

void CPU::set_pc(word_t pc_) {
    pc = pc_;
}

void CPU::update_pc(int32_t offset) {
    pc = static_cast<uint32_t>(static_cast<int64_t>(pc) + offset);
}

Memory& CPU::get_mem() {
    return memory;
}

Executor& CPU::get_executor() {
    return executor;
}

void CPU::reset(word_t pc_) {
    regs.fill(0);
    pc = pc_;
}

void CPU::dump() {
    for (std::size_t i = 0; i < cpu::NUM_REGS; ++i) {
        std::cout << "X" << i << ": " << regs[i] << std::endl;
    }
    std::cout << "PC: " << pc << std::endl;
}
