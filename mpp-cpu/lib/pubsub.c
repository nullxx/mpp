//
//  pubsub.c
//  mpp-cpu
//
//  Created by Jon Lara trigo on 22/3/22.
//

#include "pubsub.h"
#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

PubSubSubscription **subscriptions = NULL;
unsigned int subscription_count = 0;

char *pubsub_topic_tostring(PubSubTopic topic) {
	switch (topic) {
	case NONE_PUBSUB_TOPIC: return "NONE_PUBSUB_TOPIC";
	case DATA_BUS_TOPIC: return "DATA_BUS_TOPIC";
	case DIR_BUS_TOPIC: return "DIR_BUS_TOPIC";
	case PC_OUTPUT_BUS_TOPIC: return "PC_OUTPUT_BUS_TOPIC";
	case SP_OUTPUT_BUS_TOPIC: return "SP_OUTPUT_BUS_TOPIC";
	case HL_OUTPUT_BUS_TOPIC: return "HL_OUTPUT_BUS_TOPIC";
	case FFFC_OUTPUT_BUS_TOPIC: return "FFFC_OUTPUT_BUS_TOPIC";

	default: return NULL;
	}
}

PubSubSubscription *subscribe_to(PubSubTopic topic, on_message on_message_fn) {
	PubSubSubscription *subscription = malloc(sizeof(PubSubSubscription));
	if (subscription == NULL) {
		return NULL;
	}

	subscription->id = subscription_count++; // id is also the index inside subscriptions
	subscription->topic = topic;
	subscription->on_message_fn = on_message_fn;

	// allocate for n subscriptions
	PubSubSubscription **subscriptions_rllc = realloc(subscriptions, sizeof(PubSubSubscription) * subscription_count);
	if (subscriptions_rllc == NULL) {
		free(subscription);
		return NULL;
	}
	subscriptions = subscriptions_rllc;


	subscriptions[subscription->id] = subscription;

	return subscription;
}

bool unsubscribe_for(PubSubSubscription *sub) {
	if (sub == NULL) return false;

	if (sub->id >= subscription_count) {
		return false;
	}

	free(subscriptions[sub->id]);
	subscriptions[sub->id] = NULL;
	return true;
}

int publish_message_to(PubSubTopic topic, void *value) {
	PubSubMessage message = {.topic = topic, .value = value};

	int sent = 0;

	// find the subs subscribed to this topic
	for (int i = 0; i < subscription_count; i++) {
		PubSubSubscription *sub = subscriptions[i];
		if (sub == NULL || sub->topic != topic) continue;
		sub->on_message_fn(message);
		sent++;
	}

#ifdef DEBUG
	const char* msg = "%s published to %d subscribers";
    char *topic_str = pubsub_topic_tostring(topic);

    const int sent_len = (int) log10(sent + 1) + 1;
    const size_t size = sizeof(char) * (strlen(msg) + sent_len + strlen(topic_str) - 2 * 2 + 1);

    char *constructed_msg = malloc(size);
    snprintf(constructed_msg, size, msg, topic_str, sent);

	log_debug(constructed_msg);

    free(constructed_msg);
#endif
	return sent;
}
