/*
 * isr.c
 *
 *  Created on: 10/03/2014
 *      Author: Rogerio
 */

#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "sysctl.h"
#include "gpio.h"
#include "debug.h"
#include "pwm.h"
#include "pin_map.h"
#include "hw_gpio.h"
#include "rom.h"
#include "interrupt.h"
#include "timer.h"
#include "uart.h"
#include "hw_uart.h"
#include "pso_init.h"
#include "pso_uart.h"
#include "hw_ints.h"

extern uart_raw_data_t g_uart0_data; /*Defined in "pos_uart.c" */

void UART0IntHandler(void)
{
    uint32_t ui32Status;

    ui32Status = UARTIntStatus(UART0_BASE, true); //get interrupt status
    UARTIntClear(UART0_BASE, ui32Status);         //clear the asserted interrupts

    while(UARTCharsAvail(UART0_BASE)) //loop while there are chars
    {
//    	UARTCharGetNonBlocking(UART0_BASE);
    	g_uart0_data.rx_buffer[g_uart0_data.rx_index++] = HWREG(UART0_BASE + UART_O_DR);
    	UARTCharPutNonBlocking(UART0_BASE, 'x'); //echo character

        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); //blink LED
        SysCtlDelay(SysCtlClockGet() / (1000 * 3)); //delay ~1 msec
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); //turn off LED
    }

    g_uart0_data.new_data = 1;
}


void Timer0AIntHandler(void)
{
	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	// Read the current state of the GPIO pin and
	// write back the opposite state
//	if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3))
//	{
//		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
//	}
//	else
//	{
//		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
//	}
}


void WTimer1AIntHandler(void)
{
	volatile uint32_t ui32Timer, ui32Timer_prev, diff;

	// Clear the timer interrupt
	ROM_TimerIntClear(WTIMER1_BASE, TIMER_CAPA_EVENT);

//	ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);	// Turn on blue LED

//	ui32Timer = ROM_TimerValueGet(WTIMER1_BASE, TIMER_A);
//	diff = ui32Timer - ui32Timer_prev;
//	ui32Timer_prev = ui32Timer;
}


void WTimer1BIntHandler(void)
{
	// Clear the timer interrupt
	ROM_TimerIntClear(WTIMER1_BASE, TIMER_CAPB_EVENT);

//	ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 8);	// Turn on green LED

}

void WTimer5AIntHandler(void)
{
	// Clear the timer interrupt
	ROM_TimerIntClear(WTIMER5_BASE, TIMER_CAPA_EVENT);

//	ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 2);	// Turn on red LED

}


void WTimer5BIntHandler(void)
{
	// Clear the timer interrupt
	ROM_TimerIntClear(WTIMER5_BASE, TIMER_CAPB_EVENT);

//	ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 14);	// Turn on all LEDs

}
