#pragma once

#include "decoder.h"

void execute(const DecodedInsn& di, int32_t* regs, uint8_t* mem, int32_t& pc);
