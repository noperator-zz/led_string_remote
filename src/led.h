#pragma once

#include <stdint.h>

class LED {
public:
	explicit LED(volatile uint16_t *pwmptr) : pwm(pwmptr) {
	}

	uint8_t get() {
		return percent;
	}

	void off() {
		// Disable without changing the value
		*pwm = 0;
	}

	void on() {
		// Enable without changing the value
		set(percent);
	}

	void set(int16_t value) {
		if (value < 0) {
			value = 0;
		}
		if (value > 100) {
			value = 100;
		}
		percent = value;
		value = (255 * value) / 100;
		*pwm = value;
	}

	void inc() {
		set(get() + 1);
	}
	void dec() {
		set(get() - 1);
	}

	void max() {
		set(100);
	}
private:
	volatile uint16_t * pwm;
	uint8_t percent = 0;
};


extern LED led_red;
extern LED led_green;
extern LED led_blue;

extern void led_init();
extern void led_blink(uint8_t r, uint8_t g, uint8_t b, uint16_t ms);
