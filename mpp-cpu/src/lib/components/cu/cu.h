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
#include "states.h"

void init_cu(void);
void shutdown_cu(void);
void run_cu(int);



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

#endif