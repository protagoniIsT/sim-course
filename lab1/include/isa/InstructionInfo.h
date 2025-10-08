#pragma once

#include "InstructionDetails.h"
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
        isa_detail::InnerOpCode iopcode;
    };

    struct DecodedInstr {
        int32_t opcode;      
        int32_t rd;
        int32_t rs;
        int32_t rt;
        int32_t imm;
        isa_detail::InnerOpCode iopcode;
        bool changes_pc = false;
    };

    const std::vector<InstructionInfo> instruction_set = {
        // R-type
        {"ADD", isa_detail::OPCODE_RTYPE, isa_detail::FUNCT6_ADD, InstrType::R, isa_detail::InnerOpCode::OP_ADD},
        {"AND", isa_detail::OPCODE_RTYPE, isa_detail::FUNCT6_AND, InstrType::R, isa_detail::InnerOpCode::OP_AND},
        {"BDEP", isa_detail::OPCODE_RTYPE, isa_detail::FUNCT6_BDEP, InstrType::R, isa_detail::InnerOpCode::OP_BDEP},
        {"CLS", isa_detail::OPCODE_RTYPE, isa_detail::FUNCT6_CLS, InstrType::R, isa_detail::InnerOpCode::OP_CLS},
        {"SYSCALL", isa_detail::OPCODE_RTYPE, isa_detail::FUNCT6_SYSCALL, InstrType::R, isa_detail::InnerOpCode::OP_SYSCALL},

        // I-type
        {"SLTI", isa_detail::OPCODE_SLTI, 0, InstrType::I, isa_detail::InnerOpCode::OP_SLTI},
        {"ST", isa_detail::OPCODE_ST, 0, InstrType::I, isa_detail::InnerOpCode::OP_ST},
        {"STP", isa_detail::OPCODE_STP, 0, InstrType::I, isa_detail::InnerOpCode::OP_STP},
        {"RORI", isa_detail::OPCODE_RORI, 0, InstrType::I, isa_detail::InnerOpCode::OP_RORI},
        {"SSAT", isa_detail::OPCODE_SSAT, 0, InstrType::I, isa_detail::InnerOpCode::OP_SSAT},
        {"BNE", isa_detail::OPCODE_BNE, 0, InstrType::I, isa_detail::InnerOpCode::OP_BNE},
        {"BEQ", isa_detail::OPCODE_BEQ, 0, InstrType::I, isa_detail::InnerOpCode::OP_BEQ},
        {"LD", isa_detail::OPCODE_LD, 0, InstrType::I, isa_detail::InnerOpCode::OP_LD},

        // J-type
        {"J", isa_detail::OPCODE_JTYPE, 0, InstrType::J, isa_detail::InnerOpCode::OP_J}};

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
