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

	UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);  // UART_INT_RT : Timeout interrupt
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

	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGC2_GPIOF;           /* GPIO Port F Run Mode Clock  */
														/* Gating Control              */
	HWREG(GPIO_PORTF_BASE + GPIO_O_DIR) |= 0x0E;        /* xxx0-1110 -> 0x0E */
	HWREG(GPIO_PORTF_BASE + GPIO_O_AFSEL) = 0x00;       /* No alternate function */
	HWREG(GPIO_PORTF_BASE + GPIO_O_DR2R) = GPIO_STRENGTH_2MA;
	HWREG(GPIO_PORTF_BASE + GPIO_O_PUR) |= 0x11;         /* Enable pull up resistors for PF0 & PF4 */
	HWREG(GPIO_PORTF_BASE + GPIO_O_DEN) |= 0x1F;         /* Enable digital pins PF4-0 */
	HWREG(GPIO_PORTF_BASE + GPIO_O_AMSEL) = 0x00;       /* Disable analog function */
	HWREG(GPIO_PORTF_BASE + GPIO_O_PCTL) = 0x00000000;  /* GPIO clear bit PCTL */
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

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
	uint8_t freq = 200;		/* Desired frequency, [Hz] */

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

	ui32Period = (SysCtlClockGet() / freq);
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);

    TimerControlTrigger(TIMER0_BASE, TIMER_A, true); /* Trigger ADC */

	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();

	TimerEnable(TIMER0_BASE, TIMER_A);


    /*// Timer 3 config
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
	TimerConfigure(TIMER3_BASE, TIMER_CFG_PERIODIC);
    //
	timer3_period = (SysCtlClockGet() / freq3);
	TimerLoadSet(TIMER0_BASE, TIMER_A, timer3_period - 1);
    //
	IntEnable(INT_TIMER3A);
	TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	//
	TimerEnable(TIMER3_BASE, TIMER_A);
	*/

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


/******************************************************************************
 * ADC Configuration
 * ----------------------------------------------------------------------------
 * ADC XXXXXXXXXXXX
 * XXXXXXXXXXXXX
 *
 ******************************************************************************/
