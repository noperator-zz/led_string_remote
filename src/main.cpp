#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ir.h"
#include "led.h"

static constexpr uint16_t UI_BLINK_MS = 500;
static constexpr uint16_t UI_KEY_DELAY_MS = 500;

//void show_code(uint64_t code) {
//	PORTC &= ~(1 << 7);
//	int i = 64;
//	while (i--) {
//		bool state = code & 0x8000000000000000;
//		code <<= 1;
//		PORTC = (PORTC & ~(1 << 7)) | (state << 7);
//		_delay_us(500);
//	}
//	PORTC &= ~(1 << 7);
//}
//
//void blink(int num) {
//	while (num--) {
//		PORTC |= (1 << 7);
//		_delay_ms(250);
//		PORTC &= ~(1 << 7);
//		_delay_ms(250);
//	}
//}

int main() {
	enum class AppState {
		OFF,
		STATIC,
		//RAINBOW,
//		ADJUST,
	};

	AppState app_state = AppState::STATIC;

	LED * current_output = nullptr;

	ir_init();
	led_init();

	DDRC &= ~(1 << 7);

	led_red.set(50);
	led_green.set(50);
	led_blue.set(50);

	// Enable interrupts
	sei();

	while (1) {
		switch (app_state) {
			case AppState::OFF:
				if (ir_available && ir_code == IR_CODE_YELLOW) {
					app_state = AppState::STATIC;
					led_red.on();
					led_green.on();
					led_blue.on();
					_delay_ms(UI_KEY_DELAY_MS);
				}
				break;

			case AppState::STATIC:
				if (!ir_available) {
					break;
				}
				switch (ir_code) {
					case IR_CODE_YELLOW:
						// exit adjust mode
						if (current_output) {
							current_output = nullptr;
							led_blink(0, 0, 0, UI_BLINK_MS);
//							_delay_ms(UI_KEY_DELAY_MS);
						} else {
							// next state
							app_state = AppState::OFF;
							led_red.off();
							led_green.off();
							led_blue.off();
							_delay_ms(UI_KEY_DELAY_MS);
						}
						break;

					case IR_CODE_RED:
						current_output = &led_red;
						led_blink(100, 0, 0, UI_BLINK_MS);
//						_delay_ms(UI_KEY_DELAY_MS);
						break;

					case IR_CODE_GREEN:
						current_output = &led_green;
						led_blink(0, 100, 0, UI_BLINK_MS);
//						_delay_ms(UI_KEY_DELAY_MS);
						break;

					case IR_CODE_BLUE:
						current_output = &led_blue;
						led_blink(0, 0, 100, UI_BLINK_MS);
//						_delay_ms(UI_KEY_DELAY_MS);
						break;

					case IR_CODE_CHUP:
						if (current_output) {
							current_output->inc(100);
						}
						break;

					case IR_CODE_CHDN:
						if (current_output) {
							current_output->dec(0);
						}
						break;

					default:
						break;
				}
				break;
		}
		ir_available = false;
	}
	return 0;
}

