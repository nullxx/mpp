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

enum SelAluOp { SUM = 0, SUB = 1, AND = 2, OR = 3, XOR = 4, NOT = 5, TRANSPARENT = 6, INCREMENT = 7 };

void init_alu(void);
void shutdown_alu(void);
void run_alu(void);
bool set_selalu_lb(unsigned long bin);
void set_alubus_lb(void);
void reset_alubus_lb(void);

#endif