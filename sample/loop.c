#include <avr/io.h>
#include <util/delay.h>
#include "header.h"

void loop() {
	PORTB = 0xff;
	_delay_ms(500);
	PORTB = 0x00;
	_delay_ms(500);
}
