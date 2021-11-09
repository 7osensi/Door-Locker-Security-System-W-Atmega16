/*
 * HMI_ECU.c
 *
 *  Created on: Nov 9, 2021
 *      Author: Hussein Mohamed
 */

#include "timer.h"
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "micro_config.h"
#include "util/delay.h"

#define DELAY_Keypad 2000
#define DELAY_UART 100

/*******************************************************************************
 *                           Function Prototype                           	   *
 *******************************************************************************/

void EnterPW(uint8 PW[]);
void SendPW_UART(uint8 PW[]);
void timer0_isr_fn(void);

/*******************************************************************************
 *                           Global Variables	                          	   *
 *******************************************************************************/

int quarter_sec = 0;

/*******************************************************************************
 *                           Main Function		                          	   *
 *******************************************************************************/

int main(void) {

	uint8 PW[4];
	uint8 confirm_pw[4];
	uint8 command;
	volatile uint8 check_pw;
	Timer0_setCallBack(timer0_isr_fn);

	LCD_init();
	LCD_clearScreen();

	/* Asynchronous, even parity and one stop bit  */
	USART_configuration UConfig =
			{ ENABLED_EVEN, BIT_1, BIT_8, ASYNCH, FALLING };
	UART_init(&UConfig);

	/* Timer configurations */
	timer_configuration T0_Configuration = { NORMAL_MODE, F_CPU_1024, TIMER0,
			0, 7812 };

	/* Initialize Timer0 */
	Timer_Init(&T0_Configuration);

	/* Enable global interrupt register */
	/* SREG |= (1 << 7); */

	/*******************************************************************************
	 *                     Entering the password for the first time                *
	 *******************************************************************************/

	do {

		LCD_clearScreen();
		LCD_displayString("Enter Password");

		/* Send Address OF password[4] TO EnterPW() */
		EnterPW(PW);
		_delay_ms(DELAY_UART);

		SendPW_UART(PW);

		/* Entering the password again */
		LCD_clearScreen();
		LCD_displayString("Re-Enter PW");

		EnterPW(confirm_pw);
		_delay_ms(DELAY_UART);

		SendPW_UART(confirm_pw);

		/* MCU2 will check if password is valid or not */
		check_pw = UART_receiveByte();

		/* Send message to user if not valid */
		if (check_pw == 0) {
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 4, "INVALID");
			_delay_ms(DELAY_Keypad);
		}
		/* If password is valid, get out of the while loop */

	} while (check_pw == 0);
	LCD_clearScreen();
	LCD_displayString("Correct");
	_delay_ms(DELAY_Keypad);

	/*******************************************************************************
	 *                             Change the password                        	   *
	 *******************************************************************************/
	while (1) {
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "- to CHANGE PW");
		LCD_displayStringRowColumn(1, 0, "+ to OPEN");

		/* Send your choice to MC2 */
		command = KEYPAD_getPressedKey();
		_delay_ms(DELAY_Keypad);
		UART_sendByte(command);
		_delay_ms(DELAY_UART);

		/********************************************************************************
		 *                             ' - ' IS PRESSED                   	        *
		 ********************************************************************************/

		if (command == '-') {
			do {
				LCD_clearScreen();
				LCD_displayString("Enter Password");

				/* Send Address OF password[4] TO EnterPW() */
				EnterPW(PW);
				_delay_ms(DELAY_UART);

				SendPW_UART(PW);

				/* Entering the password again */
				LCD_clearScreen();
				LCD_displayString("Re-Enter PW");

				EnterPW(confirm_pw);
				_delay_ms(DELAY_UART);

				SendPW_UART(confirm_pw);

				/* MCU2 will check if password is valid or not */
				check_pw = UART_receiveByte();

				/* Send message to user if not valid */
				if (check_pw == 0) {
					LCD_clearScreen();
					LCD_displayStringRowColumn(0, 4, "INVALID");
					_delay_ms(DELAY_Keypad);
				}
				/* If password is valid, get out of the while loop */

			} while (check_pw == 0);
			LCD_clearScreen();
			LCD_displayString("Password Changed");
			_delay_ms(DELAY_Keypad);
		}

		/********************************************************************************
		 *                             ' + ' IS PRESSED                   	            *
		 ********************************************************************************/

		else if (command == '+') {

			/* This variable is used to count the number of invalid entries */
			uint8 count = 0;

			do {
				count++;

				LCD_clearScreen();
				LCD_displayString("Enter Password");
				EnterPW(PW);
				SendPW_UART(PW);

				/* MCU2 will check if password is valid or not */
				check_pw = UART_receiveByte();
				_delay_ms(DELAY_UART);

				if (check_pw == 0) {
					LCD_clearScreen();
					LCD_displayStringRowColumn(0, 4, "INVALID");
					_delay_ms(DELAY_Keypad);
				}

				/* Polling */
				/* If the password has been entered wrongly for three times */
			} while (check_pw == 0 && count < 3);

			if (check_pw) {

				/* DOOR OPENS IN 15 SECONDS AND STAYS OPENED FOR 3 SECONDS AND STARTS
				 *  CLOSING AGAIN IN 15 SECONDS */

				SECONDS_T0_MC1 = 0;
				LCD_clearScreen();
				LCD_displayString("Opening Door");
				while (SECONDS_T0_MC1 <= 15)
					;
				LCD_clearScreen();
				LCD_displayString("Door Open");
				while (SECONDS_T0_MC1 <= 18)
					;
				LCD_clearScreen();
				LCD_displayString("Closing Door");
				while (SECONDS_T0_MC1 <= 33)
					;
			}
			// if password do not match so turn on buzzer
			else if (check_pw == 0) {
				LCD_clearScreen();
				LCD_displayStringRowColumn(0, 5, "ERROR");
				SECONDS_T0_MC1 = 0;

				/* Polling */
				/* Lock MC1 for 60 seconds */
				while (SECONDS_T0_MC1 < 60)
					;
			}
		}
	}

}
