//
//  utils.c
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//

#include "utils.h"
#include <stdlib.h>
#include <string.h>

int hex_to_int(const char *hex) {
	int number = (int) strtol(hex, NULL, 16);
	return number;
}

char *int_to_hex(int num) { // TODO check malloc failures
	int size = 4;
	char *result = malloc(sizeof(char) * size + 1);
	snprintf(result, size, "%x", num);

	while (1) {
		const int next_size = size + 1;
		char *result2 = malloc(sizeof(char) * next_size + 1);
		snprintf(result2, next_size, "%x", num);

		if (!strcmp(result, result2)) break;

		free(result);
		result = result2;

		size++;
	}

	return result;
}

int random_int(const int end_bound) {
	int r = rand() % end_bound + 1;
	return r;
}
