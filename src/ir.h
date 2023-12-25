#pragma once

#include <stdint.h>

static constexpr uint16_t IR_BIT_LENGTH = 38;
static constexpr uint64_t IR_CODE_YELLOW = 0b100100100101010010100100100101001010100;
static constexpr uint64_t IR_CODE_BLUE = 0b1010100101010010100100100101001010100;
static constexpr uint64_t IR_CODE_RED = 0b10010100101010010100100100101001010100;
static constexpr uint64_t IR_CODE_GREEN = 0b10100100101010010100100100101001010100;
static constexpr uint64_t IR_CODE_CHUP = 0b10101010100101010010101010;
static constexpr uint64_t IR_CODE_CHDN = 0b100101010100101010010101010;

extern volatile bool ir_active;
extern volatile bool ir_available;
extern volatile uint64_t ir_code;

extern void ir_init();
