//
//  pubsub.h
//  mpp-cpu
//
//  Created by Jon Lara trigo on 22/3/22.
//

#ifndef pubsub_h
#define pubsub_h

#include "electronic/bus.h"

typedef enum {
    NONE_PUBSUB_TOPIC = 0,
    DATA_BUS_TOPIC,
    DIR_BUS_TOPIC_1,
    DIR_BUS_TOPIC_2,

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

    CU_RI_OUTPUT_BUS_TOPIC,

    CU_SEQ_JOINER_OUTPUT_BUS_TOPIC,
    CU_SEQ_ACTUAL_STATUS_Q4_BUS_TOPIC, 
    CU_SEQ_ACTUAL_STATUS_Q3_BUS_TOPIC, 
    CU_SEQ_ACTUAL_STATUS_Q2_BUS_TOPIC, 
    CU_SEQ_ACTUAL_STATUS_Q1_BUS_TOPIC, 
    CU_SEQ_ACTUAL_STATUS_Q0_BUS_TOPIC,

    CU_SEQ_OUTPUT_D0_BUS_TOPIC,
    CU_SEQ_OUTPUT_D1_BUS_TOPIC,
    CU_SEQ_OUTPUT_D2_BUS_TOPIC,
    CU_SEQ_OUTPUT_D3_BUS_TOPIC,
    CU_SEQ_OUTPUT_D4_BUS_TOPIC,

    CU_MXREG_JNR_OUTPUT_1_BUS_TOPIC,
    CU_MXREG_JNR_OUTPUT_2_BUS_TOPIC,
    
    CONTROL_BUS_TOPIC
} PubSubTopic;

const char *pubsub_topic_tostring(PubSubTopic topic);

typedef struct {
    int id;
    PubSubTopic topic;
    Bus_t *bus_t;
    int active;
} PubSubSubscription;

typedef int (*PubSubMiddlewareFn)(Word);

typedef struct {
    PubSubTopic topic;
    PubSubMiddlewareFn middlware;
    int id;
} PubSubMiddleware;

#ifndef DEBUG  // this is for debugging. I don't know if is the best practice. Btw is very ougly
PubSubSubscription *subscribe_to(PubSubTopic, Bus_t*);
#else
PubSubSubscription *subscribe_to_internal(PubSubTopic topic, Bus_t *bus_t, const char *caller);
#define subscribe_to(topic, bus_t) subscribe_to_internal(topic, bus_t, __func__);
#endif
int unsubscribe_for(PubSubSubscription *);

int publish_message_to(PubSubTopic, Word);

PubSubMiddleware *add_topic_middleware(PubSubTopic topic, PubSubMiddlewareFn middleware_fn);
int rm_topic_middleware(PubSubMiddleware *middleware);

#endif /* pubsub_h */
