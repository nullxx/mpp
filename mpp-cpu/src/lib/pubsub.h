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
typedef unsigned long long DataBus_t;
typedef unsigned long long DirBus_t;

typedef unsigned long long PCOutputBus_t;
typedef unsigned long long SPOutputBus_t;
typedef unsigned long long HLOutputBus_t;
typedef unsigned long long FFFCOutputBus_t;

// -- types PubSubMessage value

typedef enum {
    NONE_PUBSUB_TOPIC = 0,
    DATA_BUS_TOPIC,
    DIR_BUS_TOPIC,
    PC_OUTPUT_BUS_TOPIC,
    SP_OUTPUT_BUS_TOPIC,
    HL_OUTPUT_BUS_TOPIC,
    FFFC_OUTPUT_BUS_TOPIC  // <===== TODO FFFC is a single component
} PubSubTopic;

typedef struct {
    PubSubTopic topic;
    void *value;
} PubSubMessage;

const char *pubsub_topic_tostring(PubSubTopic topic);

typedef void (*on_message)(PubSubMessage);

typedef struct {
    unsigned int id;
    PubSubTopic topic;
    on_message on_message_fn;
} PubSubSubscription;

PubSubSubscription *subscribe_to(PubSubTopic, on_message);
bool unsubscribe_for(PubSubSubscription *);
int publish_message_to(PubSubTopic, void *);

#endif /* pubsub_h */
