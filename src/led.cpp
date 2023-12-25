#include "led.h"
#include <avr/io.h>
#include <util/delay.h>

LED led_red {&OCR1A};
LED led_green {&OCR1B};
LED led_blue {&OCR1C};

void led_init() {
	// set up timer 1 channels A, B, C for 8-bit PWM output
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << COM1C1) | (1 << WGM10);
	TCCR1B = (1 << CS10) | (1 << WGM12);

	DDRB |= (1 << 5) | (1 << 6) | (1 << 7);

	led_red.set(0);
	led_green.set(0);
	led_blue.set(0);
}

void led_blink(uint8_t r, uint8_t g, uint8_t b, uint16_t ms) {
	// TODO this should not turn the LED back on if it was off before
	uint8_t r0 = led_red.get();
	uint8_t g0 = led_green.get();
	uint8_t b0 = led_blue.get();

	led_red.set(r);
	led_green.set(g);
	led_blue.set(b);

	while (ms--) {
		_delay_ms(1);
	}

	led_red.set(r0);
	led_green.set(g0);
	led_blue.set(b0);
}
