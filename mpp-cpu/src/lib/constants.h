/*
 * File: /src/lib/constants.h
 * Project: mpp-cpu
 * File Created: Monday, 21st March 2022 11:08:39 am
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Sunday, 10th April 2022 4:03:18 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#ifndef constants_h
#define constants_h

#define MEM_VALUE_SIZE_BITS 8
#define MEM_SIZE_KB 64
#define MEM_START_VALUE 0x00

#define DATA_BUS_SIZE_BITS 8
#define DIR_BUS_SIZE_BITS 16

#define ID_LOAD_BIT_SIZE_BITS 2
#define SELALU_LOAD_BIT_SIZE_BITS 3
#define SELREG_LOAD_BIT_SIZE_BITS 2
#define REGCAR_LOAD_BIT_SIZE_BITS 2
#define SELDIR_LOAD_BIT_SIZE_BITS 2

#define SP_REG_SIZE_BIT 16
#define PCH_REG_SIZE_BIT 8
#define PCL_REG_SIZE_BIT 8
#define PC_REG_SIZE_BIT 16
#define H_REG_SIZE_BIT 8
#define L_REG_SIZE_BIT 8
#define ACUMM_REG_SIZE_BIT 8
#define OP2_REG_SIZE_BIT 8
#define REG_SIZE_BIT 8  // A, B, C, D
#define FLAG_REG_SIZE_BIT 1
#define RI_REG_SIZE_BIT 8

#define FLAGSINTA_BUS_SIZE_BITS 8
#define FLAGSINTA_FC_BUS_BIT_POSITION 0
#define FLAGSINTA_FZ_BUS_BIT_POSITION 7
#define FLAGSINTA_GND_START_BIT_POSITION 1
#define FLAGSINTA_GND_END_BIT_POSITION 5
#define FLAGSINTA_INTA_BUS_BIT_POSITION 6
#define FLAGSINTA_GND_CHAR_VALUE 0

#define MAX_CALC_BIN_SIZE_BITS 16

#define CONSTANT_RUNNING_MODULES_COUNT 2 + 1 /* +1 for the main thread. ALU, PC */

#endif /* constants_h */
