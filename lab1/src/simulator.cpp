#include "simulator.h"


Simulator::Simulator(std::size_t mem_size, Mode mode_) 
    : mem(mem_size), 
      cpu(0, mem), 
      mode(mode_) {}

void Simulator::load_program(const std::string& path, const std::vector<int32_t>& args) {
    file_size = std::filesystem::file_size(path);

    mem.reset(); 
    cpu.load_bin(path);
    word_t start_pc = mem.load_word(0);
    cpu.reset(start_pc);

    for (std::size_t i = 0; i < args.size(); ++i) {
        cpu.set_reg(i, args[i]);
    }
}

void Simulator::run() {
    while (cpu.do_cycle()) {
        if (cpu.get_pc() >= file_size) {
            cpu.set_reg(8, SyscallCode::EXIT);
            cpu.get_executor().handle_syscall(cpu);
            break;
        }
    }

    if (mode == Mode::DEBUG) {
        cpu.dump();
    }
}
