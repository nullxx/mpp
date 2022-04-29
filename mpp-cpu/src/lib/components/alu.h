/*
 * File: /src/lib/components/alu.h
 * Project: mpp-cpu
 * File Created: Tuesday, 29th March 2022 10:29:02 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Tuesday, 29th March 2022 10:29:04 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#ifndef alu_h
#define alu_h
#include <stdbool.h>

enum SelAluOp { SUM = 0b00, SUB = 0b01, AND = 0b10, OR = 0b11, XOR = 0b100, NOT = 0b101, TRANSPARENT = 0b110, INCREMENT = 0b111 };

void init_alu(void);
void shutdown_alu(void);
void run_alu(void);

#endif