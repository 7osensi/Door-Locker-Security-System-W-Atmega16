/*
 * buzzer.h
 *
 *  Created on: Oct 22, 2021
 *      Author: Hussein Mohamed
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"

/**********************************************************************************************************
 * 										     Definitions
 *********************************************************************************************************/

#define buzzer_pin			PIN0_ID
#define buzzer_port			PORTB_ID

/**********************************************************************************************************
 * 										     Prototypes
 *********************************************************************************************************/
/*
 * Definition: This function is used to initialize the buzzer driver.
 * Input: void.
 * Return: void.
 */
void buzzer_INIT(void);

/*
 * Definition: This function is used to turn on the buzzer.
 * Input: void.
 * Return: void.
 */
void buzzer_on(void);

/*
 * Definition: This function is used to turn off the buzzer.
 * Input: void.
 * Return: void.
 */
void buzzer_off(void);

/*
 * Definition: This function is used to toggle the buzzer.
 * Input: void.
 * Return: void.
 */
void buzzer_emergency(void);

#endif /* BUZZER_H_ */
