/*
 * buzzer.c
 *
 *  Created on: Oct 21, 2021
 *      Author: Hussein Mohamed
 */

#include "buzzer.h"
#include "gpio.h"

void buzzer_init(void) {

	GPIO_setupPinDirection(PORTD_ID, PIN6_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORTD_ID, PIN7_ID, PIN_OUTPUT);//PD6 AND PD7 OUTPUT PINS FOR THE BUZZER
	GPIO_writePin(PORTD_ID, PIN6_ID, LOGIC_LOW);
	GPIO_writePin(PORTD_ID, PIN7_ID, LOGIC_LOW);// BY DEFAULT OUTPUT PINS ARE 0

}

void buzzer_start(void) {

	GPIO_writePin(PORTD_ID, PIN6_ID, LOGIC_HIGH);
	GPIO_writePin(PORTD_ID, PIN7_ID, LOGIC_LOW);

}

void buzzer_stop(void) {

	GPIO_writePin(PORTD_ID, PIN6_ID, LOGIC_LOW);

}
