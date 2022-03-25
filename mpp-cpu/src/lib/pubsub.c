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

PubSubMiddleware **topics_with_middleware;
unsigned int topic_with_middleware_count = 0;

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

#ifndef DEBUG
PubSubSubscription *subscribe_to(PubSubTopic topic, on_message on_message_fn) {
#else
PubSubSubscription *subscribe_to_internal(PubSubTopic topic, on_message on_message_fn, const char *caller) {
#endif
    PubSubSubscription *subscription = (PubSubSubscription *)malloc(sizeof(PubSubSubscription));
    if (subscription == NULL) {
        return NULL;
    }

    subscription->id = subscription_count++;  // id is also the index inside subscriptions
    subscription->topic = topic;
    subscription->on_message_fn = on_message_fn;

    // allocate for n subscriptions
    PubSubSubscription **subscriptions_rllc = (PubSubSubscription **)realloc(subscriptions, sizeof(PubSubSubscription *) * subscription_count);
    if (subscriptions_rllc == NULL) {
        return NULL;
    }
    subscriptions = subscriptions_rllc;

    subscriptions[subscription->id] = subscription;

#ifdef DEBUG
    const char *msg = "[%s] Created subscription for %s with id %d";
    const char *topic_str = pubsub_topic_tostring(topic);

    const int sub_id_len = (int)log10(subscription->id + 1) + 1;
    const size_t size = sizeof(char) * (strlen(msg) + strlen(topic_str) + sub_id_len + strlen(caller) - 2 * 3 + 1);

    char *constructed_msg = malloc(size);
    snprintf(constructed_msg, size, msg, caller, topic_str, subscription->id);

    log_debug(constructed_msg);

    free(constructed_msg);
#endif
    return subscription;
}

PubSubMiddleware *add_topic_middleware(PubSubTopic topic, PubSubMiddlewareFn middleware_fn) {
    if (middleware_fn == NULL) {
        return NULL;
    }

    PubSubMiddleware **topics_with_middleware_rllc = (PubSubMiddleware **)realloc(topics_with_middleware, sizeof(PubSubMiddleware *) * ++topic_with_middleware_count);
    if (topics_with_middleware_rllc == NULL) {
        return NULL;
    }
    topics_with_middleware = topics_with_middleware_rllc;

    PubSubMiddleware *m = (PubSubMiddleware *)malloc(sizeof(PubSubMiddleware));

    m->middlware = middleware_fn;
    m->topic = topic;

    topics_with_middleware[topic_with_middleware_count - 1] = m;

#ifdef DEBUG
    const char *msg = "Adding middleware for topic %s";
    const char *topic_str = pubsub_topic_tostring(topic);

    const size_t size = sizeof(char) * (strlen(msg) + strlen(topic_str) - 2 * 1 + 1);

    char *constructed_msg = malloc(size);
    snprintf(constructed_msg, size, msg, topic_str);

    log_debug(constructed_msg);

    free(constructed_msg);
#endif
    return m;
}

bool rm_topic_middleware(PubSubMiddleware *middleware) {
    if (middleware == NULL) {
        return false;
    }

    free(middleware);
    middleware = NULL;
    subscriptions[topic_with_middleware_count] = NULL;

    return true;
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

/**
 * @brief Publishes a mesage to a topic
 *
 * @param topic
 * @param value
 * @return int -1 if middleware doesn't pass
 */
int publish_message_to(PubSubTopic topic, void *value) {
    PubSubMessage message = {.topic = topic, .value = value};

    // executing middlewares
    for (size_t i = 0; i < topic_with_middleware_count; i++) {
        if (topics_with_middleware[i] == NULL || topics_with_middleware[i]->topic != topic) continue;
        bool middleware_passes = topics_with_middleware[i]->middlware(value);
#ifdef DEBUG
        const char *msg = "Executed middleware for topic %s with result %s";
        const char *result = middleware_passes ? "true" : "false";
        const char *topic_str = pubsub_topic_tostring(topic);

        const size_t size = sizeof(char) * (strlen(msg) + strlen(result) + strlen(topic_str) - 2 * 2 + 1);

        char *constructed_msg = malloc(size);
        snprintf(constructed_msg, size, msg, topic_str, result);

        log_debug(constructed_msg);

        free(constructed_msg);
#endif
        if (!middleware_passes) return -1;
    }

    // find the subs subscribed to this topic
    int sent = 0;
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
