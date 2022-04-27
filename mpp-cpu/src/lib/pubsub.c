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

#include "constants.h"
#include "linkedlist.h"
#include "logger.h"
#include "utils.h"

// TODO check in all files that use subscribe and unsubscribe if they check return of those functions

PubSubMiddleware **topics_with_middleware;
int topic_with_middleware_count = 0;

// PubSubSubscription **subscriptions = NULL;
LlNode *subscriptions_head = NULL;
int subscription_count = 0;

#ifndef DEBUG
PubSubSubscription *subscribe_to(PubSubTopic topic, Bus_t *bus_t) {
#else
PubSubSubscription *subscribe_to_internal(PubSubTopic topic, Bus_t *bus_t, const char *caller) {
#endif
    PubSubSubscription *subscription = (PubSubSubscription *)malloc(sizeof(PubSubSubscription));
    if (subscription == NULL) {
        return NULL;
    }

    subscription->id = subscription_count++;  // id is also the index inside subscriptions
    subscription->topic = (PubSubTopic)topic;
    subscription->bus_t = bus_t;
    subscription->active = 1;

    if (subscriptions_head == NULL) {
        subscriptions_head = create_ll_node((void *)subscription, NULL);
    } else {
        push_ll_node(subscriptions_head, subscription, NULL);
    }

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

    int sub_active_prev = sub->active;
    sub->active = 0;

    int deleted = delete_node_from_value(&subscriptions_head, sub);
    if (!deleted) {
        response = false;
        sub->active = sub_active_prev;
        goto end;
    }

    // free(sub); // not deleting the subscription, just removing it from the list
end:
    return response;
}

/**
 * @brief Publishes a mesage to a topic
 *
 * @param topic
 * @param value
 * @return int -1 if middleware doesn't pass
 */
int publish_message_to(PubSubTopic topic, Word value) {
    int sent = 0;

    // executing middlewares
    for (int i = 0; i < topic_with_middleware_count; i++) {
        if (topics_with_middleware[i] == NULL || topics_with_middleware[i]->topic != topic) continue;
        bool middleware_passes = topics_with_middleware[i]->middlware(value);

        if (!middleware_passes) {
            sent = -1;
            goto end;
        }
    }

    // find the subs subscribed to this topic
    LlNode *current_node = subscriptions_head;
    while (current_node != NULL) {
        PubSubSubscription *sub = (PubSubSubscription *)current_node->value;
        current_node = current_node->next;
        if (sub == NULL) continue;
        if (!sub->active) continue;
        if (sub->topic != topic) continue;

        sub->bus_t->next_value = value;

        sent++;
    }

end:
    return sent;
}
