//
//  pubsub.c
//  mpp-cpu
//
//  Created by Jon Lara trigo on 22/3/22.
//

#include "pubsub.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

PubSubSubscription **subscriptions = NULL;
unsigned int subscription_count = 0;

PubSubSubscription *subscribe_to(PubSubTopic topic, on_message on_message_fn) {
	PubSubSubscription *subscription = malloc(sizeof(PubSubSubscription));
    if (subscription == NULL) {
        return NULL;
    }

	subscription->id = subscription_count++; // id is also the index inside subscriptions
	subscription->topic = topic;
	subscription->on_message_fn = on_message_fn;

	// allocate for n subscriptions
	subscriptions = realloc(subscriptions, sizeof(PubSubSubscription) * subscription_count);
	subscriptions[subscription->id] = subscription;

	return subscription;
}

bool unsubscribe_for(PubSubSubscription *sub) {
	if (sub->id >= subscription_count) {
		return false;
	}

	free(subscriptions[sub->id]);
	subscriptions[sub->id] = NULL;
	return true;
}

int publish_message_to(PubSubTopic topic, void *value) {
	PubSubMessage message = {
		.topic = topic,
		.value = value
	};
    
    int sent = 0;

	// find the subs subscribed to this topic
	for (int i = 0; i < subscription_count; i++) {
		PubSubSubscription *sub = subscriptions[i];
		if (sub == NULL || sub->topic != topic) continue;
		sub->on_message_fn(message);
        sent++;
	}

    return sent;
}
