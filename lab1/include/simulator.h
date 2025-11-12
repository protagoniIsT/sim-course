#pragma once

#include "machine.h"
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

    void load_program(const std::string& input_path, const std::vector<word_t>& args);
    void set_debug_mode(bool dbg) { mode = dbg ? Mode::DEBUG : Mode::DEFAULT; };
    void run();

private:
    Machine machine;
    Mode mode;
    std::string input_path;
    std::vector<int32_t> args;
};