void PSO_ADCConfig()
{
	/*** 13.4.1 Module Initialization ***/

	/**************************************************************************
	 * 1.1) Enable the ADC clock using the  RCGADC register (page 350)
	 *************************************************************************/
	SYSCTL_RCGCADC_R |= SYSCTL_RCGCADC_R0;	/* Enable ADC Module 0   */
	SYSCTL_RCGCADC_R |= SYSCTL_RCGCADC_R1;  /* Enable ADC Module 1   */

	/**************************************************************************
	 * 1.2) Enable the clock to the appropriate GPIO modules via the RCGCGPIO
	 *    register (page 338). To find out which GPIO port to enable, refer to
	 *    "Signal Description" on page 801.
	 *    ---------------------------------------------------------------------
	 *    Port D:
	 *             PD0 -> Strain gage
	 *             PD1 -> Ax
	 *             PD2 -> Ay
	 *             PD3 -> Az
	 *    Port E:
	 *             PE1 -> V_motor
	 *             PE2 -> I_motor
	 *************************************************************************/
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;    /* Enable Port D */
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;    /* Enable Port E */

	/**************************************************************************
	 *  1.3) Set the GPIO 'AFSEL' bits for the ADC input pins (page 668). To
	 *     determine which GPIOs to configure, see Table 23-4 on page 1337.
	 *     -------------------------------------------------------------------
	 *     Inputs:
	 *             PD0
	 *             PD1
	 *             PD2
	 *             PD3
	 *             PE1
	 *             PE2
	 *************************************************************************/
	GPIO_PORTD_DIR_R &=  ~(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIO_PORTD_AFSEL_R |= (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIO_PORTE_DIR_R &=  ~(GPIO_PIN_1 | GPIO_PIN_2);
	GPIO_PORTE_AFSEL_R |= (GPIO_PIN_1 | GPIO_PIN_2);
	//GPIO_PORTD_PCTL_R


	/**************************************************************************
	 * 1.4) Configure the AINx signals to be analog inputs by clearing the
	 *    corresponding DEN bit in the GPIO Digital Enable (GPIODEN) register
	 *    (page 682).
	 *************************************************************************/
	GPIO_PORTD_DEN_R &= ~(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIO_PORTE_DEN_R &= ~(GPIO_PIN_1 | GPIO_PIN_2);

	/**************************************************************************
	 * 1.5) Disable the analog isolation circuit for all ADC input pins that
	 *      are to be used by writing a 1 to
	 *************************************************************************/
	GPIO_PORTD_AMSEL_R |= (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIO_PORTE_AMSEL_R |= (GPIO_PIN_1 | GPIO_PIN_2);

    /**************************************************************************
     * 1.6) Configure the ADC to sample at 250 ksps
     *
     * ADC Peripheral Configuration (ADCPC, page 888)
     *************************************************************************/
    ADC0_PC_R = ADC_PP_MSR_500K;
    ADC1_PC_R = ADC_PP_MSR_500K;


    /*** 13.4.2 Sample Sequencer Configuration ***/

	/**************************************************************************
	 * 2.1) Ensure that the sample sequencer is disabled by clearing the
	 *      corresponding "ASENn" bit in the "ADCACTSS" register. Programming
	 *      of the sample sequencers is allowed without having them enabled.
	 *************************************************************************/
	ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN1;   /* ADC0 SS1 Disable */
	ADC1_ACTSS_R &= ~ADC_ACTSS_ASEN1;   /* ADC1 SS1 Disable */

	/**************************************************************************
	 * 2.2) Coincident sampling of different signals. The sample sequence steps
	 *      run coincidently in both converters (see page 802).
	 *
	 * ADC Sample Phase Control (ADCSPC, page 840)
	 *************************************************************************/
	ADC0_SPC_R = ADC_SPC_PHASE_0;
	ADC1_SPC_R = ADC_SPC_PHASE_0;

    /**************************************************************************
     * 2.3) Configure sequence priority: order (highest to lowest)= 3, 2, 0, 1
     *      ADC Sample Sequencer Priority register
     *      SS3 = 0x3 (lowest)
     *      SS2 = 0x2 (mid-low)
     *      SS1 = 0x0 (highest)
     *      SS0 = 0x1 (mid-high)
     *************************************************************************/
	ADC0_SSPRI_R = 0x3201;
	ADC1_SSPRI_R = 0x3201;

	/**************************************************************************
     * 2.4) Configure the trigger event for the sample sequencer in the
     *      "ADCEMUX" register.
     *************************************************************************/
	ADC0_EMUX_R |= ADC_EMUX_EM1_TIMER;
	ADC1_EMUX_R |= ADC_EMUX_EM1_TIMER;

    /**************************************************************************
     * 2.5) Configure ADC Sample Sequence Input Multiplexer Select 1 register
     *
     *      ADC0:         Ax    Thrust    V_m
     *              -------|-------|-------|-------> t(s)
     *
     *      ADC1:         Ay      Az      I_m
     *              -------|-------|-------|-------> t(s)
     *
     *
     *      ADC0:
     *            - Step 0: AIN6 - PD1 - Accel-x
	 *            - Step 1: AIN7 - PD0 - Thrust
	 *            - Step 2: AIN2 - PE1 - V_motor
	 *
	 *      ADC1:
     *            - Step 0: AIN5 - PD2 - Accel-y
	 *            - Step 1: AIN4 - PD3 - Accel-z
	 *            - Step 2: AIN1 - PE2 - I_motor
     *************************************************************************/
	ADC0_SSMUX1_R |= ((0x6 << ADC_SSMUX1_MUX0_S) |
			          (0x7 << ADC_SSMUX1_MUX1_S) |
			          (0x2 << ADC_SSMUX1_MUX2_S));

	ADC1_SSMUX1_R |= ((0x5 << ADC_SSMUX1_MUX0_S) |
                      (0x4 << ADC_SSMUX1_MUX1_S) |
                      (0x1 << ADC_SSMUX1_MUX2_S));

    /**************************************************************************
     * 2.6) Configure Sample Sequence 1 Control
     *
     *      ADC0:
     *            - Step 0: Single-ended, No temp sensor, No interrupt
     *            - Step 1: Single-ended, No temp sensor, No interrupt
     *            - Step 2: Single-ended, No temp sensor, Interrupt, End of sequence
     *      ADC1:
     *            - Step 0: Single-ended, No temp sensor, No interrupt
     *            - Step 1: Single-ended, No temp sensor, No interrupt
     *            - Step 2: Single-ended, No temp sensor, End of sequence
     *
     * ADC Sample Sequence Control 1 register (ADCn_SSCTL1_R)
     *************************************************************************/
    ADC0_SSCTL1_R |= (ADC_SSCTL1_END2 | ADC_SSCTL1_IE2);

    ADC1_SSCTL1_R |= ADC_SSCTL1_END2;              /* Interrupt only in ADC0 */

    /**************************************************************************
     * 2.7) Enable Hardware Averaging Circuit for 4x./
     *
     *
     *  ADC Sample Averaging Control (ADCSAC, page 844)
     *************************************************************************/
    ADC0_SAC_R |= ADC_SAC_AVG_4X;                /* 4x hardware oversampling */
    ADC1_SAC_R |= ADC_SAC_AVG_4X;                /* 4x hardware oversampling */

    /**************************************************************************
     *  2.8) Enable the interrupt for sample sequence 1 (only for ADC0 here)
     *
     *  ADC Interrupt Mask (ADCIM - page 822)
     *************************************************************************/
    ADC0_IM_R = ADC_IM_MASK1;

    /**************************************************************************
     * 2.9) Enable Sample Sequencer 1
     *
     *
     * ADC Active Sample Sequencer (ADCACTSS, page 818)
     *************************************************************************/
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN1;   /* ADC1 SS0 Enable */
    ADC1_ACTSS_R |= ADC_ACTSS_ASEN1;   /* ADC1 SS1 Enable */

    /**************************************************************************
     * 2.10) Enable Master Interrupt for ADC0 Sequencer 1.
     *         ADC0SS1 = 31 -> bit 15 in EN0
     *         ADC1SS1 = 65 -> bit 02 in EN2
     *
     *  Interrupt 0-31 Set Enable (EN0, page 140)
     *  Interrupt 64-95 Set Enable (EN2, page 140),
     *************************************************************************/
    NVIC_EN0_R |= 0x8000;
    NVIC_EN2_R |= 0x0002;
}

void pso_rpm_config()
{
	/**************************************************************************
	 * The RPM function is implemented using two (2) timers. One timer is
	 * configured as "Input Edge-Count Mode" in order to count all the pulses
	 * applied at the input pin. The second timer is configured as "Periodic"
	 * with a period of 1 second and in every second, it's counted the addi-
	 * tional pulses received in a window of 1 second.
	 *
	 *
	 *************************************************************************/

	/* 11.4.1 Module Initialization - Periodic Timer Mode (T3CCP0 : PB2)     */
	/**************************************************************************
	 * 1.0) Enable the Timer 3 using the  RCGGCTIMER register (page 336)
	 *      16/32-Bit General-Purpose Timer 3 Run Mode Clock Gating Control.
	 *
	 *  16/32-Bit General-Purpose Timer Run Mode Clock Gating Control
	 *  (RCGCTIMER, page 336)
	 *************************************************************************/
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3;

	/**************************************************************************
	 * 1.1) Ensure the timer is disabled (the TAEN bit in the GPTMCTL register
	 *      is cleared) before making any changes.
	 *
	 * GPTM Control (GPTMCTL, page 734)
	 *************************************************************************/
	TIMER3_CTL_R &= ~(TIMER_CTL_TAEN);  /* GPTM Timer A Disable */

	/**************************************************************************
	 * 1.2)  Write the GPTM Configuration Register (GPTMCFG) with a value of
	 *       0x0000.0000.
	 *
	 * GPTM Configuration (GPTMCFG, page 724)
	 *************************************************************************/
	TIMER3_CFG_R = TIMER_CFG_32_BIT_TIMER;  // |= TIMER_CFG_16_BIT;

	/**************************************************************************
	 * 1.3) Configure the TnMR field in the GPTM Timer n Mode Register
	 *      (GPTMTnMR):
     *      a)   Write a value of 0x1 for One-Shot mode.
     *      b)   Write a value of 0x2 for Periodic mode. <=
     *
     * 1.4)  Optionally configure the TnSNAPS, TnWOT, TnMTE, and TnCDIRbits in
     *       the GPTMTnMR register to select whether to capture the value of
     *       the free-running timer at time-out, use an external trigger to
     *       start counting, configure an additional trigger or interrupt, and
     *       count up or down.
	 *
	 *       TIMER_TAMR_TAMR_PERIOD : Periodic
	 *       TIMER_TAMR_TACDIR      : Count Up
	 *
	 *  GPTM Timer A Mode (GPTMTAMR, page 726)
	 *************************************************************************/
	TIMER3_TAMR_R |= (TIMER_TAMR_TAMR_PERIOD | TIMER_TAMR_TACDIR
			                                 | TIMER_TAMR_TAILD);


	/**************************************************************************
	 * 1.5) Load the start value into the GPTM Timer n Interval Load
	 *      Register (GPTMTnILR).
	 *
	 *  GPTM Timer A Interval Load (GPTMTAILR, page 753)
	 *************************************************************************/
	TIMER3_TAILR_R = 0x02625A00;    /* 0x02625A00 = 40.000.000 => 1s  */
                                    /* 0x003D0900 =  4.000.000 => 0.1s */

	/**************************************************************************
	 * 1.6) If interrupts are required, set the appropriate bits in the GPTM
	 *      Interrupt Mask Register (GPTMIMR).
	 *
	 * GPTM Interrupt Mask (GPTMIMR, page 742)
	 *************************************************************************/
	TIMER3_IMR_R |= TIMER_IMR_TATOIM;

	/**************************************************************************
	 * 1.7) Set the TnEN bit in the GPTMCTL register to enable the timer and
	 *      start counting.
	 *
	 * GPTM Control (GPTMCTL, page 734)
	 *************************************************************************/
	TIMER3_CTL_R |= TIMER_CTL_TAEN; /* GPTM Timer A Enable */


    /**************************************************************************
     * 1.8) Enable Master Interrupt for 16/32-Bit Timer 3A.
     *        T3CCP0 = 51 -> bit 03 in EN1
     *
     *  Interrupt 32-63 Set Enable (EN1, page 140)
     *************************************************************************/
    NVIC_EN1_R = 0x00000008;





    /*=======================================================================*/
	/* 11.4.3 Module Initialization - Input Edge-Count Mode (WT1CCP0 : PC6)  */


    /*************************************************************************
     * 0.1) Port configuration for the proper use as Input Edge-Count
     *      (WT1CCP0-PC6).
     *
     *      It's choose the alternate function for the PC6.
     *
     *      Refer to Table 23-5 (page 1344) to check the appropriate encoding
     *      for the bit fields in the register GPIOPCTL.
     *
     * GPIO Alternate Function Select (GPIOAFSEL, page 668)
     * GPIO Port Control (GPIOPCTL, page 685)
     *************************************************************************/
    GPIO_PORTC_LOCK_R  = GPIO_LOCK_KEY;      /* Unlocks the GPIO_CR register */
    GPIO_PORTC_CR_R   |= GPIO_PIN_6;         /* Allow changes to PC6         */
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGC2_GPIOC; /* GPIO Port C Run Mode Clock   */
    										 /* Gating Control               */
    GPIO_PORTC_DIR_R  &= ~(GPIO_DIR_MODE_IN | GPIO_PIN_6); /* Only for inputs*/
    GPIO_PORTC_AFSEL_R |= GPIO_PIN_6;        /* Alternate function for PC6   */
    GPIO_PORTC_PUR_R  |=  GPIO_PIN_6;    /* Enable pull up resistors for PC6 */
    GPIO_PORTC_DEN_R  |= GPIO_PIN_6;               /* Enable digital pins PC6*/
    GPIO_PORTC_AMSEL_R &= ~(GPIO_PIN_6);          /* Disable analog function */
    GPIO_PORTC_PCTL_R  |= GPIO_PCTL_PC6_WT1CCP0;       /* Port Mux Control 6 */
    GPIO_PORTC_LOCK_R = GPIO_LOCK_UNLOCKED; /* The GPIOCR register is locked */

    /**************************************************************************
	 * 1.0) Enable the Wide Timer 0 using the  RCGCWTIMER register.
	 *
	 * 32/64-Bit Wide General-Purpose Timer Run Mode Clock Gating Control
	 * (RCGCWTIMER, page 355)
	 *************************************************************************/
    SYSCTL_RCGCWTIMER_R |= SYSCTL_RCGCWTIMER_R1;

	/**************************************************************************
	 * 1.1) Ensure the timer is disabled (the TAEN bit in the GPTMCTL register
	 *      is cleared) before making any changes.
	 *
	 * GPTM Control (GPTMCTL, page 734)
	 *************************************************************************/
    WTIMER1_CTL_R &= ~(TIMER_CTL_TAEN);  /* GPTM Timer A Disable */

	/**************************************************************************
	 * 1.2)  Write the GPTM Configuration (GPTMCFG) register with a value of
	 *       0x0000.0004. For a 32/64-bit wide timer, this value selects the
	 *       32-bit timer configuration.
	 *
	 * GPTM Configuration (GPTMCFG, page 724)
	 *************************************************************************/
    WTIMER1_CFG_R |= TIMER_CFG_16_BIT;  // |= TIMER_CFG_16_BIT;

	/**************************************************************************
	 * 1.3)  In the GPTM Timer Mode (GPTMTnMR) register, write the TnCMR field
	 *       to 0x0 and the TnMR field to 0x3.
	 *
	 *   TIMER_TAMR_TAMR_M : Capture Mode
	 *   TACMR = 0         : Edge-Count Mode
	 *
	 *  GPTM Timer A Mode (GPTMTAMR, page 726)
	 *************************************************************************/
    WTIMER1_TAMR_R |= (TIMER_TAMR_TAMR_M | TIMER_TAMR_TACDIR);

    /**************************************************************************
     * 1.4) Configure the type of event(s) that the timer captures by writing
     *      the TnEVENTfield of the GPTM Control (GPTMCTL) register. Negative
     *      edge is selected for the application.
     *
     * GPTM Control (GPTMCTL, page 734)
     *************************************************************************/
    WTIMER1_CTL_R |= TIMER_CTL_TAEVENT_NEG;

    /**************************************************************************
     * 1.5) Not intended to use the GPTM Timer A Prescale
     *
     *  GPTM Timer A Prescale (GPTMTAPR, page 757)
     *************************************************************************/

	/**************************************************************************
	 * 1.6) Load the start value into the GPTM Timer n Interval Load
	 *      Register (GPTMTnILR).
	 *      Starts from 0.
	 *
	 *  GPTM Timer A Interval Load (GPTMTAILR, page 753)
	 *************************************************************************/
    WTIMER1_TAILR_R = 0xFFFFFFF0;   //TIMER_TAILR_M;

    /**************************************************************************
     * 1.7) Load the prescaler match value (if any) in the GPTM Timer n
     *      Prescale Match (GPTMTnPMR) register.
     *
     * WTIMER1_TAPMR_R
     *
     * GPTM TimerA Prescale Match (GPTMTAPMR, page 759)
     *************************************************************************/

    /**************************************************************************
     * 1.8) Load the event count into the GPTM Timer n Match (GPTMTnMATCHR)
     *      register. Note that when executing an up-count, the value of
     *      GPTMTnPR and GPTMTnILR must be greater than the value of GPTMTnPMR
     *      and GPTMTnMATCHR.
     *
     *  GPTM Timer A Match (GPTMTAMATCHR, page 755)
     *************************************************************************/
    WTIMER1_TAMATCHR_R |= TIMER_TAMATCHR_TAMR_S;

    /**************************************************************************
     * 1.9) If interrupts are required, set the CnMIM bit in the GPTM Interrupt
     *      Mask (GPTMIMR) register. Generates an interrupt only when were
     *      counted from 0xFFFF.FFFF down to 0x0000.0000 in order to set all
     *      needed flags to keep the timer (edge-count) counting.
     *
     *  GPTM Interrupt Mask (GPTMIMR, page 742)
     *************************************************************************/
    WTIMER1_IMR_R |= TIMER_IMR_CAMIM;

	/**************************************************************************
	 * 1.10) Set the TnEN bit in the GPTMCTL register to enable the timer and
	 *      start counting.
	 *
	 * GPTM Control (GPTMCTL, page 734)
	 *************************************************************************/
    WTIMER1_CTL_R |= TIMER_CTL_TAEN; /* GPTM Timer A Enable */

    /**************************************************************************
     * 1.12) Enable Master Interrupt for 16/32-Bit Timer 3A.
     *        WT1CCP0 = 112 -> bit 00 in EN3
     *
     *   Interrupt 96-127 Set Enable (EN3, page 140)
     *************************************************************************/
    NVIC_EN3_R = 0x00000001;

}

void pso_pwm_config()
{
	/**************************************************************************
	 * PWM is implemented to drive the brushless motor through the Electronic
	 * Speed Controller (ESC). It's send to the ESC... WTimer-1B
	 *
	 * Period     = 20 ms (50 Hz)
	 * Duty Cycle =  1 ms (minimum) to 2 ms (maximum) linearly displaced.
	 *
	 *************************************************************************/

	/*************************************************************************
     * 0.1) Port configuration for the proper use as PWM (WT1CCP1-PC7).
     *
     *      It's choose the alternate function for the PC6.
     *
     *      Refer to Table 23-5 (page 1344) to check the appropriate encoding
     *      for the bit fields in the register GPIOPCTL.
     *
     * GPIO Alternate Function Select (GPIOAFSEL, page 668)
     * GPIO Port Control (GPIOPCTL, page 685)
     *************************************************************************/
    GPIO_PORTC_LOCK_R  = GPIO_LOCK_KEY;      /* Unlocks the GPIO_CR register */
    GPIO_PORTC_CR_R   |= GPIO_PIN_7;         /* Allow changes to PC6         */
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGC2_GPIOC; /* GPIO Port C Run Mode Clock   */
    										 /* Gating Control               */
    GPIO_PORTC_DIR_R  |=  GPIO_PIN_7;        /* 1: Output                    */
    GPIO_PORTC_AFSEL_R |= GPIO_PIN_7;        /* Alternate function for PC7   */
//	    GPIO_PORTC_PUR_R  |=  GPIO_PIN_7;    /* Enable pull up resistors for PC7 */
    GPIO_PORTC_DEN_R  |= GPIO_PIN_7;              /* Enable digital pins PC7 */
    GPIO_PORTC_AMSEL_R &= ~(GPIO_PIN_7);          /* Disable analog function */
    GPIO_PORTC_PCTL_R  |= GPIO_PCTL_PC7_WT1CCP1;       /* Port Mux Control 7 */
    GPIO_PORTC_LOCK_R = GPIO_LOCK_UNLOCKED; /* The GPIOCR register is locked */

    /**************************************************************************
	 * 1.0) Enable the Wide Timer 0 using the  RCGCWTIMER register.
	 *
	 * 32/64-Bit Wide General-Purpose Timer Run Mode Clock Gating Control
	 * (RCGCWTIMER, page 355)
	 *************************************************************************/
    SYSCTL_RCGCWTIMER_R |= SYSCTL_RCGCWTIMER_R1;
	/**************************************************************************
	 * 1.1) Ensure the timer is disabled (the TAEN bit in the GPTMCTL register
	 *      is cleared) before making any changes.
	 *
	 * GPTM Control (GPTMCTL, page 734)
	 *************************************************************************/
    WTIMER1_CTL_R &= ~(TIMER_CTL_TBEN);  /* GPTM Timer B Disable */
	/**************************************************************************
	 * 1.2)  Write the GPTM Configuration (GPTMCFG) register with a value of
	 *       0x0000.0004. For a 32/64-bit wide timer, this value selects the
	 *       32-bit timer configuration.
	 *
	 * GPTM Configuration (GPTMCFG, page 724)
	 *************************************************************************/
    WTIMER1_CFG_R |= TIMER_CFG_16_BIT;  // |= TIMER_CFG_16_BIT;
	/**************************************************************************
	 * 1.3) In the GPTM Timer Mode (GPTMTBMR) register, set the TnAMS bit to
	 *      0x1, the TnCMR bit to 0x0, and the TnMR field to 0x2.
	 *
	 * TBAMS = 0x1 : PWM mode is enabled.
	 * TBCMR = 0x0 : Edge-Count mode
	 * TBMR  = 0x2 : Periodic Timer mode
	 *
	 *  GPTM Timer B Mode (GPTMTAMR, page 730)
	 *************************************************************************/
    WTIMER1_TBMR_R |= (TIMER_TBMR_TBAMS | TIMER_TBMR_TBMR_PERIOD);
    /**************************************************************************
     * 1.4) Configure the output state of the PWM signal (whether or not it is
     *      inverted) in the TnPWML field of the GPTM Control (GPTMCTL)
     *      register.
     *
     * TBPWML = 0x1 : Output is inverted.
     *
     * GPTM Control (GPTMCTL, page 734)
     *************************************************************************/
    WTIMER1_CTL_R |= TIMER_CTL_TBPWML;

    /**************************************************************************
     * 1.5) Not intended to use the GPTM Timer B Prescale
     *
     *    GPTM Timer B Prescale (GPTMTBPR, page 758)
     *************************************************************************/

    /**************************************************************************
     * 1.6) If PWM interrupts are used, configure the interrupt condition in
     *      the TnEVENT field in the GPTMCTL register and enable the interrupts
     *      by setting the TnPWMIE bit in the GPTMTnMR register. Note that edge
     *      detect interrupt behavior is reversed when the PWM output is
     *      inverted (see page 734).
     *
     *  TBEVENT = 0x0 : Positive edge
     *
     * GPTM Control (GPTMCTL, page 734)
     *************************************************************************/
    //WTIMER1_CTL_R |= ???

	/**************************************************************************
    * 1.7)  Load the timer start value into the GPTM Timer n Interval Load
    *       (GPTMTnILR) register.
    *
    *       Considering the system clock frequency of 40 MHz (T = 25 ns), to
    *       generate a period of 20 ms (50 Hz), the start value that should
    *       be counted down is
    *
    *           Count = 20ms / 25ns
    *                 = 800000
    *                 = 0x000C.3500
    *
    *  GPTM Timer B Interval Load (GPTMTBILR, page 754)
    *************************************************************************/
    WTIMER1_TBILR_R = 0x000C3500;

    /**************************************************************************
    * 1.8)  Load the GPTM Timer n Match (GPTMTnMATCHR) register with the match
    *       value.
    *
    * Minimum -> 1.0 ms : 1.0ms / 25ns = 40000 = 0x0000.9C40
    * Mid     -> 1.5 ms : 1.5ms / 25ns = 60000 = 0x0000.EA60
    * Maximum -> 2.0 ms : 2.0ms / 25ns = 80000 = 0x0001.3880
    *
    *  GPTM Timer B Match (GPTMTBMATCHR, 756)
    *************************************************************************/
    WTIMER1_TBMATCHR_R = 0x00009C40;        /* Starts in minimum for safety */

    /**************************************************************************
    * 1.9) Set the TnEN bit in the GPTM Control (GPTMCTL) register to enable
    *      the timer and begin generation of the output PWM signal.
    *
    *  GPTM Control (GPTMCTL, page 734)
    *************************************************************************/
    WTIMER1_CTL_R |= TIMER_CTL_TBEN;                /* GPTM Timer B Enable */





    /**************************************************************************
     * 1.9) If interrupts are required, set the CnMIM bit in the GPTM Interrupt
     *      Mask (GPTMIMR) register. Generates an interrupt only when were
     *      counted from 0xFFFF.FFFF down to 0x0000.0000 in order to set all
     *      needed flags to keep the timer (edge-count) counting.
     *
     *  GPTM Interrupt Mask (GPTMIMR, page 742)
     *************************************************************************/
//    WTIMER1_IMR_R |= TIMER_IMR_CAMIM;
	/**************************************************************************
	 * 1.10) Set the TnEN bit in the GPTMCTL register to enable the timer and
	 *      start counting.
	 *
	 * GPTM Control (GPTMCTL, page 734)
	 *************************************************************************/
//    WTIMER1_CTL_R |= TIMER_CTL_TAEN; /* GPTM Timer A Enable */
    /**************************************************************************
    * 1.12) Enable Master Interrupt for 16/32-Bit Timer 3A.
    *        WT1CCP0 = 112 -> bit 00 in EN3
    *
    *   Interrupt 96-127 Set Enable (EN3, page 140)
    *************************************************************************/
//    NVIC_EN3_R = 0x00000001;

}
