////
////  pubsubtest.c
////  mpp-cpu
////
////  Created by Jon Lara trigo on 22/3/22.
////
//
//#include <stdio.h>
//
//#include "pubsub.h"
//
//void on_message_fn(PubSubMessage message) {
//    printf("Message Topic: %d\n", message.topic);
//    printf("Message Value: %s\n", (char*)message.value);
//}
//
//int main(void) {
//    PubSubSubscription *sub = subscribe_to(DATA_BUS_TOPIC, &on_message_fn);
//    int published_to1 = publish_message_to(DATA_BUS_TOPIC, (void*) "Message");
//    unsubscribe_for(sub);
//    int published_to2 = publish_message_to(DATA_BUS_TOPIC, (void*) "Message2");
//    int published_to3 = publish_message_to(DATA_BUS_TOPIC, (void*) "Message3");
//
//    printf("published_to1 %d\n", published_to1);
//    printf("published_to2 %d\n", published_to2);
//    printf("published_to3 %d\n", published_to3);
//    return 0;
//}
