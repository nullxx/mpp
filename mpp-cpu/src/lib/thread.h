/*
 * File: /src/lib/process.h
 * Project: mpp-cpu
 * File Created: Friday, 8th April 2022 6:04:56 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Friday, 8th April 2022 6:04:59 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#ifndef process_h
#define process_h
#include <pthread.h>
pthread_t *create_pthread(const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
int get_thread_count(void);
// int create_process(void (*)(void));
// int kill_process(int pid);
// void run_task_in(int pid);
// void wait_for(int signal_code, unsigned long pid);
// unsigned long wait_for_init(int signal_code, unsigned long pid);
#endif