#pragma once

#include <cstdint>
#include <iostream>
#include "decoder.h"
#include "executor.h"

#define MEM_SIZE 128
#define NUM_REGS 32

typedef enum {
    SIGTERM,
    SIGPRINT,
    SIGEXCEPT
} SyscallCode;

int32_t load_word(const uint8_t* mem, int32_t addr);

void store_word(uint8_t* mem, int32_t addr, int32_t value);