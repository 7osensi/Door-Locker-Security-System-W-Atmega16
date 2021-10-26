/*
 * buzzer.c
 *
 *  Created on: Oct 22, 2021
 *      Author: Hussein Mohamed
 */

#include "buzzer.h"
#include "std_types.h"
#include <util/delay.h>

void buzzer_INIT(void) {

	GPIO_setupPinDirection(buzzer_port, buzzer_pin, PIN_OUTPUT);
}

void buzzer_on(void) {

	GPIO_writePin(buzzer_port, buzzer_pin, LOGIC_HIGH);
}

void buzzer_off(void) {

	GPIO_writePin(buzzer_port, buzzer_pin, LOGIC_LOW);
}

/* Toogle the buzzer every half second */
void buzzer_emergency(void) {

	_delay_ms(500);
	GPIO_writePin(buzzer_port, buzzer_pin, LOGIC_HIGH);
	_delay_ms(500);
	GPIO_writePin(buzzer_port, buzzer_pin, LOGIC_HIGH);
}
