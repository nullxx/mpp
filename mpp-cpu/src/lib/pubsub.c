//
//  pubsub.c
//  mpp-cpu
//
//  Created by Jon Lara trigo on 22/3/22.
//

#include "pubsub.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logger.h"

// TODO check in all files that use subscribe and unsubscribe if they check return of those functions

PubSubSubscription **subscriptions = NULL;
unsigned int subscription_count = 0;

const char *pubsub_topic_tostring(PubSubTopic topic) {
    switch (topic) {
        case NONE_PUBSUB_TOPIC:
            return "NONE_PUBSUB_TOPIC";
        case DATA_BUS_TOPIC:
            return "DATA_BUS_TOPIC";
        case DIR_BUS_TOPIC:
            return "DIR_BUS_TOPIC";
        case PC_OUTPUT_BUS_TOPIC:
            return "PC_OUTPUT_BUS_TOPIC";
        case SP_OUTPUT_BUS_TOPIC:
            return "SP_OUTPUT_BUS_TOPIC";
        case HL_OUTPUT_BUS_TOPIC:
            return "HL_OUTPUT_BUS_TOPIC";
        case FFFC_OUTPUT_BUS_TOPIC:
            return "FFFC_OUTPUT_BUS_TOPIC";

        default:
            return NULL;
    }
}

PubSubSubscription *subscribe_to(PubSubTopic topic, on_message on_message_fn) {
    PubSubSubscription *subscription = (PubSubSubscription *)malloc(sizeof(PubSubSubscription));
    if (subscription == NULL) {
        return NULL;
    }

    subscription->id = subscription_count++;  // id is also the index inside subscriptions
    subscription->topic = topic;
    subscription->on_message_fn = on_message_fn;

    // allocate for n subscriptions
    PubSubSubscription **subscriptions_rllc = (PubSubSubscription **)realloc(subscriptions, sizeof(PubSubSubscription) * subscription_count);
    if (subscriptions_rllc == NULL) {
        free(subscriptions);
        return NULL;
    }
    subscriptions = subscriptions_rllc;

    subscriptions[subscription->id] = subscription;

#ifdef DEBUG
    const char *msg = "Created subscription for %s with id %d";
    const char *topic_str = pubsub_topic_tostring(topic);

    const int sub_id_len = (int)log10(subscription->id + 1) + 1;
    const size_t size = sizeof(char) * (strlen(msg) + strlen(topic_str) + sub_id_len - 2 * 2 + 1);

    char *constructed_msg = malloc(size);
    snprintf(constructed_msg, size, msg, topic_str, subscription->id);

    log_debug(constructed_msg);

    free(constructed_msg);
#endif
    return subscription;
}

bool unsubscribe_for(PubSubSubscription *sub) {
    const int sub_id = sub->id;  // it is used

    if (sub == NULL) return false;

    if (sub->id >= subscription_count) {
        return false;
    }

    free(sub);
    sub = NULL;
    subscriptions[sub_id] = NULL;

#ifdef DEBUG
    const char *msg = "Deleting subscription with id %d";

    const int sub_id_len = (int)log10(sub_id + 1) + 1;
    const size_t size = sizeof(char) * (strlen(msg) + sub_id_len - 2 * 1 + 1);

    char *constructed_msg = malloc(size);
    snprintf(constructed_msg, size, msg, sub_id);

    log_debug(constructed_msg);

    free(constructed_msg);
#endif

    return true;
}

int publish_message_to(PubSubTopic topic, void *value) {
    PubSubMessage message = {.topic = topic, .value = value};

    int sent = 0;

    // find the subs subscribed to this topic
    for (unsigned int i = 0; i < subscription_count; i++) {
        PubSubSubscription *sub = subscriptions[i];
        if (sub == NULL || sub->topic != topic) continue;
        sub->on_message_fn(message);
        sent++;
    }

#ifdef DEBUG
    const char *msg = "%s published to %d subscribers";
    const char *topic_str = pubsub_topic_tostring(topic);

    const int sent_len = (int)log10(sent + 1) + 1;
    const size_t size = sizeof(char) * (strlen(msg) + sent_len + strlen(topic_str) - 2 * 2 + 1);

    char *constructed_msg = malloc(size);
    snprintf(constructed_msg, size, msg, topic_str, sent);

    log_debug(constructed_msg);

    free(constructed_msg);
#endif
    return sent;
}
