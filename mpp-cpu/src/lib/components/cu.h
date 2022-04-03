/*
 * File: /src/lib/components/cu.h
 * Project: mpp-cpu
 * File Created: Saturday, 2nd April 2022 1:23:02 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Saturday, 2nd April 2022 1:23:04 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#ifndef cu_h
#define cu_h
void init_cu(void);
void shutdown_cu(void);
void run_cu(void);

typedef enum {
    S0,
    S1,
    S2,
    S3,
    S4,
    S5,
    S6,
    S7,
    S8,
    S9,
    S10,
    S11,
    S12,
    S13,
    S14,
    S15,
    S16,
    S17,
    S18,
    S19,
    S20,
    S21,
    S22,
    S23,
    S24,
    S25,
    S26,
    S27,
    S28,
    S29,
    S30,
    S31,
    S32,
    S33,
    S34,
    S35,
    S36,
    S37,
    S38
} OpStateID;

typedef struct {
    int sel_alu;
    int mx_reg;
    int sel_dir;
    int i_d;
    int l_e;
    int reg_bus;
    int mem_bus;
    int fl_bus;
    int ac_bus;
    int alu_bus;
    int pch_bus;
    int pcl_bus;
    int ri_car;
    int pc_car;
    int reg_car;
    int ac_car;
    int op2_car;
    int sel_fl;
    int fl_car;
    int h_car;
    int l_car;
    int pch_car;
    int pcl_car;
    int sp_car;
    int inta_in;
    int eoi;
} StateLoadBits;

typedef struct {
    OpStateID id;
    StateLoadBits load_bits;
} OpState;

typedef struct OpStateTrace {
    OpState state;
    struct OpStateTrace *next;
} OpStateTrace;
#endif