//
//  mxdir.c
//  mpp-cpu
//
//  Created by https://github.com/nullxx on 23/3/22.
//

#include "addsub.h"

#include <stdio.h>

#include "../constants.h"
#include "../electronic/bus.h"
#include "../error.h"
#include "../pubsub.h"
#include "../thread.h"
#include "../utils.h"
#include "components.h"

static Bus_t *last_bus_dir = NULL;
static Bus_t *control_bus = NULL;

static PubSubSubscription *dir_bus_topic_subscription = NULL;
static PubSubSubscription *control_bus_topic_subscription = NULL;


void run_addsub(void) {
    update_bus_data(control_bus);

    int dir_bus = word_to_int(last_bus_dir->next_value);

    Word id_lb;
    initialize_word(&id_lb, 0);

    id_lb.bits[0] = control_bus->current_value.bits[CONTROL_BUS_ID_0_BIT_POSITION];
    id_lb.bits[1] = control_bus->current_value.bits[CONTROL_BUS_ID_1_BIT_POSITION];

    switch (word_to_int(id_lb)) {
        case 01:
            // +1
            dir_bus++;
            break;
        case 00:
            // +0
            break;
        case 11:
            // -1
            dir_bus--;
            break;
        default:
            break;
    }

    publish_message_to(DIR_BUS_TOPIC_1, int_to_word(dir_bus));
}

void init_addsub(void) {
    last_bus_dir = create_bus_data();
    control_bus = create_bus_data();
    dir_bus_topic_subscription = subscribe_to(DIR_BUS_TOPIC_2, last_bus_dir);
    control_bus_topic_subscription = subscribe_to(CONTROL_BUS_TOPIC, control_bus);
}

void shutdown_addsub(void) {
    unsubscribe_for(dir_bus_topic_subscription);
    unsubscribe_for(control_bus_topic_subscription);
    destroy_bus_data(last_bus_dir);
    destroy_bus_data(control_bus);
}