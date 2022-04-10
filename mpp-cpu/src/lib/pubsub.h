//
//  pubsub.h
//  mpp-cpu
//
//  Created by Jon Lara trigo on 22/3/22.
//

#ifndef pubsub_h
#define pubsub_h

#include "definitions.h"

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

    FLAGS_OUTPUT_BUS_TOPIC,
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
    Bus_t *var;
} PubSubSubscription;

typedef int (*PubSubMiddlewareFn)(Bus_t);

typedef struct {
    PubSubTopic topic;
    PubSubMiddlewareFn middlware;
} PubSubMiddleware;

#ifndef DEBUG  // this is for debugging. I don't know if is the best practice. Btw is very ougly
PubSubSubscription *subscribe_to(PubSubTopic, Bus_t*);
#else
PubSubSubscription *subscribe_to_internal(PubSubTopic topic, Bus_t *var_ptr, const char *caller);
#define subscribe_to(topic, var_ptr) subscribe_to_internal(topic, var_ptr, __func__);
#endif
int unsubscribe_for(PubSubSubscription *);

int publish_message_to(PubSubTopic, Bus_t);

PubSubMiddleware *add_topic_middleware(PubSubTopic topic, PubSubMiddlewareFn middleware_fn);
int rm_topic_middleware(PubSubMiddleware *middleware);

#endif /* pubsub_h */
