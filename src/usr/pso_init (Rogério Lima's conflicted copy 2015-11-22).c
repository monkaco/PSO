/*
 * myinit.c
 *
 *  Created on: 07/03/2014
 *      Author: Rogerio
 */

#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_sysctl.h"
#include "hw_uart.h"
#include "sysctl.h"
#include "gpio.h"
#include "debug.h"
#include "pwm.h"
#include "pin_map.h"
#include "hw_gpio.h"
#include "rom.h"
#include "interrupt.h"
#include "timer.h"
#include "pso_init.h"
#include "uart.h"


/******************************************************************************
 * Interrupt Service Routine (ISR) Configuration
 * ----------------------------------------------------------------------------
 * UART 0 -> Port A -> Pins PA0(Tx) & PA1(Rx)
 * 9600-8N1
 *
 ******************************************************************************/
void myISR_Config()
{

}



/******************************************************************************
 * UART-0 Configuration VCP (Virtual Com Port)
 * ----------------------------------------------------------------------------
 * UART 0 -> Port A -> Pins PA0(Tx) & PA1(Rx)
 * 9600-8N1
 *
 ******************************************************************************/
void PSO_UART0Config()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);	/* Enable UART Module 0 */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);	/* Enable GPIO Port A for UART-0 use */

	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
	                    UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
	                    UART_CONFIG_PAR_NONE);

	/* UART-0 interrupt configuration */
	UARTDisable(UART0_BASE);
	UARTFIFOEnable(UART0_BASE);

	IntEnable(INT_UART0);

	UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8, UART_FIFO_RX7_8);

	UARTIntEnable(UART0_BASE, UART_INT_RX);  // | UART_INT_RT
	UARTEnable(UART0_BASE);
	UARTFIFOEnable(UART0_BASE);
}


/******************************************************************************
 * UART-2 Configuration
 * ----------------------------------------------------------------------------
 * UART 2 -> Port B -> Pins PD6 & PD7
 * SysClk = 16 MHz
 * 9600-8N1
 *
 ******************************************************************************/
void myUART2Config_Init()
{
	HWREG(SYSCTL_RCGCUART) = SYSCTL_RCGCUART_R2;        /* UART 2 activated  */
	HWREG(SYSCTL_RCGCGPIO) = SYSCTL_RCGCGPIO_R3;        /* Port D activated  */
	HWREG(UART2_BASE + UART_O_CTL) &= ~UART_CTL_UARTEN; /* Disable UART      */
	/*-------------------------------------------------------------------------
	 * Baud-Rate Generation
	 * BRD  =  BRDI  +  BRDF  =  UARTSysClk  /  (ClkDiv  *  Baud  Rate)
	 *         BRDI -> Integer number
	 *         BRDF -> Fractional number = round(BRDF * 64 + 0.5)
	 *
	 * Baud-Rate = 9600 bps
	 * BRDI + BRDF = 16000000/(5 * 9600) = 3333.3333
	 *     BRDI = 3333 = 0x0D05
	 *     BRDF = (0.3333 * 64 + 0.5) = 26
	 *
	 * --------------------------------------------------------------------- */
	HWREG(UART2_BASE + UART_O_IBRD) = 0x00000D05;
    HWREG(UART2_BASE + UART_O_FBRD) = 0x0000001A;

    /* UART Line Control */
    HWREG(UART2_BASE + UART_O_LCRH) = UART_CONFIG_WLEN_8|UART_CONFIG_PAR_NONE|UART_CONFIG_STOP_ONE;

    HWREG(UART2_BASE + UART_O_CTL) = UART_CTL_UARTEN; /* Enable UART */

    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK)   = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR)    |= (GPIO_PIN_6 | GPIO_PIN_7);
    HWREG(GPIO_PORTD_BASE + GPIO_O_AFSEL) |= (GPIO_PIN_6 | GPIO_PIN_7);
    HWREG(GPIO_PORTD_BASE + GPIO_O_DR2R)  |= GPIO_STRENGTH_2MA;
    HWREG(GPIO_PORTD_BASE + GPIO_O_DEN)   |= (GPIO_PIN_6 | GPIO_PIN_7);
    HWREG(GPIO_PORTD_BASE + GPIO_O_PCTL)  |= 0x11000000;

    /* Terminar a inicialização na mão... */

	/* Enable UART Module 1 */
    //ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);

	/* Sets the pins for UART use */
    //ROM_GPIOPinTypeUART(UART1_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    /* Configures the pin function mux */
    //GPIOPinConfigure(GPIO_PB0_U1RX);

	/* Disables UART prior configuration */
	//ROM_UARTDisable(UART1_BASE);

    /* Sets up the serial protocol: 9600-8N1 */
 //   ROM_UARTConfigSetExpClk(UART1_BASE,
//                            SysCtlClockGet(),
//                            9600,
//                            UART_CONFIG_WLEN_8 | UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE);

    /* Enable UART after configuration */
//    UARTEnable(UART1_BASE);

    /* MODIFICAR ESSAS FUNÇÕES PARA TENTAR USAR NA CONFIGURAÇÃO
     * ----------------------------------------------------------
     * ROM_GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
     * ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
     *
     * */

}



void PSO_PeripheralEnable()
{
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);		// Enable PWM Module 0
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);		// Enable PWM Module 1
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER1);	// Enable Wide Timer 1
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER5);	// Enable Wide Timer 5
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);	// Enable GPIO Port C [PWM:PC4, PC5],[Timer: PC6, PC7]
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);	// Enable GPIO Port D [PWM:PD0, PD1],[Timer: PD6, PD7]
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);	// Enable GPIO Port F

}


