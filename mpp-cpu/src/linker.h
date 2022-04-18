/*
 * File: /src/linker.h
 * Project: mpp-cpu
 * File Created: Thursday, 1st January 1970 12:00:00 am
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Sunday, 17th April 2022 12:34:29 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#ifndef linker_h
#define linker_h
#include "lib/watcher.h"
typedef enum { STATE, MICRO_INSTRUCTION, PROGRAM } RunConfigType;

typedef struct {
    RunConfigType type;
} RunConfig;

typedef struct {
    char *offset_hex;
    char *value_hex;
} MemValueUpdated;

typedef struct {
    int values_count;
    MemValueUpdated *values;
} MemUpdated;

void init_linker(void);
void shutdown_linker(void);
void set_update_ui(void (*update_ui_func)(void));
void run(RunConfig);
int is_running(void);
void cancel_run(void);

Watchers get_watchers_updated(void);
char* get_current_state_updated(void);
char *get_data_bus_updated(void);
char *get_dir_bus_updated(void);
char *get_control_bus_updated(void);
MemUpdated get_mem_updated(void);
#endif