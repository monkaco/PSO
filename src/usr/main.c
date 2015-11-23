#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"	/* Interrupt and register assignments on the Tiva C LauchPad board */
#include "hw_memmap.h"      /* Macros defining the memory map of the device. */
#include "hw_types.h"       /* Macros for hardware access, direct and via the bit-band region. */
#include "sysctl.h"         /* Prototypes for the system control driver. */
#include "gpio.h"           /* Defines and Macros for GPIO API */
#include "debug.h"          /* Macros for assisting debug of the driver library. */
#include "pwm.h"            /* API function protoypes for Pulse Width Modulation (PWM) ports */
#include "pin_map.h"        /* Mapping of peripherals to pins for all parts. */
#include "hw_gpio.h"        /* Defines and Macros for GPIO hardware. */
#include "rom.h"            /* Macros to facilitate calling functions in the ROM. */
#include "timer.h"          /* Prototypes for the timer module */
#include "uart.h"           /* Defines and Macros for the UART. */
#include "interrupt.h"      /* Prototypes for the NVIC Interrupt Controller Driver */
#include "pso_init.h"
#include "pso_uart.h"
#include "pso_led.h"
#include "pso_data.h"
#include "ulink.h"
#include "ulink_types.h"
#include "adc.h"
#include "pso_pwm.h"

#define PART_TM4C123GH6PM    /* Used for Port/Pin Mapping Definitions defined in "pin_map.h" */
#define PWM_FREQUENCY  50

extern uart_raw_data_t g_uart0_data;
extern uint8_t g_tx_buffer_uart;
extern uint16_t uart_tx_buffer[ULINK_MAX_PACKET_LEN];
extern uint8_t g_timer_a0_scan_flag;
extern ulink_pso_data_t dp;


int main(void)
{
	uint8_t pos = 0U;
	uint32_t aux;

	/* 16MHz -> PLL -> 400MHz -> (1/2 * 1/5 = 1/10) = 40MHz  */
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

	PSO_PeripheralEnable();
	PSO_GPIOConfig();
	PSO_UART0Config();
	PSO_Timers();
	PSO_ADCConfig();
	pso_rpm_config();
	pso_pwm_config();

	IntMasterEnable();


	while(1)
    {

		if (g_timer_a0_scan_flag)
		{

			g_timer_a0_scan_flag = 0U;



			/* Switch #1 */
			if ( !(GPIO_PORTF_DATA_R & GPIO_PIN_4) )
			{
				set_pwm_position(pos++);

				if ( (pos > 100U) | (pos < 0U) )
					pos = 100U;
			}

			/* Switch #2 */
			if ( !(GPIO_PORTF_DATA_R & GPIO_PIN_0) )
			{
				set_pwm_position(pos--);

				if ( (pos > 100U) | (pos < 1U) )
				    pos = 0U;
			}



			if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3))
			{
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
			}
			else
			{
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
			}

			read_raw_data (&g_uart0_data);
			copy_raw_data (&g_tx_buffer_uart, &g_uart0_data);
			uart_write( );
			packet_data (&dp);
			uart_write2(uart_tx_buffer, &dp);
			uartBatchWrite (UART0_BASE, uart_tx_buffer, 20);
		} /* END 'if' */
	} /* END 'while' */

}

