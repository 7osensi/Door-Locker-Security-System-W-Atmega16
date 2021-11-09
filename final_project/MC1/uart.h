/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Hussein Mohamed
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

#define USART_BAUDRATE 9600
#define F_CPU 1000000UL

typedef enum {

	DISABLED, RESERVED, ENABLED_EVEN, ENABLED_ODD

} USART_parity_mode;

typedef enum {

	BIT_1, BIT_2

} USART_stop_bit;

typedef enum {

	BIT_5, BIT_6, BIT_7, BIT_8, BIT_9 = 7

} USART_character_size;

typedef enum {

	ASYNCH, SYNCH

} USART_mode_select;

typedef enum {

	RISING, FALLING

} USART_clock_polarity;

typedef struct {

	USART_parity_mode parity;
	USART_stop_bit stop;
	USART_character_size size;
	USART_mode_select mode;
	USART_clock_polarity polarity;

} USART_configuration;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const USART_configuration *config_ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte();

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
