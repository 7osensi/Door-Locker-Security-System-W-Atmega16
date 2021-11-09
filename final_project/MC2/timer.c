/*
 * timer.c
 *
 *  Created on: Oct 22, 2021
 *      Author: Hussein Mohamed
 */

#include <avr/io.h>
#include "timer.h"
#include "std_types.h"

/* Defined static to be file scoped, and volatile because it is called in ISR  */
static volatile void (*timer0_ovf_ptr)(void) = NULL; /*pointer to timer0 overflow function*/
static volatile void (*timer0_comp_ptr)(void) = NULL; /*pointer to timer0 compare function*/
static volatile void (*timer1_ovf_ptr)(void) = NULL; /*pointer to timer1 overflow function*/
static volatile void (*timer1_compa_ptr)(void) = NULL; /*pointer to timer1 compare a function*/
static volatile void (*timer1_compb_ptr)(void) = NULL; /*pointer to timer1 compare b function*/
static volatile void (*timer2_ovf_ptr)(void) = NULL; /*pointer to timer2 overflow function*/
static volatile void (*timer2_comp_ptr)(void) = NULL; /*pointer to timer0 compare function*/

ISR(TIMER0_OVF_vect) {

	if (timer0_ovf_ptr != NULL) {
		(*timer0_ovf_ptr)();
	}
}

ISR(TIMER0_COMP_vect) {

	if (timer0_comp_ptr != NULL) {
		(*timer0_comp_ptr)();
	}
}

ISR(TIMER1_OVF_vect) {

	if (timer1_ovf_ptr != NULL) {
		(*timer1_ovf_ptr)();
	}
}

/*timer1 compare a ISR*/
ISR(TIMER1_COMPA_vect) {
	if (timer1_compa_ptr != NULL) {
		(*timer1_compa_ptr)();
	}
}

/*timer1 compare b ISR*/
ISR(TIMER1_COMPB_vect) {
	if (timer1_compb_ptr != NULL) {
		(*timer1_compb_ptr)();
	}
}

/*timer2 overflow ISR*/
ISR(TIMER2_OVF_vect) {
	if (timer2_ovf_ptr != NULL) {
		(*timer2_ovf_ptr)();
	}
}

/*timer2 compare ISR*/
ISR(TIMER2_COMP_vect) {
	if (timer2_comp_ptr != NULL) {
		(*timer2_comp_ptr)();
	}
}

/***************************************************Call Back Functions For ISR***************************************************/
/*timer0 overflow Call Back Function*/
void TIMER0_OVF_interrupt(void (*a_ptr)(void)) {
	timer0_ovf_ptr = a_ptr;
}

/*timer0 compare Call Back Function*/
void TIMER0_COMP_interrupt(void (*a_ptr)(void)) {
	timer0_comp_ptr = a_ptr;
}

/*timer1 overflow Call Back Function*/
void TIMER1_OVF_interrupt(void (*a_ptr)(void)) {
	timer1_ovf_ptr = a_ptr;
}

/*timer1 compare A Call Back Function*/
void TIMER1_COMPA_interrupt(void (*a_ptr)(void)) {
	timer1_compa_ptr = a_ptr;
}

/*timer1 compare B Call Back Function*/
void TIMER1_COMPB_interrupt(void (*a_ptr)(void)) {
	timer1_compb_ptr = a_ptr;
}

/*timer2 overflow Call Back Function*/
void TIMER2_OVF_interrupt(void (*a_ptr)(void)) {
	timer2_ovf_ptr = a_ptr;
}

/*timer02 compare Call Back Function*/
void TIMER2_COMP_interrupt(void (*a_ptr)(void)) {
	timer2_comp_ptr = a_ptr;
}

/***************************************************************Timer_Init***************************************************************************/

