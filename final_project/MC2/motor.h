/*
 * motor.h
 *
 *  Created on: Oct 8, 2021
 *      Author: Hussein Mohamed
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/************************************   Preprocessor    ******************************/

/* MOTOR HW Configuration */
#define A PC5
#define B PC6
#define MOTOR_PORT_DIR DDRC
#define MOTOR_PORT PORTC

/* MOTOR Commands */
#define MOTOR_stop	 		CLEAR_BIT(MOTOR_PORT,A);  CLEAR_BIT(MOTOR_PORT,B)
#define MOTOR_anti_clockw 	CLEAR_BIT(MOTOR_PORT,A);  SET_BIT(MOTOR_PORT,B)
#define MOTOR_clockw		SET_BIT(MOTOR_PORT,A);	  CLEAR_BIT(MOTOR_PORT,B)


/********************************* Global Variable and Function Prototypes ******************************/

extern volatile uint8 g_MOTOR_flag;

void MOTOR_init(void);



#endif /* MOTOR_H_ */
