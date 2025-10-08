#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "isa/InstructionInfo.h"
#include <unordered_map>

inline void trim(std::string &s) {
    s.erase(0, s.find_first_not_of(" \t\r\n"));
    s.erase(s.find_last_not_of(" \t\r\n") + 1);
};

inline uint32_t parseReg(const std::string &tok) {
    if (tok.empty() || tok[0] != 'X')
        throw std::runtime_error("Expected register, got: " + tok);
    return std::stoi(tok.substr(1));
};

inline std::vector<std::string> tokenize(const std::string &line);

void assemble(std::ifstream &fin, std::ofstream &fout);
