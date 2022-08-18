#include <stdio.h>

#include "../lib/constants.h"
#include "../lib/pubsub.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

static Bus_t *control_bus = NULL;
static Bus_t *last_bus_data = NULL;

static PubSubSubscription *control_bus_topic_subscription = NULL;
static PubSubSubscription *data_bus_topic_subscription = NULL;

void init_linker_bus(void) {
    control_bus = create_bus_data();
    last_bus_data = create_bus_data();

    control_bus_topic_subscription = subscribe_to(CONTROL_BUS_TOPIC, control_bus);
    data_bus_topic_subscription = subscribe_to(DATA_BUS_TOPIC, last_bus_data);
}

void shutdown_linker_bus(void) {
    unsubscribe_for(control_bus_topic_subscription);
    unsubscribe_for(data_bus_topic_subscription);

    destroy_bus_data(control_bus);
    destroy_bus_data(last_bus_data);
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_data_bus(void) { return word_to_int(last_bus_data->next_value); }


#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_control_bus_pccar(void) { return control_bus->next_value.bits[CONTROL_BUS_PCCAR_BIT_POSITION]; }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_control_bus_accar(void) { return control_bus->next_value.bits[CONTROL_BUS_ACCAR_BIT_POSITION]; }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_control_bus_acbus(void) { return control_bus->next_value.bits[CONTROL_BUS_ACBUS_BIT_POSITION]; }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_control_bus_spcar(void) { return control_bus->next_value.bits[CONTROL_BUS_SPCAR_BIT_POSITION]; }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_control_bus_2opcar(void) { return control_bus->next_value.bits[CONTROL_BUS_OP2CAR_BIT_POSITION]; }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_control_bus_hcar(void) { return control_bus->next_value.bits[CONTROL_BUS_HCAR_BIT_POSITION]; }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_control_bus_lcar(void) { return control_bus->next_value.bits[CONTROL_BUS_LCAR_BIT_POSITION]; }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_control_bus_ricar(void) { return control_bus->next_value.bits[CONTROL_BUS_RICAR_BIT_POSITION]; }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_control_bus_regcar(void) { return control_bus->next_value.bits[CONTROL_BUS_REGCAR_BIT_POSITION]; }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_control_bus_regbus(void) { return control_bus->next_value.bits[CONTROL_BUS_REGBUS_BIT_POSITION]; }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_control_bus_membus(void) { return control_bus->next_value.bits[CONTROL_BUS_MEMBUS_BIT_POSITION]; }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_control_bus_le(void) { return control_bus->next_value.bits[CONTROL_BUS_LE_BIT_POSITION]; }

