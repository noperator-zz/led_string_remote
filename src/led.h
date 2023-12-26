#pragma once

#include <stdint.h>

class LED {
public:
	LED(
		volatile uint8_t *ddrptr,
		volatile uint8_t *portptr,
		uint8_t pin,
		volatile uint8_t *timerptr,
		volatile uint16_t *pwmptr,
		uint8_t timerval
		)
		: ddr(ddrptr)
		, port(portptr)
		, pin(pin)
		, timer(timerptr)
		, pwm(pwmptr)
		, timerval(timerval)
	{
	}

	void init() {
		*ddr |= (1 << pin);
		off();
	}

	uint8_t get() const {
		return percent;
	}

	void off() {
		// Disable without changing the value
		*timer &= ~(1 << timerval);
		*port &= ~(1 << pin);
	}

	void on() {
		// Enable without changing the value
		*timer |= (1 << timerval);
		set(percent);
	}

	void set(int16_t value) {
		if (value < 0) {
			value = 0;
		}
		if (value > 100) {
			value = 100;
		}
		if (value) {
			*timer |= (1 << timerval);
		} else {
			off();
		}
		percent = value;
		value = (255 * value) / 100;
		*pwm = value;
	}

	void inc(uint8_t maximum) {
		set(get() + 1 > maximum ? maximum : get() + 1);
	}
	void dec(uint8_t minimum) {
		set(get() - 1 < minimum ? minimum : get() - 1);
	}

	void max() {
		set(100);
	}
private:
	volatile uint8_t * ddr;
	volatile uint8_t * port;
	uint8_t pin;
	volatile uint8_t * timer;
	volatile uint16_t * pwm;
	uint8_t timerval;
	uint8_t percent = 0;
};


extern LED led_red;
extern LED led_green;
extern LED led_blue;

extern void led_init();
extern void led_blink(uint8_t r, uint8_t g, uint8_t b, uint16_t ms);
