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
uint8_t g_timer_a0_scan_flag = 0U;
uint8_t g_timer_a3_scan_flag = 0U;   /* 1 Hz scan rate */

volatile uint32_t adc0_buffer[3];      /* Ax - Thr - V_m */
volatile uint32_t adc1_buffer[3];      /* Ay -  Az - I_m */
uint32_t delta;
uint32_t wt1cpp0_tav_buffer;  /* RPM */

void UART0IntHandler(void)
{
    uint32_t ui32Status;

    ui32Status = UARTIntStatus(UART0_BASE, true); //get interrupt status
    UARTIntClear(UART0_BASE, ui32Status);         //clear the asserted interrupts

    while(UARTCharsAvail(UART0_BASE)) //loop while there are chars
    {
//    	UARTCharGetNonBlocking(UART0_BASE);
    	g_uart0_data.rx_buffer[g_uart0_data.rx_index++] = HWREG(UART0_BASE + UART_O_DR);
//    	UARTCharPutNonBlocking(UART0_BASE, 'x'); //echo character

//        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); //blink LED
//        SysCtlDelay(SysCtlClockGet() / (1000 * 3)); //delay ~1 msec
//        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); //turn off LED
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

	// g_timer_a0_scan_flag = 1U;
}


void WTimer1AIntHandler(void)
{
	volatile uint32_t ui32Timer, ui32Timer_prev, diff;

    /**************************************************************************
     * 1.11) Poll the CnMRISbit in the GPTMRIS register or wait for the
     *       interrupt to be generated (if enabled). In both cases, the status
     *       flags are cleared by writing a 1 to the CnMCINT bit of the GPTM
     *       Interrupt Clear (GPTMICR) register.
     *
     *  GPTM Interrupt Clear (GPTMICR, page 751)
     *************************************************************************/
    WTIMER1_ICR_R |= TIMER_ICR_CAMCINT;


	// Clear the timer interrupt
	//ROM_TimerIntClear(WTIMER1_BASE, TIMER_CAPA_EVENT);

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

void Timer3AIntHandler(void)
{
	/**************************************************************************
	 * 16/32-Bit General-Purpose Timer 3 is configured as a Count-Up 32-bit
	 * timer with the upper value set to 0x02625A00 (TIMER3_TAILR_R) which
	 * corresponds to 40.000.000 in decimal, that is the System Clock frequency.
	 * And running through this value, the ISR is called every 1 second.
	 * So this timer is used to generate interrupts at rate of 1 Hz in order to
	 * get the value of the counted positive input edges on the pin PC6
	 * configured as Input Edge-Count timer over the WT1CCP0.
	 *
	 *************************************************************************/
    static uint32_t tav_1 = 0U; /* Previous edge count */


    /**************************************************************************
     * Clear the timer interrupt GPTM Timer A Time-Out Raw Interrupt
     *  GPTM Interrupt Clear (GPTMICR, page 751)
     *************************************************************************/
	TIMER3_ICR_R |= TIMER_ICR_TATOCINT;

	/**************************************************************************
	 * Reads the current value of the positive input edges counted on the pin
	 * PC6. This value is cumulative and shall be subtracted from the previous
	 * value read.
	 *************************************************************************/
	wt1cpp0_tav_buffer = WTIMER1_TAV_R;

	delta = wt1cpp0_tav_buffer - tav_1;
	tav_1 = wt1cpp0_tav_buffer;

	g_timer_a3_scan_flag = 1U;


	GPIO_PORTF_DATA_R ^= GPIO_PIN_2;
}

void ADC0SS1IntHandler(void)
{
	/**************************************************************************
	 *  ADC0 - Acknowledge Sample Sequencer 1 Interrupt
	 *
	 *  ADC Interrupt Status and Clear (ADCISC, page 825)
	 *************************************************************************/
	ADC0_ISC_R = ADC_ISC_IN1;

    /**************************************************************************
     * Retrieve data from sample sequence 1 FIFO. The data, if HW averaged is
     * enabled, are available in the FIFO.
     *
     *  ADC Sample Sequence Result FIFO 1 (ADCSSFIFO1, page 857)
     *************************************************************************/
	adc0_buffer[2] = ADC0_SSFIFO1_R;	/* PE1_AIN2_V_motor */
	adc0_buffer[0] = ADC0_SSFIFO1_R;    /* PD1_AIN6_Ax      */
	adc0_buffer[1] = ADC0_SSFIFO1_R;    /* PD0_AIN7_StrGage */

	adc1_buffer[2] = ADC1_SSFIFO1_R;    /* PE2_AIN1_I_motor */
	adc1_buffer[0] = ADC1_SSFIFO1_R;    /* PD2_AIN5_Ay      */
	adc1_buffer[1] = ADC1_SSFIFO1_R;    /* PD3_AIN4_Az      */

	g_timer_a0_scan_flag = 1U;
}

void ADC1SS1IntHandler(void)
{
	/**************************************************************************
	 *  ADC1 - Acknowledge Sample Sequencer 1 Interrupt
	 *
	 *  ADC Interrupt Status and Clear (ADCISC, page 825)
	 *************************************************************************/
	ADC1_ISC_R = ADC_ISC_IN1;

    /**************************************************************************
     * Retrieve data from sample sequence 1 FIFO. The data, if HW averaged is
     * enabled, are available in the FIFO.
     *
     *  ADC Sample Sequence Result FIFO 1 (ADCSSFIFO1, page 857)
     *************************************************************************/
	adc1_buffer[0] = ADC1_SSFIFO1_R;
	adc1_buffer[1] = ADC1_SSFIFO1_R;
	adc1_buffer[2] = ADC1_SSFIFO1_R;
}
