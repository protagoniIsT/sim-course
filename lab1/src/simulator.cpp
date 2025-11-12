#include "simulator.h"   

Simulator::Simulator(std::size_t mem_size, Mode mode_)
    : machine(mem_size), mode(mode_) {}

void Simulator::load_program(const std::string& path, const std::vector<word_t>& args) {
    machine.reset(); 
    std::size_t program_size = machine.load_bin(path);
    auto& mem = machine.get_mem();
    mem.set_executable_end(program_size);

    word_t start_pc = mem.load_word(0, AccessType::READ);

    if (start_pc % sizeof(word_t) != 0 || start_pc >= program_size) {
        std::cerr << "[SIM] Invalid entry point: " << start_pc << "\n";
        std::abort();
    }

    machine.get_cpu().reset(start_pc);

    for (std::size_t i = 0; i < args.size(); ++i) {
        machine.get_cpu().set_reg(i, args[i]);
    }
}

void Simulator::run() {
    ExecStatus status = ExecStatus::CONTINUE;

    while (true) {
        try {
            status = machine.do_cycle();
        } catch (const MemoryRegionAccessFault& e) {
            std::cerr << "[SIM] Program terminated without EXIT syscall.\n";
            if (mode == Mode::DEBUG) {
                machine.dump();
            }
            std::abort();
        }

        if (status == ExecStatus::CONTINUE) continue;

        if (status == ExecStatus::EXITED) {
            if (mode == Mode::DEBUG) {
                machine.dump();
            }
            return;
        }

        if (status == ExecStatus::EXCEPT) {
            std::cerr << "[SIM] Program aborted due to exception.\n";
            if (mode == Mode::DEBUG) {
                machine.dump();
            }
            std::abort();
        }

        break;
    }
}
