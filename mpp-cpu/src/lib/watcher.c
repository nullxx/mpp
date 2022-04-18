/*
 * File: /src/lib/watcher.c
 * Project: mpp-cpu
 * File Created: Thursday, 7th April 2022 4:24:11 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Thursday, 7th April 2022 4:24:14 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "watcher.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkedlist.h"
#include "logger.h"
#include "utils.h"

Watchers watchers = {.ACUM = NULL, .B = NULL, .C = NULL, .D = NULL, .E = NULL, .FC = NULL, .FZ = NULL, .H = NULL, .L = NULL, .OP2 = NULL, .RI = NULL, .PCL = NULL, .PCH = NULL, .PC = NULL, .SP = NULL};
Watchers prev_watchers = {
    .ACUM = NULL, .B = NULL, .C = NULL, .D = NULL, .E = NULL, .FC = NULL, .FZ = NULL, .H = NULL, .L = NULL, .OP2 = NULL, .RI = NULL, .PCL = NULL, .PCH = NULL, .PC = NULL, .SP = NULL};

RegisterWatcher **get_register(const char *name) {
    if (strcmp(name, "ACUM") == 0) {
        return &watchers.ACUM;
    } else if (strcmp(name, "B") == 0) {
        return &watchers.B;
    } else if (strcmp(name, "C") == 0) {
        return &watchers.C;
    } else if (strcmp(name, "D") == 0) {
        return &watchers.D;
    } else if (strcmp(name, "E") == 0) {
        return &watchers.E;
    } else if (strcmp(name, "FC") == 0) {
        return &watchers.FC;
    } else if (strcmp(name, "FZ") == 0) {
        return &watchers.FZ;
    } else if (strcmp(name, "H") == 0) {
        return &watchers.H;
    } else if (strcmp(name, "L") == 0) {
        return &watchers.L;
    } else if (strcmp(name, "2OP") == 0) {
        return &watchers.OP2;
    } else if (strcmp(name, "RI") == 0) {
        return &watchers.RI;
    } else if (strcmp(name, "PCL") == 0) {
        return &watchers.PCL;
    } else if (strcmp(name, "PCH") == 0) {
        return &watchers.PCH;
    } else if (strcmp(name, "PC") == 0) {
        return &watchers.PC;
    } else if (strcmp(name, "SP") == 0) {
        return &watchers.SP;
    } else {
        return NULL;
    }
}
void register_watcher(RegisterWatcher *reg_watcher) {
    RegisterWatcher **reg = get_register(reg_watcher->name);
    if (reg == NULL) {
        return;
    }
    reg_watcher->hex_repr = NULL;
    reg_watcher->changed = 0;
    *reg = reg_watcher;
}

void unregister_watcher(RegisterWatcher *reg_watcher) {
    RegisterWatcher **reg = get_register(reg_watcher->name);
    if (reg == NULL) {
        log_warn("Register %s not registered", reg_watcher->name);
        return;
    }
    *reg = NULL;
}

unsigned int has_changed(char *new_hex_repr, char *hex_repr) {
    if (hex_repr == NULL || new_hex_repr == NULL) return 0;

    if (strcmp(hex_repr, new_hex_repr) == 0) return 0;
    return 1;
}

Watchers get_watchers(void) {
    char *acum_hex_repr = int_to_hex("0x", word_to_int(watchers.ACUM->reg->value));
    char *b_hex_repr = int_to_hex("0x", word_to_int(watchers.B->reg->value));
    char *c_hex_repr = int_to_hex("0x", word_to_int(watchers.C->reg->value));
    char *d_hex_repr = int_to_hex("0x", word_to_int(watchers.D->reg->value));
    char *e_hex_repr = int_to_hex("0x", word_to_int(watchers.E->reg->value));
    char *fc_hex_repr = int_to_hex("0x", word_to_int(watchers.FC->reg->value));
    char *fz_hex_repr = int_to_hex("0x", word_to_int(watchers.FZ->reg->value));
    char *h_hex_repr = int_to_hex("0x", word_to_int(watchers.H->reg->value));
    char *l_hex_repr = int_to_hex("0x", word_to_int(watchers.L->reg->value));
    char *op2_hex_repr = int_to_hex("0x", word_to_int(watchers.OP2->reg->value));
    char *ri_hex_repr = int_to_hex("0x", word_to_int(watchers.RI->reg->value));
    char *pcl_hex_repr = int_to_hex("0x", word_to_int(watchers.PCL->reg->value));
    char *pch_hex_repr = int_to_hex("0x", word_to_int(watchers.PCH->reg->value));
    char *pc_hex_repr = int_to_hex("0x", word_to_int(watchers.PC->reg->value));
    char *sp_hex_repr = int_to_hex("0x", word_to_int(watchers.SP->reg->value));

    int acum_changed = has_changed(acum_hex_repr, watchers.ACUM->hex_repr);
    int b_changed = has_changed(b_hex_repr, watchers.B->hex_repr);
    int c_changed = has_changed(c_hex_repr, watchers.C->hex_repr);
    int d_changed = has_changed(d_hex_repr, watchers.D->hex_repr);
    int e_changed = has_changed(e_hex_repr, watchers.E->hex_repr);
    int fc_changed = has_changed(fc_hex_repr, watchers.FC->hex_repr);
    int fz_changed = has_changed(fz_hex_repr, watchers.FZ->hex_repr);
    int h_changed = has_changed(h_hex_repr, watchers.H->hex_repr);
    int l_changed = has_changed(l_hex_repr, watchers.L->hex_repr);
    int op2_changed = has_changed(op2_hex_repr, watchers.OP2->hex_repr);
    int ri_changed = has_changed(ri_hex_repr, watchers.RI->hex_repr);
    int pcl_changed = has_changed(pcl_hex_repr, watchers.PCL->hex_repr);
    int pch_changed = has_changed(pch_hex_repr, watchers.PCH->hex_repr);
    int pc_changed = has_changed(pc_hex_repr, watchers.PC->hex_repr);
    int sp_changed = has_changed(sp_hex_repr, watchers.SP->hex_repr);

    if (watchers.ACUM->hex_repr != NULL) {
        free(watchers.ACUM->hex_repr);
    }
    if (watchers.B->hex_repr != NULL) {
        free(watchers.B->hex_repr);
    }
    if (watchers.C->hex_repr != NULL) {
        free(watchers.C->hex_repr);
    }
    if (watchers.D->hex_repr != NULL) {
        free(watchers.D->hex_repr);
    }
    if (watchers.E->hex_repr != NULL) {
        free(watchers.E->hex_repr);
    }
    if (watchers.FC->hex_repr != NULL) {
        free(watchers.FC->hex_repr);
    }
    if (watchers.FZ->hex_repr != NULL) {
        free(watchers.FZ->hex_repr);
    }
    if (watchers.H->hex_repr != NULL) {
        free(watchers.H->hex_repr);
    }
    if (watchers.L->hex_repr != NULL) {
        free(watchers.L->hex_repr);
    }
    if (watchers.OP2->hex_repr != NULL) {
        free(watchers.OP2->hex_repr);
    }
    if (watchers.RI->hex_repr != NULL) {
        free(watchers.RI->hex_repr);
    }
    if (watchers.PCL->hex_repr != NULL) {
        free(watchers.PCL->hex_repr);
    }
    if (watchers.PCH->hex_repr != NULL) {
        free(watchers.PCH->hex_repr);
    }
    if (watchers.PC->hex_repr != NULL) {
        free(watchers.PC->hex_repr);
    }
    if (watchers.SP->hex_repr != NULL) {
        free(watchers.SP->hex_repr);
    }

    watchers.ACUM->hex_repr = acum_hex_repr;
    watchers.B->hex_repr = b_hex_repr;
    watchers.C->hex_repr = c_hex_repr;
    watchers.D->hex_repr = d_hex_repr;
    watchers.E->hex_repr = e_hex_repr;
    watchers.FC->hex_repr = fc_hex_repr;
    watchers.FZ->hex_repr = fz_hex_repr;
    watchers.H->hex_repr = h_hex_repr;
    watchers.L->hex_repr = l_hex_repr;
    watchers.OP2->hex_repr = op2_hex_repr;
    watchers.RI->hex_repr = ri_hex_repr;
    watchers.PCL->hex_repr = pcl_hex_repr;
    watchers.PCH->hex_repr = pch_hex_repr;
    watchers.PC->hex_repr = pc_hex_repr;
    watchers.SP->hex_repr = sp_hex_repr;

    watchers.ACUM->changed = acum_changed;
    watchers.B->changed = b_changed;
    watchers.C->changed = c_changed;
    watchers.D->changed = d_changed;
    watchers.E->changed = e_changed;
    watchers.FC->changed = fc_changed;
    watchers.FZ->changed = fz_changed;
    watchers.H->changed = h_changed;
    watchers.L->changed = l_changed;
    watchers.OP2->changed = op2_changed;
    watchers.RI->changed = ri_changed;
    watchers.PCL->changed = pcl_changed;
    watchers.PCH->changed = pch_changed;
    watchers.PC->changed = pc_changed;
    watchers.SP->changed = sp_changed;

    return watchers;
}