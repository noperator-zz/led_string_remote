#include <avr/interrupt.h>
#include <avr/io.h>
#include "ir.h"

volatile bool ir_active = false;
volatile bool ir_available = false;
volatile uint64_t ir_code = 0;

void ir_init() {
	// D0 as input for INT0
	DDRD &= ~(1 << 0);
	// INT0 on either edge
	EICRA |= (1 << ISC00);
	// Enable INT0
	EIMSK |= (1 << INT0);

	TCNT4 = 0;
	TCCR4B = (1 << CS43) | (1 << CS40);
	TIMSK4 = (1 << TOIE4);
}

// INT0 handler
ISR(INT0_vect) {
	enum class IRState {
		START,
			DATA,
			ERROR,
	};

	static IRState ir_state = IRState::START;

	bool last_pin_state = !(PIND & (1 << 0));
	uint16_t pulse_len = TCNT4;
	TCNT4 = 0;

	// Only start capturing if the previous code has been consumed
	if (!ir_active) {
		if (!ir_available) {
			ir_state = IRState::START;
			ir_code = 0;
			ir_active = true;
		}
		return;
	}

	pulse_len += IR_BIT_LENGTH / 2;
	uint8_t nbits = pulse_len / IR_BIT_LENGTH;

//	if (pulse_len > IR_BIT_LENGTH) {
//		PORTC ^= (1 << 7);
//	}
//	int b = nbits;
//	while(b--) {
//		PORTC ^= (1 << 7);
//		_delay_us(50);
//	}

	switch (ir_state) {

		case IRState::START:
			if (!last_pin_state && nbits == 4) {
				ir_state = IRState::DATA;
			} else {
				ir_state = IRState::ERROR;
			}
		break;
//		case IRState::START_HIGH:
//			if (last_pin_state && nbits == 1) {
//				ir_state = IRState::DATA;
//			} else {
//				ir_state = IRState::ERROR;
//			}
//			break;
		case IRState::DATA:
			while (nbits--) {
				ir_code <<= 1;
				ir_code |= last_pin_state;
			}
		break;
		case IRState::ERROR:
			break;
	}

}


ISR(TIMER4_OVF_vect) {
	if (ir_active) {
		ir_active = false;
		ir_available = true;
	}
}
