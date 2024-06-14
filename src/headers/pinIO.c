#include "pinIO.h"
#include <stdlib.h>
#include <stdint.h>

bool* massPinRead(enum Port port) {
	bool* res = malloc(sizeof(bool) * 8);
	for (int i = 0; i < 8; ++i) {
		res[i] = 0;
	}

	int offset = 0;

	if (port >= H) {
		offset = 0x100;
	}

	uint16_t* directionRegister = 0x01 + (0x03 * port) + 0x20 + offset;

	for (int i = 0; i < 8; ++i) {
		if (!(*directionRegister & (1 << i))) {
			res[i] = *(directionRegister - 1) & (1 << i);
		}
	}

	
	return(res);
}

void massPinSet(enum Port port, bool* valArr) {

	int offset = 0;

	if (port >= H) {
		offset = 0x100;
	}

	uint16_t* directionRegister = 0x01 + (0x03 * port) + 0x20 + offset;

	for (int i = 0; i < 8; ++i) {
		if (!(*directionRegister & (1 << i))) {
			if (!valArr[i]) {
				*(directionRegister - 1) &= ~(1 << i);
			}
			else {
				*(directionRegister - 1) |= (1 << i);
			}
		}
	}
}

void massDirSet(enum Port port, bool* valArr) {

	int offset = 0;

	if (port >= H) {
		offset = 0x100;
	}

	uint16_t* directionRegister = 0x01 + (0x03 * port) + 0x20 + offset;

	for (int i = 0; i < 8; ++i) {
		if (res[i]) {
			*directionRegister |= (1 << i);
		}
		else {
			*(directionRegister) &= ~(1 << i);
		}
		
	}
}

void setPullUp(enum Port port, bool* valArr) {

	int offset = 0;

	if (port >= H) {
		offset = 0x100;
	}

	uint16_t* directionRegister = 0x02 + (0x03 * port) + 0x20 + offset;

	for (int i = 0; i < 8; ++i) {
		if (res[i]) {
			*(directionRegister + 1) |= (1 << i);
		}

	}
}