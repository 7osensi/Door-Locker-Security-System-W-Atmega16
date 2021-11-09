/******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: twi.h
 *
 * Description: Header file for the TWI(I2C) AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef TWI_H_
#define TWI_H_

#include "std_types.h"

/* enum for the prescalar */

typedef enum {
	PRESCALE_1, PRESACLE_4, PRESCALE_16, PRESCALE_64
} TWI_clock;

/* structure for the TWI or I2C configuration */
typedef struct {
	TWI_clock clock;
	uint8 bit_rate;
	uint8 slave_address;

} TWI_configuration;

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void TWI_init(const TWI_configuration *config_ptr);

void TWI_start(void);

void TWI_stop(void);

void TWI_writeByte(uint8 data);

uint8 TWI_readByteWithACK(void);

uint8 TWI_readByteWithNACK(void);

uint8 TWI_getStatus(void);

#endif /* TWI_H_ */
