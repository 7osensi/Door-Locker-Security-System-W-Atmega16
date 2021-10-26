/*
 * timer.c
 *
 *  Created on: Oct 22, 2021
 *      Author: Hussein Mohamed
 */

#include <avr/io.h>
#include "timer.h"
#include "std_types.h"

/* Volatile because it is inside the ISR */
/* uint16 to store timer ticks */
volatile uint16 g_tick_OVF = 0;

volatile uint16 g_tick_COMP = 0;

/* The enum is used to store the timers modes value (NM or CTC) */
enum Timers_modes {
	mode_timer0, mode_timer1, mode_timer2
} mode;

ISR(TIMER0_OVF_vect) {

	g_tick_OVF++;
}

ISR(TIMER0_COMP_vect) {

	g_tick_COMP++;
}

void Timer_Init(const timer_configiration *config_ptr) {

	/* Enable global interrupts in MC */
	SREG = (1 << 7);

	if (config_ptr->number == TIMER0) {

		/* Initial Value */
		TCNT0 = Timer0_initial_value;

		/* The FOC0 bit is only active when the WGM00 bit specifies a non-PWM mode. */
		TCCR0 = (1 << FOC0);

		/* This variable is used to store the mode value (NM or CTC) */
		mode_timer0 = config_ptr->mode;

		/* TCCR0: Bit 2:0 – CS02:0: Clock Select */
		TCCR0 = (TCCR0 & 0xF8) | (config_ptr->prescalar);

		if (mode_timer0 == CTC_MODE) {

			/* In register TCCR0: Bit 3, 6 – WGM01:0: Waveform Generation Mode */
			TCCR0 = (TCCR0 & 0x7) | ((config_ptr->mode) << 3);

			/* Set compare value */
			OCR0 = Timer0_compare_value;

			/* Bit 1 – OCIE0: Timer/Counter0 Output Compare Match Interrupt Enable */
			TIMSK = (1 << OCIE0);

		} else if (mode_timer0 == NORMAL_MODE) {

			/* In register TCCR0: Bit 3, 6 – WGM01:0: Waveform Generation Mode */
			TCCR0 = (TCCR0 & 0xB7) | ((config_ptr->mode) << 3);

			/* Bit 0 – TOIE0: Timer/Counter0 Overflow Interrupt Enable */
			TIMSK |= (1 << TOIE0);

			TCCR0 &= ~(1 << COM00) & ~(1 << COM01); /* Normal mode */

		}
	} else if (config_ptr->number == TIMER1) {

		/* Initial Value */
		TCNT1 = Timer0_initial_value;

		/* The FOC0 bit is only active when the WGM00 bit specifies a non-PWM mode. */
		TCCR1A = (1 << FOC1A);

		/* This variable is used to store the mode value (NM or CTC) */
		mode_timer1 = config_ptr->mode;

		/*  Bit 2:0 – CS12:0: Clock Select */
		TCCR1B = (TCCR1B & 0xF8) | (config_ptr->prescalar);

		if (mode_timer1 == CTC_MODE) {

			/* In register TCCR0: Bit 3, 6 – WGM01:0: Waveform Generation Mode */
			TCCR1B = (TCCR1B & 0xE7) | ((config_ptr->mode) << 3);

			/* Set compare value */
			OCR1A = Timer1_compare_value;

			/*  Bit 4 – OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable */
			TIMSK = (1 << OCIE1A);

		} else if (mode_timer1 == NORMAL_MODE) {

			/*  Bit 1:0 – WGM11:0: Waveform Generation Mode */
			TCCR1A = (TCCR1A & 0xFC) | (config_ptr->mode);

			/* Bit 4:3 – WGM13:2: Waveform Generation Mode */
			TCCR1A = (TCCR1A & 0xE7) | ((config_ptr->mode) << 4);

			/*  Bit 2 – TOIE1: Timer/Counter1, Overflow Interrupt Enable */
			TIMSK |= (1 << TOIE1);

			TCCR1A &= ~(1 << COM1A1) & ~(1 << COM1A0); /* Normal mode */

		}

	} else if (config_ptr->number == TIMER2) {

		/* Initial Value */
		TCNT2 = Timer0_initial_value;

		/*  Bit 7 – FOC2: Force Output Compare */
		TCCR2 = (1 << FOC2);

		/* This variable is used to store the mode value (NM or CTC) */
		mode_timer2 = config_ptr->mode;

		/*  Bit 2:0 – CS22:0: Clock Select */
		TCCR2 = (TCCR2 & 0xF8) | (config_ptr->prescalar);

		if (mode_timer2 == CTC_MODE) {

			/*  Bit 3, 6 – WGM21:0: Waveform Generation Mode */
			TCCR2 = (TCCR2 & 0xB7) | ((config_ptr->mode) << 3);

			/* Set compare value */
			OCR2 = Timer2_compare_value;

			/*  Bit 7 – OCIE2: Timer/Counter2 Output Compare Match Interrupt Enable */
			TIMSK = (1 << OCIE2);

		} else if (mode_timer0 == NORMAL_MODE) {

			/* Bit 3, 6 – WGM21:0: Waveform Generation Mode */
			TCCR2 = (TCCR2 & 0xB7) | ((config_ptr->mode) << 3);

			/* Bit 6 – TOIE2: Timer/Counter2 Overflow Interrupt Enable */
			TIMSK |= (1 << TOIE2);

			/*  Bit 5:4 – COM21:0: Compare Match Output Mode */
			TCCR2 &= ~(1 << COM20) & ~(1 << COM21); /* Normal mode */

		}

	}
}

void Timer_deInit(const timer_configiration *config_ptr) {

	if (config_ptr->number == TIMER0) {

		/* Bit 0 – TOIE0: Timer/Counter0 Overflow Interrupt Disable */
		TIMSK &= ~(1 << TOIE0);

		/* The FOC0 bit is only active when the WGM00 bit specifies a non-PWM mode
		 * TCCR0: Bit 3, 6 – WGM01:0: Waveform Generation Mode
		 * Bit 2:0 – CS02:0: Clock Select
		 */
		TCCR0 = 0;

		/* Compare value */
		OCR0 = 0;

		/* Initial Value */
		TCNT0 = 0;

	} else if (config_ptr->number == TIMER1) {

		/* Bit 4 – OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Disable
		 * Bit 3 – OCIE1B: Timer/Counter1, Output Compare B Match Interrupt Disable
		 *Bit 2 – TOIE1: Timer/Counter1, Overflow Interrupt Enable/
		 TIMSK &= ~(1 << OCIE1A) & ~(1 << OCIE1B) & ~(1 <<  TOIE1);

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

	} else if (config_ptr->number == TIMER2) {

		/*  Bit 7 – OCIE2: Timer/Counter2 Output Compare Match Interrupt Disable
		 *  Bit 6 – TOIE2: Timer/Counter2 Overflow Interrupt  Disable */
		TIMSK &= ~(1 << OCIE2) & ~(1 << OCIE2);

		/* Bit 7 – FOC2: Force Output Compare
		 * Bit 3, 6 – WGM21:0: Waveform Generation Mode
		 *  Bit 2:0 – CS22:0: Clock Select
		 */
		TCCR2 = 0;

		/* Compare value */
		OCR2 = 0;

		/* Initial value */
		TCNT2 = 0;
	}
}

