#pragma once

#include "assembler.h"
#include "memory.h"
#include "cpu.h"
#include <fstream>
#include <iostream>
#include <filesystem>

enum class Mode {
    DEFAULT,
    DEBUG
};

class Simulator {
public:
    explicit Simulator(std::size_t mem_size = memory::MEM_SIZE, Mode mode = Mode::DEFAULT);

    void load_program(const std::string& input_path, const std::vector<int32_t>& args);

    void run();

private:
    Memory mem;
    CPU cpu;
    
    std::string input_path;
    std::string output_path;
    std::vector<int32_t> args;
    Mode mode;
    std::size_t file_size;

    bool handle_syscall();
};