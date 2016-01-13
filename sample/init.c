#include <avr/io.h>
#include "header.h"

void init() {
	DDRB = 0xff;
}
