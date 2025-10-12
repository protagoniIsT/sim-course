#pragma once

#include "InstructionDetails.h"
#include "defs.h"
#include <cstdint>
#include <string>
#include <vector>

namespace isa {

    enum class InstrType { R, I, J };

    struct InstructionInfo {
        std::string name;
        uint8_t opcode;
        uint8_t funct6;
        InstrType type;
        isa::InnerOpCode iopcode;
    };

    struct DecodedInstr {
        int32_t opcode;      
        int32_t rd;
        int32_t rs;
        int32_t rt;
        int32_t imm;
        isa::InnerOpCode iopcode;
        bool changes_pc = false;
    };

    const std::vector<InstructionInfo> instruction_set = {
        // R-type
        {"ADD", isa::Opcode::RTYPE, isa::Funct::ADD, InstrType::R, isa::InnerOpCode::ADD},
        {"AND", isa::Opcode::RTYPE, isa::Funct::AND, InstrType::R, isa::InnerOpCode::AND},
        {"BDEP", isa::Opcode::RTYPE, isa::Funct::BDEP, InstrType::R, isa::InnerOpCode::BDEP},
        {"CLS", isa::Opcode::RTYPE, isa::Funct::CLS, InstrType::R, isa::InnerOpCode::CLS},
        {"SYSCALL", isa::Opcode::RTYPE, isa::Funct::SYSCALL, InstrType::R, isa::InnerOpCode::SYSCALL},

        // I-type
        {"SLTI", isa::Opcode::SLTI, 0, InstrType::I, isa::InnerOpCode::SLTI},
        {"ST", isa::Opcode::ST, 0, InstrType::I, isa::InnerOpCode::ST},
        {"STP", isa::Opcode::STP, 0, InstrType::I, isa::InnerOpCode::STP},
        {"RORI", isa::Opcode::RORI, 0, InstrType::I, isa::InnerOpCode::RORI},
        {"SSAT", isa::Opcode::SSAT, 0, InstrType::I, isa::InnerOpCode::SSAT},
        {"BNE", isa::Opcode::BNE, 0, InstrType::I, isa::InnerOpCode::BNE},
        {"BEQ", isa::Opcode::BEQ, 0, InstrType::I, isa::InnerOpCode::BEQ},
        {"LD", isa::Opcode::LD, 0, InstrType::I, isa::InnerOpCode::LD},

        // J-type
        {"J", isa::Opcode::JTYPE, 0, InstrType::J, isa::InnerOpCode::J}};

    inline const InstructionInfo *findByName(const std::string &name) {
        for (auto &info: instruction_set) {
            if (info.name == name)
                return &info;
        }
        return nullptr;
    }

    inline const InstructionInfo *findByOpcode(uint8_t opcode, uint8_t funct6) {
        for (auto &info: instruction_set) {
            if (info.opcode == opcode && (info.type != InstrType::R || info.funct6 == funct6))
                return &info;
        }
        return nullptr;
    }

} // end namespace isa
