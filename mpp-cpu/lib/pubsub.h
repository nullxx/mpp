//
//  pubsub.h
//  mpp-cpu
//
//  Created by Jon Lara trigo on 22/3/22.
//

#ifndef pubsub_h
#define pubsub_h

#include <stdbool.h>

// types PubSubMessage value
typedef unsigned long long BusData_t;
// -- types PubSubMessage value

typedef enum {
    NONE_PUBSUB_TOPIC = 0,
    DATA_BUS_TOPIC
} PubSubTopic;

typedef struct {
    PubSubTopic topic;
    void *value;
} PubSubMessage;

typedef void (*on_message)(PubSubMessage);

typedef struct {
    unsigned int id;
    PubSubTopic topic;
    on_message on_message_fn;
} PubSubSubscription;

PubSubSubscription *subscribe_to(PubSubTopic topic, on_message on_message_fn);
bool unsubscribe_for(PubSubSubscription *sub);
int publish_message_to(PubSubTopic topic, void *value);

#endif /* pubsub_h */
