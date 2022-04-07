/*
 * File: /src/lib/components/cu/cu_constants.h
 * Project: mpp-cpu
 * File Created: Sunday, 3rd April 2022 1:56:22 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Sunday, 3rd April 2022 1:57:00 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */
#ifndef cu_constants_h
#define cu_constants_h
#include "cu.h"

const OpState s0 = {.id = S0,
                    .load_bits = {
                        .sel_alu = -1,
                        .mx_reg = -1,
                        .sel_dir = 00,
                        // .i_d = -1, // <== documentation says this
                        .i_d = 01,  // <== fix
                        .l_e = 1,
                        .reg_bus = 0,
                        .mem_bus = 1,
                        .fl_bus = 0,
                        .ac_bus = 0,
                        .alu_bus = 0,
                        .pch_bus = 0,
                        .pcl_bus = 0,
                        .ri_car = 1,
                        .pc_car = 1,
                        .reg_car = 0,
                        .ac_car = 0,
                        .op2_car = 0,
                        .sel_fl = -1,
                        .fl_car = 0,
                        .h_car = 0,
                        .l_car = 0,
                        .pch_car = 0,
                        .pcl_car = 0,
                        .sp_car = 0,
                    }};
const OpState s1 = {.id = S1,
                    .load_bits = {
                        .sel_alu = -1,
                        .mx_reg = -1,
                        .sel_dir = -1,
                        .i_d = -1,
                        .l_e = 1,
                        .reg_bus = 0,
                        .mem_bus = 0,
                        .fl_bus = 0,
                        .ac_bus = 0,
                        .alu_bus = 0,
                        .pch_bus = 0,
                        .pcl_bus = 0,
                        .ri_car = 0,
                        .pc_car = 0,
                        .reg_car = 0,
                        .ac_car = 0,
                        .op2_car = 0,
                        .sel_fl = -1,
                        .fl_car = 0,
                        .h_car = 0,
                        .l_car = 0,
                        .pch_car = 0,
                        .pcl_car = 0,
                        .sp_car = 0,
                    }};
// TODO all the other OpState
const OpState s2 = {.id = S2};
const OpState s3 = {.id = S3};
const OpState s4 = {.id = S4};
const OpState s5 = {.id = S5};
const OpState s6 = {.id = S6};
const OpState s7 = {.id = S7};
const OpState s8 = {.id = S8};
const OpState s9 = {.id = S9};
const OpState s10 = {.id = S10};
const OpState s11 = {.id = S11};
const OpState s12 = {.id = S12};
const OpState s13 = {.id = S13};
const OpState s14 = {.id = S14};
const OpState s15 = {.id = S15};
const OpState s16 = {.id = S16};
const OpState s17 = {.id = S17};
const OpState s18 = {.id = S18};
const OpState s19 = {.id = S19};
const OpState s20 = {.id = S20};
const OpState s21 = {.id = S21};
const OpState s22 = {.id = S22};
const OpState s23 = {.id = S23};
const OpState s24 = {.id = S24};
const OpState s25 = {.id = S25};
const OpState s26 = {.id = S26};
const OpState s27 = {.id = S27};
const OpState s28 = {.id = S28};
const OpState s29 = {.id = S29};
const OpState s30 = {.id = S30};
const OpState s31 = {.id = S31};
const OpState s32 = {.id = S32};
const OpState s33 = {.id = S33};
const OpState s34 = {.id = S34};
const OpState s35 = {.id = S35};
const OpState s36 = {.id = S36};
const OpState s37 = {.id = S37};
const OpState s38 = {.id = S38};
#endif