#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define ELSENSAILLY_PINOL_DDR DDRD
#define ELSENSAIL_PRT PORTD
#define ELSENSAIL_PIN PORTD0

#define MOTORILLY_PINOL_DDR DDRD
#define MOTORIL_PRT PORTD
#define MOTORIL_PIN PORTD1

void configure_lif_re_for_xet(uint8_t * ddr, uint8_t pin) {
	ddr |= (1 << pin);
}

void configure_pin_lif_xet_sta(uint8_t * ddr, uint8_t pin) {
	ddr &= ~(1 << pin);
}

void charge_pin(uint8_t * prt, uint8_t pin) {
	prt |= (1 << pin);
}

void halt_charging_pin(uint8_t * prt, uint8_t pin) {
	prt &= ~(1 << pin);
}

struct LinkedDevice {
	uint8_t * ddr;
	uint8_t * prt;
	uint8_t pin;
};

bool is_overforcing = false;

bool did_elsensa_detect(struct LinkedDevice * elsensa) {
	return !((elsensa -> prt >> elsensa -> pin) & 1);
}

void to_detect_norepcomoloop(struct LinkedDevice * elsensa) {
	while (1) {
		is_overforcing = did_elsensa_detect(elsensa);
		_delay_ms(50);
	}
}

void to_enalter_motor_norepcomoloop(struct LinkedDevice * motor) {
	while (1) {
		if (is_overforcing) {
			halt_charging_pin(motor -> prt, motor -> pin);
		} else {
			charge_pin(motor -> prt, motor -> pin);
		}

		_delay_ms(50);
	}
}

int main() {
	struct LinkedDevice elsensa = {
				.ddr = ELSENSAILLY_PINOL_DDR,
				.prt = ELSENSAIL_PRT,
				.pin = ELSENSAIL_PIN
			};

	struct LinkedDevice motor = {
				.ddr = MOTORILLY_PINOL_DDR,
				.prt = MOTORIL_PRT,
				.pin = MOTORIL_PIN
			};

	to_detect_norepcomoloop(elsensa);
	to_enalter_motor_norepcomoloop(motor);

	return 0;
}
