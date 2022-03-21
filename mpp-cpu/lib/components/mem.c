//
//  mem.c
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//

#include "mem.h"
#include "../constants.h"
#include "../utils.h"
#include "../error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// INTERNAL
Mem mem;
on_component_change component_change_fn;

// READ by default
Flag l_e_flag = {false, OUTPUT, 0};

bool is_mem_value_valid(char *hex) {
	const int num = hex_to_int(hex);
	if (num < 0 || num > pow(2, MEM_VALUE_SIZE_BITS) - 1) return false;
	return true;
}

void fill_memory(void) {
    const int mem_size_bits = MEM_SIZE_KB * 1024 * 8;
	const int mem_size = mem_size_bits / MEM_VALUE_SIZE_BITS;

	mem.values = malloc(sizeof(MemValue) * mem_size);
	mem.values_count = 0;

	// TODO free mem.values
	for (int i = MEM_START_VALUE; i < mem_size; i++) {
		MemValue mem_value;
		mem_value.offset = int_to_hex(i);
		mem_value.value_hex = int_to_hex(random_int(pow(2, MEM_VALUE_SIZE_BITS) - 1));
		mem.values[i] = mem_value;
		mem.values_count++;
		// TODO free mem_value.offset, mem_value.value_hex
	}
}

void init_mem(on_component_change on_component_change_fn) {
	fill_memory();
	component_change_fn = on_component_change_fn;
}

void trigger_l_e_flag_change(void) {
	CallbackEvent cbe;
	cbe.type = FLAG_CHANGE;
	cbe.flag = l_e_flag;
	component_change_fn(cbe);
}

// le_flag OUTPUT
void set_l_e_flag(void) {
	l_e_flag.value = 1;

	trigger_l_e_flag_change();
}

// le_flag RESET
void reset_l_e_flag(void) {
	l_e_flag.value = 0;

	trigger_l_e_flag_change();
}

MemValue *get_value_by_offset(char *offset) {
	for (int i = 0; i < mem.values_count; i++) {
		if (!strcmp(mem.values[i].offset, offset)) {
			return &mem.values[i];
		}
	}

	return NULL;
}

// -- INTERNAL

ComponentActionReturn set_mem_value(MemValue mem_value) {
	ComponentActionReturn car = { true };

	if (!is_mem_value_valid(mem_value.value_hex)) {
		car.success = false;
		car.err.show_errno = false;
		const char *msg = "[set_mem_value] Value %s invalid. Must be between 0-%d";
		const int end_bound_mem_value = pow(2, MEM_VALUE_SIZE_BITS) - 1;
		const int end_bound_mem_value_size = (int)log10((end_bound_mem_value + 1) + 1) + 1;
		const size_t size = (size_t) sizeof(char) * (strlen(msg) + strlen(mem_value.value_hex) + end_bound_mem_value_size - 2*2 + 1);
		car.err.message = malloc(size);
		snprintf(car.err.message, size, msg, mem_value.value_hex, end_bound_mem_value);
		return car;
	}

	MemValue *target_mem_value = get_value_by_offset(mem_value.offset);
	if (target_mem_value == NULL) {
		car.success = false;
		car.err.show_errno = false;
		const char *msg = "[set_mem_value] Couldn't find mem_value at %0x";
		const size_t size = (size_t) sizeof(char) * (strlen(msg) + strlen(mem_value.offset) - 3 + 1);
		car.err.message = malloc(size);
		snprintf(car.err.message, size, msg, mem_value.offset);
		return car;
	}

	free(target_mem_value->offset);
	free(target_mem_value->value_hex);

	target_mem_value->value_hex = mem_value.value_hex;
	target_mem_value->offset = mem_value.offset;

	return car;
}

ComponentActionReturn get_mem_value(char *offset) {
	ComponentActionReturn car = { true };
	MemValue *target_mem_value = get_value_by_offset(offset);
	if (target_mem_value == NULL) {
		car.success = false;
		car.err.show_errno = false;
		const char *msg = "[get_mem_value] Couldn't find mem_value at %0x";
		const size_t size = (size_t) sizeof(char) * (strlen(msg) + strlen(offset) - 3 + 1);
		car.err.message = malloc(size);
		snprintf(car.err.message, size, msg, offset);
		return car;
	}

	car.return_value = (void *) target_mem_value;

	return car;
}
