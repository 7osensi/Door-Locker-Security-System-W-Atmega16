/*
 * timer.h
 *
 *  Created on: Oct 22, 2021
 *      Author: Hussein Mohamed
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"

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

/* F_CPU = 1MHz
 * Prescalar (N) = 256
 * F_timer = (F_CPU / N)
 * T_timer0 = 1 / F_timer0
 * T_overflow = 256 * T_timer0
 * No. of overflows per second = 1 / T_overflow = 15.25 overflows/second
 */
#define NUMBER_OF_OVERFLOWS_PER_SECOND_T0	15.25

/* F_CPU = 1MHz
 * Prescalar (N) = 256
 * F_timer1 = (F_CPU / N)
 * T_timer1 = 1 / F_timer1
 * T_overflow = 1024 * T_timer1
 * No. of overflows per second = 1 / T_overflow = 3.81 overflows/second
 */
#define NUMBER_OF_OVERFLOWS_PER_SECOND_T1	3.81

/* F_CPU = 1MHz
 * Prescalar (N) = 256
 * F_timer2 = (F_CPU / N)
 * T_timer2 = 1 / F_timer
 * T2_overflow = 256 * T_timer2
 * No. of overflows per second = 1 / T_overflow = 3.81 overflows/second
 */
#define NUMBER_OF_OVERFLOWS_PER_SECOND_T2	15.25

extern volatile uint32 SECONDS_T1;
extern volatile uint32 SECONDS_T0_MC1;
extern volatile uint32 SECONDS_T0_MC2;

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
	timer_prescalar clock;
	timer_number number;
	uint16 compare_value;
	uint16 initial_value;
} timer_configuration;

typedef struct {
	timer_number timerNumber;
} timer_deInt;

/********************************************************************************************************
 * 											Prototypes												    *
 *******************************************************************************************************/

/*
 * Name: Timer0_Init
 * Description: This function is used to initialize the timer.
 * Input: pointer to structure
 * Return: void
 */

void Timer_Init(const timer_configuration *config_ptr);

/*
 * Name: Timer0_deInit
 * Description: This function is used to de-initialize the timer.
 * Input: pointer to structure
 * Return: void
 */

void Timer_deInit(const timer_deInt *deInt_ptr);

/*
 * Name: Timer0_setCallBack
 * Description: Call back function.
 * Input: pointer to function
 * Return: None
 */

void Timer0_setCallBack(void (*a_ptr)(void));

#endif /* TIMER_H_ */