void PSO_GPIOConfig()
{

	/*** Port F Configuration ***/
//	HWREG(SYSCTL_RCGCUART) = SYSCTL_RCGCGPIO_R5;        /* GPIO Port F Run Mode Clock  */
	                                                    /* Gating Control              */
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x1F;          /* Allow changes to PF4-0 */

	HWREG(SYSCTL_RCGC2) = SYSCTL_RCGC2_GPIOF;           /* GPIO Port F Run Mode Clock  */
														/* Gating Control              */
	HWREG(GPIO_PORTF_BASE + GPIO_O_DIR) |= 0x0E;        /* xxx0-1110 -> 0x0E */
	HWREG(GPIO_PORTF_BASE + GPIO_O_AFSEL) = 0x00;       /* No alternate function */
	HWREG(GPIO_PORTF_BASE + GPIO_O_DR2R) = GPIO_STRENGTH_2MA;
	HWREG(GPIO_PORTF_BASE + GPIO_O_PUR) |= 0x11;         /* Enable pull up resistors for PF0 & PF4 */
	HWREG(GPIO_PORTF_BASE + GPIO_O_DEN) |= 0x1F;         /* Enable digital pins PF4-0 */
	HWREG(GPIO_PORTF_BASE + GPIO_O_AMSEL) = 0x00;       /* Disable analog function */
	HWREG(GPIO_PORTF_BASE + GPIO_O_PCTL) = 0x00000000;  /* GPIO clear bit PCTL */
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

	/*
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0x1;
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    */
//	HWREG(SYSCTL_RCGCGPIO) = SYSCTL_RCGCGPIO_R3;        /* Port D activated  */
//	HWREG(UART2_BASE + UART_O_CTL) &= ~UART_CTL_UARTEN; /* Disable UART      */


//	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);	/* Set LED pins as output */

//	ROM_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);               /* Set buttons as input */
//	ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

//    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
//	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= (GPIO_PIN_4|GPIO_PIN_0);   // 0x01;
//   HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
//    ROM_GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
//	ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}


void myPWM_Init()
{
	GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4);	// Set Port PC4 as PWM
	GPIOPinConfigure(GPIO_PC4_M0PWM6);

	GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_5);	// Set Port PC5 as PWM
	GPIOPinConfigure(GPIO_PC5_M0PWM7);

	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);	// Set Port PD0 as PWM
	GPIOPinConfigure(GPIO_PD0_M1PWM0);

	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_1);	// Set Port PD1 as PWM
	GPIOPinConfigure(GPIO_PD1_M1PWM1);
}


void PSO_Timers()
{
	uint32_t ui32Period;

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

	ui32Period = (SysCtlClockGet() / 10) / 2;
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);

	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();

	TimerEnable(TIMER0_BASE, TIMER_A);

//	ROM_TimerControlEvent(WTIMER1_BASE,	TIMER_BOTH,TIMER_EVENT_BOTH_EDGES);
//	ROM_TimerControlEvent(WTIMER5_BASE,	TIMER_BOTH,TIMER_EVENT_BOTH_EDGES);
//
//	ROM_TimerConfigure(WTIMER1_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_CAP_TIME|TIMER_CFG_B_CAP_TIME);	// Timer 1-A and -B events edge-timer
//	ROM_TimerConfigure(WTIMER5_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_CAP_TIME|TIMER_CFG_B_CAP_TIME);	// Timer 1-A and -B events edge-timer
//	//
//	ROM_GPIOPinTypeTimer(GPIO_PORTC_BASE,GPIO_PIN_6);	// Set Port PC6 as Capture
//	ROM_GPIOPinConfigure(GPIO_PC6_WT1CCP0);
//
//	ROM_GPIOPinTypeTimer(GPIO_PORTC_BASE,GPIO_PIN_7);	// Set Port PC7 as Capture
//	ROM_GPIOPinConfigure(GPIO_PC7_WT1CCP1);
//
//	ROM_GPIOPinTypeTimer(GPIO_PORTD_BASE,GPIO_PIN_6);	// Set Port PD6 as Capture
//	ROM_GPIOPinConfigure(GPIO_PD6_WT5CCP0);
//
//	ROM_GPIOPinTypeTimer(GPIO_PORTD_BASE,GPIO_PIN_7);	// Set Port PD7 as Capture
//	ROM_GPIOPinConfigure(GPIO_PD7_WT5CCP1);
//
//	TimerSynchronize(TIMER0_BASE, WTIMER_1A_SYNC|WTIMER_1B_SYNC|WTIMER_5A_SYNC|WTIMER_5B_SYNC);
//
//	// Timer Interrupt Configuration
//	ROM_IntEnable(INT_WTIMER1A); 	// Enable Wide Timer 1-A Interrupt (Macro shall be used individually)
//	ROM_IntEnable(INT_WTIMER1B); 	// Enable Wide Timer 1-B Interrupt (Macro shall be used individually)
//	ROM_IntEnable(INT_WTIMER5A); 	// Enable Wide Timer 5-A Interrupt (Macro shall be used individually)
//	ROM_IntEnable(INT_WTIMER5B); 	// Enable Wide Timer 5-B Interrupt (Macro shall be used individually)
//	ROM_TimerIntEnable(WTIMER1_BASE, TIMER_CAPA_EVENT|TIMER_CAPB_EVENT);
//	ROM_TimerIntEnable(WTIMER5_BASE, TIMER_CAPA_EVENT|TIMER_CAPB_EVENT);
//	ROM_TimerEnable(WTIMER1_BASE, TIMER_BOTH);
//	ROM_TimerEnable(WTIMER5_BASE, TIMER_BOTH);
//	ROM_IntMasterEnable();

}
