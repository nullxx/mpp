//
//  pubsub.c
//  mpp-cpu
//
//  Created by Jon Lara trigo on 22/3/22.
//
#define PUBSUB_SEM "/MPP_PUBSUB_SEM02"

#include "pubsub.h"

#include <math.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkedlist.h"
#include "logger.h"
#include "utils.h"
#include "constants.h"

// TODO check in all files that use subscribe and unsubscribe if they check return of those functions

PubSubMiddleware **topics_with_middleware;
int topic_with_middleware_count = 0;

// PubSubSubscription **subscriptions = NULL;
LlNode *subscriptions_head = NULL;
int subscription_count = 0;

sem_t *sem;
int init_pubsub(void) {
    sem = sem_open(PUBSUB_SEM, O_CREAT, 0660, CONSTANT_RUNNING_MODULES_COUNT);  // initial value to 1 does't work
    if (sem == SEM_FAILED) {
        return -1;
    }

    return 1;
}

void shutdown_pubsub(void) {
    sem_close(sem);
    sem_unlink(PUBSUB_SEM);
}

const char *pubsub_topic_tostring(PubSubTopic topic) {
    switch (topic) {
        case NONE_PUBSUB_TOPIC:
            return "NONE_PUBSUB_TOPIC";
        case DATA_BUS_TOPIC:
            return "DATA_BUS_TOPIC";
        case DIR_BUS_TOPIC_1:
            return "DIR_BUS_TOPIC_1";
        case DIR_BUS_TOPIC_2:
            return "DIR_BUS_TOPIC_2";
        case PC_OUTPUT_BUS_TOPIC:
            return "PC_OUTPUT_BUS_TOPIC";
        case SP_OUTPUT_BUS_TOPIC:
            return "SP_OUTPUT_BUS_TOPIC";
        case HL_OUTPUT_BUS_TOPIC:
            return "HL_OUTPUT_BUS_TOPIC";
        case FFFC_OUTPUT_BUS_TOPIC:
            return "FFFC_OUTPUT_BUS_TOPIC";
        case ACUMM_OUTPUT_BUS_TOPIC:
            return "ACUMM_OUTPUT_BUS_TOPIC";
        case OP2_OUTPUT_BUS_TOPIC:
            return "OP2_OUTPUT_BUS_TOPIC";
        case ALU_FC_OUTPUT_BUS_TOPIC:
            return "ALU_FC_OUTPUT_BUS_TOPIC";
        case ALU_FZ_OUTPUT_BUS_TOPIC:
            return "ALU_FZ_OUTPUT_BUS_TOPIC";
        case SELREG_OUTPUT_BUS_TOPIC:
            return "SELREG_OUTPUT_BUS_TOPIC";
        case MXFLD7_OUTPUT_BUS_TOPIC:
            return "MXFLD7_OUTPUT_BUS_TOPIC";
        case MXFLD0_OUTPUT_BUS_TOPIC:
            return "MXFLD0_OUTPUT_BUS_TOPIC";
        case FLAGS_OUTPUT_BUS_TOPIC:
            return "FLAGS_OUTPUT_BUS_TOPIC";

        default:
            return NULL;
    }
}

#ifndef DEBUG
PubSubSubscription *subscribe_to(PubSubTopic topic, Bus_t *var_ptr) {
#else
PubSubSubscription *subscribe_to_internal(PubSubTopic topic, Bus_t *var_ptr, const char *caller) {
#endif
    PubSubSubscription *subscription = (PubSubSubscription *)malloc(sizeof(PubSubSubscription));
    if (subscription == NULL) {
        return NULL;
    }

    subscription->id = subscription_count++;  // id is also the index inside subscriptions
    subscription->topic = topic;
    subscription->var = var_ptr;

    if (subscriptions_head == NULL) {
        subscriptions_head = create_ll_node((void *)subscription, NULL);
    } else {
        push_ll_node(subscriptions_head, subscription, NULL);
    }

#ifdef DEBUG
    char *sub_id_str = itoa(subscription->id);
    char *constructed_msg = create_str(caller, "Created subscription for", pubsub_topic_tostring(topic), "with id", sub_id_str);
    log_debug(constructed_msg);
    free(sub_id_str);
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
    char *constructed_msg = create_str("Adding middleware for topic", pubsub_topic_tostring(topic));
    log_debug(constructed_msg);
    free(constructed_msg);
#endif
    return m;
}

int rm_topic_middleware(PubSubMiddleware *middleware) {
    if (middleware == NULL) {
        return false;
    }

    free(middleware);
    middleware = NULL;
    topics_with_middleware[topic_with_middleware_count] = NULL;

    return true;
}

int unsubscribe_for(PubSubSubscription *sub) {
    bool response = true;
    if (sub == NULL) {
        response = false;
        goto end;
    };
    if (subscriptions_head == NULL) {
        response = false;
        goto end;
    };

    if (sub->id >= subscription_count) {
        response = false;
        goto end;
    }

    sem_wait(sem);
    int deleted = delete_node_from_value(&subscriptions_head, sub);
    if (!deleted) {
        response = false;
        goto end;
    }

#ifdef DEBUG
    char *sub_id_str = itoa(sub->id);
    char *constructed_msg = create_str("Deleting subscription with id", sub_id_str);
    log_debug(constructed_msg);
    free(sub_id_str);
    free(constructed_msg);

#endif
    free(sub);
end:
    sem_post(sem);
    return response;
}

/**
 * @brief Publishes a mesage to a topic
 *
 * @param topic
 * @param value
 * @return int -1 if middleware doesn't pass
 */
int publish_message_to(PubSubTopic topic, Bus_t value) {
    sem_wait(sem);
    int sent = 0;

    // executing middlewares
    for (int i = 0; i < topic_with_middleware_count; i++) {
        if (topics_with_middleware[i] == NULL || topics_with_middleware[i]->topic != topic) continue;
        bool middleware_passes = topics_with_middleware[i]->middlware(value);
#ifdef DEBUG
        const char *result = middleware_passes ? "true" : "false";
        char *constructed_msg = create_str("Executed middleware for topic", pubsub_topic_tostring(topic), "with result", result);
        log_debug(constructed_msg);
        free(constructed_msg);
#endif
        if (!middleware_passes) {
            sent = -1;
            goto end;
        };
    }

    // find the subs subscribed to this topic
    LlNode *current_node = subscriptions_head;
    while (current_node != NULL) {
        PubSubSubscription *sub = (PubSubSubscription *)current_node->value;
        current_node = current_node->next;
        if (sub == NULL) continue;
        if (sub->topic != topic) continue;
        
        *sub->var = value;

        sent++;
    }

#ifdef DEBUG
    char *sent_str = itoa(sent);
    char *constructed_msg = create_str(pubsub_topic_tostring(topic), "published to", sent_str, "subscribers");
    log_debug(constructed_msg);
    free(sent_str);
    free(constructed_msg);
#endif

end:
    sem_post(sem);
    return sent;
}
