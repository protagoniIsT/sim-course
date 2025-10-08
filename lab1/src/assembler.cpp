#include "assembler.h"

inline std::vector<std::string> tokenize(std::string &line) {
    line.erase(std::remove(line.begin(), line.end(), ','), line.end());
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string tok;
    while (iss >> tok) tokens.push_back(tok);
    return tokens;
}

void assemble(std::ifstream &fin, std::ofstream &fout) {
    std::string line;

    int32_t pc = 0;

    std::unordered_map<std::string, uint32_t> label_addrs;
    uint32_t currAddr = 4;
    bool inCode = false;

    while (getline(fin, line)) { // analyzing labels
        trim(line);
        if (line.empty() || line[0] == ';') continue;

        if (line == ".data") {
            inCode = false;
            continue;
        }
        if (line == ".text") {
            inCode = true;
            pc = currAddr;
            continue;
        }

        if (!inCode) {
            currAddr += 4;
            continue;
        }

        if (line.back() == ':') {
            std::string name = line.substr(0, line.size() - 1);
            label_addrs[name] = currAddr;
        } else if (line[0] != '.' && line[0] != ';') {
            currAddr += 4;
        }
    }

    fin.clear();
    fin.seekg(0, std::ios::beg);

    fout.write(reinterpret_cast<const char*>(&pc), sizeof(pc));

    uint32_t tisa_pc = pc;

    while (std::getline(fin, line)) {
        trim(line);
        if (line.empty()) continue;
        if (line[0] == ';') continue;
        if (line == ".data") { 
            inCode = false; 
            continue; 
        }
        if (line == ".text") { 
            inCode = true; 
            continue; 
        }

        if (!inCode) {
            try {
                int32_t val = std::stoi(line);
                fout.write(reinterpret_cast<const char*>(&val), sizeof(val));
            } catch (std::invalid_argument&) {
                throw new std::runtime_error("Invalid data value: " + line);
            }
            continue;
        }

        uint32_t encodedInstr = 0;

        auto tokens = tokenize(line);
        if (tokens.empty() || tokens[0].back() == ':') continue;

        const isa::InstructionInfo* instrInfo = isa::findByName(tokens[0]);
        if (!instrInfo) {
            throw new std::runtime_error("Unknown instruction: " + tokens[0]);
        }
        
        switch (instrInfo->type) {
            case isa::InstrType::R: {
                switch (instrInfo->funct6) {
                    case isa_detail::FUNCT6_ADD: 
                    case isa_detail::FUNCT6_AND:
                    case isa_detail::FUNCT6_BDEP: {
                        uint32_t rd = parseReg(tokens[1]);
                        uint32_t rs = parseReg(tokens[2]);
                        uint32_t rt = parseReg(tokens[3]); 

                        encodedInstr |= (instrInfo->opcode & 0x3F) << 26;
                        encodedInstr |= (rs & 0x1F) << 21;
                        encodedInstr |= (rt & 0x1F) << 16;
                        encodedInstr |= (rd & 0x1F) << 11;
                        encodedInstr |= (instrInfo->funct6 & 0x3F);

                        break;
                    }
                    case isa_detail::FUNCT6_CLS: {
                        uint32_t rd = parseReg(tokens[1]);
                        uint32_t rs = parseReg(tokens[2]);

                        encodedInstr |= (instrInfo->opcode & 0x3F) << 26;
                        encodedInstr |= (rd & 0x1F) << 21;
                        encodedInstr |= (rs & 0x1F) << 16;
                        encodedInstr |= (instrInfo->funct6 & 0x3F);

                        break;
                    }
                    case isa_detail::FUNCT6_SYSCALL: {
                        encodedInstr |= (instrInfo->opcode & 0x3F) << 26;
                        encodedInstr |= (instrInfo->funct6 & 0x3F);
                        break;
                    }     
                }
                break;
            }
            case isa::InstrType::I: {
                switch (instrInfo->opcode) {
                    case isa_detail::OPCODE_SLTI: {
                        uint32_t rt = parseReg(tokens[1]);
                        uint32_t rs = parseReg(tokens[2]);
                        int32_t imm = std::stoi(tokens[3]);

                        encodedInstr |= (isa_detail::OPCODE_SLTI & 0x3F) << 26;
                        encodedInstr |= (rs & 0x1F) << 21;
                        encodedInstr |= (rt & 0x1F) << 16;
                        encodedInstr |= (imm & 0xFFFF); 

                        break;
                    }
                    case isa_detail::OPCODE_ST: {
                        auto lparen = tokens[2].find('(');
                        auto rparen = tokens[2].find(')');
                    
                        std::string offset_str = tokens[2].substr(0, lparen);
                        std::string base_str = tokens[2].substr(lparen + 1, rparen - lparen - 1);

                        uint32_t rt = parseReg(tokens[1]);
                        uint32_t base = std::stoi(base_str.substr(1));
                        int32_t offset = std::stoi(offset_str);

                        encodedInstr |= (instrInfo->opcode & 0x3F) << 26;
                        encodedInstr |= (base & 0x1F) << 21;
                        encodedInstr |= (rt & 0x1F) << 16;
                        encodedInstr |= (offset & 0xFFFF);

                        break;
                    }
                    case isa_detail::OPCODE_STP: {
                        auto lparen = tokens[3].find('(');
                        auto rparen = tokens[3].find(')');

                        std::string offset_s = tokens[3].substr(0, lparen);
                        std::string base_s = tokens[3].substr(lparen + 1, rparen - lparen - 1);

                        uint32_t rt1 = parseReg(tokens[1]);
                        uint32_t rt2 = parseReg(tokens[2]);
                        uint32_t base = std::stoi(base_s.substr(1));
                        int32_t offset = std::stoi(offset_s);

                        encodedInstr |= (instrInfo->opcode & 0x3F) << 26;
                        encodedInstr |= (base & 0x1F) << 21;
                        encodedInstr |= (rt1 & 0x1F) << 16;
                        encodedInstr |= (rt2 & 0x1F) << 11;
                        encodedInstr |= (offset & 0x7FF); 

                        break;
                    }
                    case isa_detail::OPCODE_RORI:
                    case isa_detail::OPCODE_SSAT: {
                        uint32_t rd = parseReg(tokens[1]);
                        uint32_t rs = parseReg(tokens[2]);
                        int32_t imm = std::stoi(tokens[3]);

                        encodedInstr |= (instrInfo->opcode & 0x3F) << 26;
                        encodedInstr |= (rd & 0x1F) << 21;
                        encodedInstr |= (rs & 0x1F) << 16;
                        encodedInstr |= ((imm & 0x1F) << 11);

                        break;
                    }
                    case isa_detail::OPCODE_BNE:
                    case isa_detail::OPCODE_BEQ: {
                        uint32_t rs = parseReg(tokens[1]);
                        uint32_t rt = parseReg(tokens[2]);

                        int32_t offset = 0;
                        try {
                            offset = std::stoi(tokens[3]);
                        } catch (std::invalid_argument&) {
                            if (label_addrs.count(tokens[3])) {
                                uint32_t target = label_addrs[tokens[3]];
                                offset = (int32_t)((target - tisa_pc) / 4);
                            } else {
                                std::cerr << "Unknown label: " << tokens[3] << "\n";
                            }
                        }

                        encodedInstr |= (instrInfo->opcode & 0x3F) << 26;
                        encodedInstr |= (rs & 0x1F) << 21;
                        encodedInstr |= (rt & 0x1F) << 16;
                        encodedInstr |= (offset & 0xFFFF);

                        break;
                    }
                    case isa_detail::OPCODE_LD: {
                        auto lparen = tokens[2].find('(');
                        auto rparen = tokens[2].find(')');

                        std::string offset_s = tokens[2].substr(0, lparen);
                        std::string base_s = tokens[2].substr(lparen + 1, rparen - lparen - 1);

                        uint32_t rt = parseReg(tokens[1]);
                        uint32_t base = std::stoi(base_s.substr(1));
                        int32_t offset = std::stoi(offset_s);

                        encodedInstr |= (instrInfo->opcode & 0x3F) << 26;
                        encodedInstr |= (base & 0x1F) << 21;
                        encodedInstr |= (rt & 0x1F) << 16;
                        encodedInstr |= (offset & 0xFFFF);
                        break;
                    }
                }
                break;
            }
            case isa::InstrType::J: {
                std::string target = tokens[1];

                uint32_t index = 0;
                if (std::isdigit(target[0])) {
                    index = std::stoul(target) / 4;
                } else if (label_addrs.count(target)) {
                    index = label_addrs[target] / 4;
                } else {
                    throw std::runtime_error("Unknown label in J: " + target);
                }

                encodedInstr |= (instrInfo->opcode & 0x3F) << 26;
                encodedInstr |= (index & 0x3FFFFFF);

                break;   
            }
        }
        fout.write(reinterpret_cast<const char*>(&encodedInstr), sizeof(encodedInstr));
        tisa_pc += 4;
    }
}