void Timer_Init(const timer_configuration *config_ptr) {

	/* Enable global interrupts in MC */
	SREG = (1 << 7);

	if ((*config_ptr).number == TIMER0) {

		/* Initial Value */
		TCNT0 = Timer0_initial_value;

		/* The FOC0 bit is only active when the WGM00 bit specifies a non-PWM mode. */
		TCCR0 = (1 << FOC0);

		/* TCCR0: Bit 2:0 – CS02:0: Clock Select */
		TCCR0 = (TCCR0 & 0xF8) | (config_ptr->clock);

		if ((*config_ptr).mode == CTC_MODE) {

			/* TCCR0: Bit 3, 6 – WGM01:0: Waveform Generation Mode */
			TCCR0 = (TCCR0 & 0xB7) | ((config_ptr->mode) << 3);

			/* Set compare value */
			OCR0 = Timer0_initial_value;

			/* Bit 1 – OCIE0: Timer/Counter0 Output Compare Match Interrupt Enable */
			TIMSK = (1 << OCIE0);

		} else if ((*config_ptr).mode == NORMAL_MODE) {

			/* TCCR0: Bit 3, 6 – WGM01:0: Waveform Generation Mode */
			TCCR0 = (TCCR0 & 0xB7) | ((config_ptr->mode) << 3);

			/* Bit 0 – TOIE0: Timer/Counter0 Overflow Interrupt Enable */
			TIMSK |= (1 << TOIE0);

			TCCR0 &= ~(1 << COM00) & ~(1 << COM01); /* Normal mode */

		}
	} else if ((*config_ptr).mode == TIMER1) {

		/* Initial Value */
		TCNT1 = Timer1_initial_value;

		/* The FOC0 bit is only active when the WGM00 bit specifies a non-PWM mode. */
		TCCR1A = (1 << FOC1A);

		/*  Bit 2:0 – CS12:0: Clock Select */
		TCCR1B = (TCCR1B & 0xF8) | (config_ptr->clock);

		if ((*config_ptr).mode == CTC_MODE) {

			/* In register TCCR0: Bit 3, 6 – WGM01:0: Waveform Generation Mode */
			TCCR1B = (TCCR1B & 0xE7) | ((config_ptr->mode) << 3);

			/* Set compare value */
			OCR1A = Timer1_compare_value;

			/*  Bit 4 – OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable */
			TIMSK = (1 << OCIE1A);

		} else if ((*config_ptr).mode == NORMAL_MODE) {

			/*  Bit 1:0 – WGM11:0: Waveform Generation Mode */
			TCCR1A = (TCCR1A & 0xFC) | (config_ptr->mode);

			/* Bit 4:3 – WGM13:2: Waveform Generation Mode */
			TCCR1A = (TCCR1A & 0xE7) | ((config_ptr->mode) << 4);

			/*  Bit 2 – TOIE1: Timer/Counter1, Overflow Interrupt Enable */
			TIMSK |= (1 << TOIE1);

			TCCR1A &= ~(1 << COM1A1) & ~(1 << COM1A0); /* Normal mode */

		}

	} else if ((*config_ptr).number == TIMER2) {

		/* Initial Value */
		TCNT2 = Timer2_initial_value;

		/*  Bit 7 – FOC2: Force Output Compare */
		TCCR2 = (1 << FOC2);

		/*  Bit 2:0 – CS22:0: Clock Select */
		TCCR2 = (TCCR2 & 0xF8) | (config_ptr->clock);

		if ((*config_ptr).mode == CTC_MODE) {

			/*  Bit 3, 6 – WGM21:0: Waveform Generation Mode */
			TCCR2 = (TCCR2 & 0xB7) | ((config_ptr->mode) << 3);

			/* Set compare value */
			OCR2 = Timer2_compare_value;

			/*  Bit 7 – OCIE2: Timer/Counter2 Output Compare Match Interrupt Enable */
			TIMSK = (1 << OCIE2);

		} else if ((*config_ptr).mode == NORMAL_MODE) {

			/* Bit 3, 6 – WGM21:0: Waveform Generation Mode */
			TCCR2 = (TCCR2 & 0xB7) | ((config_ptr->mode) << 3);

			/* Bit 6 – TOIE2: Timer/Counter2 Overflow Interrupt Enable */
			TIMSK |= (1 << TOIE2);

			/*  Bit 5:4 – COM21:0: Compare Match Output Mode */
			TCCR2 &= ~(1 << COM20) & ~(1 << COM21); /* Normal mode */

		}

	}
}

void Timer_deInit(const timer_deInt *deInt_ptr) {

	if ((*deInt_ptr).timerNumber == TIMER0) {

		/* The FOC0 bit is only active when the WGM00 bit specifies a non-PWM mode
		 * TCCR0: Bit 3, 6 – WGM01:0: Waveform Generation Mode
		 * Bit 2:0 – CS02:0: Clock Select
		 */
		TCCR0 = 0;

	} else if ((*deInt_ptr).timerNumber == TIMER1) {

		/* Bit 4 – OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Disable
		 * Bit 3 – OCIE1B: Timer/Counter1, Output Compare B Match Interrupt Disable
		 *Bit 2 – TOIE1: Timer/Counter1, Overflow Interrupt Enable*/
		TIMSK &= ~(1 << TOIE1);

		/* Bit 3 – FOC1A: Force Output Compare for Channel A
		 * Bit 2 – FOC1B: Force Output Compare for Channel B
		 * Bit 1:0 – WGM11:0: Waveform Generation Mode
		 */
		TCCR1A = 0;

		/* TCCR1B: Bit 2:0 – CS12:0: Clock Select */
		TCCR1B = 0;

		/* Compare value */
		OCR1A = 0;
		OCR1B = 0;

		/* Initial value */
		TCNT1 = 0;

	} else if ((*deInt_ptr).timerNumber == TIMER2) {

		/* Bit 7 – FOC2: Force Output Compare
		 * Bit 3, 6 – WGM21:0: Waveform Generation Mode
		 *  Bit 2:0 – CS22:0: Clock Select
		 */
		TCCR2 = 0;

	}
}
