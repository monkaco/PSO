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
#include "mavlink_bridge.h"

#define PART_TM4C123GH6PM    /* Used for Port/Pin Mapping Definitions defined in "pin_map.h" */
#define PWM_FREQUENCY  50

extern uart_raw_data_t g_uart0_data;
extern uint8_t g_tx_buffer_uart;
extern uint16_t uart_tx_buffer[MAVLINK_MAX_PACKET_LEN];


int main(void)
{
	uint8_t token = 0U;

	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

	PSO_PeripheralEnable();
	PSO_GPIOConfig();
	PSO_UART0Config();


	IntMasterEnable();

    /* Timer initialization */
	PSO_Timers();

	while(1){


		if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00)
		{

			switch (token)
			{
				case 0:
					PSO_LEDRedOn();
				break;

				case 1:
					PSO_LEDGreenOn();
				break;

				case 2:
					PSO_LEDBlueOn();
				break;
				case 3:
					PSO_LEDCyanOn();
				break;

				case 4:
					PSO_LEDPurpleOn();
				break;

				case 5:
					PSO_LEDYellowOn();
				break;
				case 6:
					PSO_LEDWhiteOn();
				break;

				default:
					PSO_LEDAllOff();
			} /* switch */

			SysCtlDelay(100000);
			SysCtlDelay(100000);

			token++;

			if (token > 6)
			{
				token = 0;
			}

		} /* if */

		if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00)
		{
			PSO_LEDAllOff();
		}

		SysCtlDelay(100000);

		read_raw_data (&g_uart0_data);
		copy_raw_data (&g_tx_buffer_uart, &g_uart0_data);
		uart_write();
		uartBatchWrite (UART0_BASE, uart_tx_buffer, 10);
	}




}

