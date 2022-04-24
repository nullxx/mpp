/*
 * File: /src/lib/process.c
 * Project: mpp-cpu
 * File Created: Friday, 8th April 2022 6:05:02 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Friday, 8th April 2022 6:05:07 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

// not used

#define THREAD_DEFAULT_TIMEOUT_S 0.5

#include "thread.h"

#include <stdlib.h>
#include <time.h>

static int thread_count = 0;

typedef struct {
    pthread_t *thread_id;
    void *(*start_routine)(void *);
    void *start_routine_arg;
    int started;
    float start_timeout;
} thread_t;

void *routine_wrapper(void *arg) {
    thread_t *thread = (thread_t *)arg;

    thread->started = 1;

    thread->start_routine(thread->start_routine_arg);

    free(thread->thread_id);
    free(thread);

    thread_count--;
    return NULL;
}

pthread_t *create_pthread(const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg) {
    pthread_t *thread_id = NULL;
    thread_t *thread = NULL;

    thread_id = malloc(sizeof(pthread_t));
    if (thread_id == NULL) {
        goto error;
    }

    thread = malloc(sizeof(thread_t));
    if (thread == NULL) {
        goto error;
    }

    thread->thread_id = thread_id;
    thread->start_routine = start_routine;
    thread->start_routine_arg = arg;
    thread->started = 0;
    thread->start_timeout = THREAD_DEFAULT_TIMEOUT_S;

    int succss = pthread_create(thread_id, attr, routine_wrapper, thread);
    if (succss != 0) {
        goto error;
    }

    time_t time_start = clock();
    while (thread->started == 0) {
        time_t time_end = clock();
        float time_taken = (float)(time_end - time_start) / CLOCKS_PER_SEC;
        
        if (time_taken > thread->start_timeout) {
            break;
        }
    }

    if (thread->started == 0) {
        goto error;
    }

    thread_count++;

    return thread_id;
error:
    free(thread_id);
    free(thread);
    return NULL;
}

int get_thread_count(void) { return thread_count; }