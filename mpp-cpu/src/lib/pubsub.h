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

typedef unsigned long long ACUMMOutputBus_t;
typedef unsigned long long OP2OutputBus_t;

typedef unsigned int ALUFCOutputBus_t;
typedef unsigned int ALUFZOutputBus_t;

typedef unsigned long long SelRegOutputBus_t;

typedef unsigned int MXFlD7OutputBus_t;
typedef unsigned int MXFlD0OutputBus_t;

// -- types PubSubMessage value

typedef enum {
    NONE_PUBSUB_TOPIC = 0,
    DATA_BUS_TOPIC,
    DIR_BUS_TOPIC,

    PC_OUTPUT_BUS_TOPIC,
    SP_OUTPUT_BUS_TOPIC,
    HL_OUTPUT_BUS_TOPIC,
    FFFC_OUTPUT_BUS_TOPIC,

    ACUMM_OUTPUT_BUS_TOPIC,
    OP2_OUTPUT_BUS_TOPIC,

    ALU_FC_OUTPUT_BUS_TOPIC,
    ALU_FZ_OUTPUT_BUS_TOPIC,

    SELREG_OUTPUT_BUS_TOPIC,

    MXFLD7_OUTPUT_BUS_TOPIC,
    MXFLD0_OUTPUT_BUS_TOPIC,
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

typedef bool (*PubSubMiddlewareFn)(void *);

typedef struct {
    PubSubTopic topic;
    PubSubMiddlewareFn middlware;
} PubSubMiddleware;

#ifndef DEBUG  // this is for debugging. I don't know if is the best practice. Btw is very ougly
PubSubSubscription *subscribe_to(PubSubTopic, on_message);
#else
PubSubSubscription *subscribe_to_internal(PubSubTopic topic, on_message on_message_fn, const char *caller);
#define subscribe_to(topic, on_message_fn) subscribe_to_internal(topic, on_message_fn, __func__);
#endif
bool unsubscribe_for(PubSubSubscription *);

int publish_message_to(PubSubTopic, void *);

PubSubMiddleware *add_topic_middleware(PubSubTopic topic, PubSubMiddlewareFn middleware_fn);
bool rm_topic_middleware(PubSubMiddleware *middleware);

#endif /* pubsub_h */
