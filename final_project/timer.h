/*
 * timer.h
 *
 *  Created on: Oct 22, 2021
 *      Author: Hussein Mohamed
 */

#ifndef TIMER_H_
#define TIMER_H_

/********************************************************************************************************
 * 											Definitions													*
 *******************************************************************************************************/

/* Timer/Counter Register – TCNT0 */

#define Timer0_initial_value				0
#define Timer1_initial_value				0
#define Timer2_initial_value				0

#define Timer0_compare_value 				50
#define Timer1_compare_value 				50
#define Timer2_compare_value 				50

#define F_CPU								1000000UL

/* F_CPU = 1MHz
 * Prescalar (N) = 256
 * F_timer = (F_CPU / N)
 * T_timer = 1 / F_timer
 * T_overflow = 256 * T_timer
 * No. of overflows per second = 1 / t_overflow = 15.25 overflows/second
 */
#define NUMBER_OF_OVERFLOWS_PER_SECOND_T0	15.25

typedef enum {
	NO_CLOCK, NO_PRESCALING, F_CPU_8, F_CPU_64, F_CPU_256, F_CPU_1024
} timer_prescalar;

typedef enum {
	NORMAL_MODE, CTC_MODE
} timer_mode;

typedef enum {
	TIMER0, TIMER1, TIMER2
} timer_number;

typedef struct {
	timer_mode mode;
	timer_prescalar prescalar;
	timer_number number;
} timer_configiration;
/********************************************************************************************************
 * 											Prototypes												    *
 *******************************************************************************************************/

/*
 * Name: Timer0_Init
 * Description: This function is used to initialize the timer.
 * Input: pointer to structure
 * Return: void
 */

void Timer_Init(const timer_configiration *config_ptr);

/*
 * Name: Timer0_deInit
 * Description: This function is used to de-initialize the timer.
 * Input: pointer to structure
 * Return: void
 */

void Timer_deInit(const timer_configiration *config_ptr);

#endif /* TIMER_H_ */
