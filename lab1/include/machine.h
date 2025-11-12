#pragma once

#include "cpu.h"
#include "executor.h"


class Machine {
public:
    explicit Machine(std::size_t mem_size = memory::MEM_SIZE);

    std::size_t load_bin(const std::string& filename);
    void reset(word_t pc = 0);
    ExecStatus do_cycle();

    CPU& get_cpu() { return cpu; };
    Executor& get_executor() { return executor; } ;
    Memory& get_mem() { return memory; };

    void dump() const;

private:
    Executor executor;
    Memory memory;
    CPU cpu;
};