/*
 * myinit.h
 *
 *  Created on: 07/03/2014
 *      Author: Rogerio
 */

#ifndef PSO_INIT_H_
#define PSO_INIT_H_

/* UART */
#define BAUD_9600         9600


void PSO_PeripheralEnable(void);
void PSO_GPIOConfig(void);
void myPWM_Init(void);
void PSO_Timers(void);
void PSO_UART0Config(void);
void myUART2Config_Init(void);
void mySysCtl_Init(void);
void myISR_Config(void);

#endif /* PSO_INIT_H_ */
