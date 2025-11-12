#include "cpu.h"

CPU::CPU(word_t pc_, Executor& executor_, Memory& memory_) : pc(pc_), executor(executor_), memory(memory_) {}

ExecStatus CPU::do_cycle() {
    word_t insn = memory.load_word(pc, AccessType::EXEC);
    isa::DecodedInstr di{};
    decoder.decode(insn, di);
    ExecStatus status = executor.execute(di, *this);
    if (!di.changes_pc) pc += sizeof(word_t);
    return status;
}

word_t CPU::get_reg(std::size_t index) const {
    return regs[index];
}

void CPU::set_reg(std::size_t index, word_t value) {
    regs[index] = value;
}

word_t CPU::get_pc() const {
    return pc;
}

void CPU::set_pc(word_t pc_) {
    pc = pc_;
}

void CPU::update_pc(word_t offset) {
    pc = static_cast<word_t>(pc + offset);
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
