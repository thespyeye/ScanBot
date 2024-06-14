/*
	A set of functions to read and write to pins on the ATMega2560, as well as set pullup resistors and direction.
	This file assumes the user is using Microchip Studio 7.
*/

#ifndef PINIO_H
#define PINIO_H

enum Port{
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L
};

// Reads all of bits in a port and returns it as a bool* of length 8, where pin0 is element 0, pin1 is element 1, etc etc.
bool* massPinRead(enum Port port);

// All of these functions assume that valArr has 8 elements -- passing a shorter array will result in undefined behavior.
// massPinSet and setPullUp will check whether the pin is set as an output, and if so, skip that pin.


// Sets the values of all pins in a port.
void massPinSet(enum Port port, bool* valArr);


// Sets the direction of all pins in a port.
void massDirSet(enum Port port, bool* valArr);


// Sets the pull-up resistors of all pins in a port.
void setPullUp(enum Port port, bool* valArr);

#endif