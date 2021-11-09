/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver
 *
 * Author: Hussein Mohamed
 *
 *******************************************************************************/

#include "uart.h"
#include "avr/io.h" /* To use the UART Registers */
#include "common_macros.h" /* To use the macros like SET_BIT */
#include "gpio.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const USART_configuration *config_ptr) {

	uint16 ubrr_value_u2x = 0, ubrr_value_synch = 0;

	/* Bit 4 – RXEN: Receiver Enable && Bit 3 – TXEN: Transmitter Enable */
	UCSRB = (1 << RXEN) | (1 << TXEN);

	/* SELECTING UCSRC BY SETTING 1 TO 'URSEL' */
	UCSRC |= (1 << URSEL);

	/* DATA TRANSMITTED ON FALLING EDGE , AND SAMPLED ON POSITIVE EDGE || DATA TRANSMITTED ON FALLING EDGE , AND SAMPLED ON POSITIVE EDGE */
	UCSRC = (UCSRC & 0xFE) | (config_ptr->polarity);

	/* Bit 6 – UMSEL: USART Mode Select */
	UCSRC = (UCSRC & 0xBF) | (config_ptr->mode << 6);

	/* Bit 5:4 – UPM1:0: Parity Mode */
	UCSRC = (UCSRC & 0xCF) | (config_ptr->parity << 4);

	/* Bit 3 – USBS: Stop Bit Select */
	UCSRC = (UCSRC & 0xF7) | (config_ptr->stop);

	/* Bit 2:1 – UCSZ1:0: Character Size */
	UCSRC = (UCSRC & 0xF9) | (config_ptr->size);

	if ((*config_ptr).size == BIT_9) {

		/* Bit 1 – RXB8: Receive Data Bit 8 */
		GPIO_readPin(UCSRB, RXB8);

		/* Bit 0 – TXB8: Transmit Data Bit 8 */
		GPIO_writePin(UCSRB, TXB8, LOGIC_HIGH);

		/* Bit 2 – UCSZ2: Character Size */
		UCSRB |= (1 << UCSZ2);

	}

	if ((*config_ptr).mode == ASYNCH) {

		/* U2X = 1 for double transmission speed */
		UCSRA = (1 << U2X);

		/* Bit 15 – URSEL: Register Select */
		UBRRH |= (1 << URSEL);

		/* Calculate the UBRR register value */
		ubrr_value_u2x = (uint16) (((F_CPU / (USART_BAUDRATE * 8UL))) - 1);

		/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
		UBRRH = ubrr_value_u2x >> 8;
		UBRRL = ubrr_value_u2x;

	}

	if ((*config_ptr).mode == SYNCH) {

		/* Bit 15 – URSEL: Register Select */
		UBRRH |= (1 << URSEL);

		ubrr_value_synch = (uint16) (((F_CPU / (USART_BAUDRATE * 16UL))) - 1);

		/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
		UBRRH = ubrr_value_synch >> 8;
		UBRRL = ubrr_value_synch;

	}

}

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data) {
	/*
	 * UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one
	 */
	while (BIT_IS_CLEAR(UCSRA, UDRE)) {
	}

	/*
	 * Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now
	 */
	UDR = data;

	/************************* Another Method *************************
	 UDR = data;
	 while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transmission is complete TXC = 1
	 SET_BIT(UCSRA,TXC); // Clear the TXC flag
	 *******************************************************************/
}

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void) {
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	while (BIT_IS_CLEAR(UCSRA, RXC)) {
	}

	/*
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
	return UDR;
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str) {
	uint8 i = 0;

	/* Send the whole string */
	while (Str[i] != '\0') {
		UART_sendByte(Str[i]);
		i++;
	}
	/************************* Another Method *************************
	 while(*Str != '\0')
	 {
	 UART_sendByte(*Str);
	 Str++;
	 }
	 *******************************************************************/
}

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str) {
	uint8 i = 0;

	/* Receive the first byte */
	Str[i] = UART_recieveByte();

	/* Receive the whole string until the '#' */
	while (Str[i] != '#') {
		i++;
		Str[i] = UART_recieveByte();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	Str[i] = '\0';
}
