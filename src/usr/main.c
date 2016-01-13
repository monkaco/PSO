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
#include "ff.h"		/* Declarations of FatFs API */
#include "fifo.h"

/* ff11a */
#if (FAT_FS11A)
FATFS FatFs;		/* FatFs work area needed for each volume */
FIL Fil;			/* File object needed for each open file */
#endif

/* ff09b */
#if (FAT_FS09B)
FATFS fs[1];         // Work area (file system object) for logical drives
FIL ftxt;            // file objects
BYTE buffer[1024];    // file copy buffer
BYTE buf[21];
FRESULT res;         // FatFs function common result code
WORD bw;             // File write count
CHAR filename[12];
#endif


#define PART_TM4C123GH6PM    /* Used for Port/Pin Mapping Definitions defined in "pin_map.h" */
#define PWM_FREQUENCY  50

extern uart_raw_data_t g_uart0_data;
extern uint8_t g_tx_buffer_uart;
extern uint16_t uart_tx_buffer[ULINK_MAX_PACKET_LEN];
extern uint8_t g_timer_a0_scan_flag;
extern ulink_pso_data_t dp;
extern fifo_t g_fifo_ping;
extern fifo_t g_fifo_pong;
extern uint8_t pwm_throttle;

static uint16_t scan_period_prior = 0U;
uint16_t scan_period_actual;

bool b_aux;



int main(void)
{
	uint8_t pos = 0U;
	uint16_t aux1 = 0U;
	uint16_t aux2 = 0U;
	uint8_t start_test = 0U;
	uint16_t ramp = 0U;
	uint16_t i;
	UINT bw;
	uint8_t sys_state = 0U;
	uint8_t enable_fifo_write = 1U;
	uint8_t state = 0U;
	uint8_t record_sd = 0U;
	uint8_t ping_flag = 0U;
	uint8_t pong_flag = 0U;
	uint8_t ret_func = 0U;

	/* 16MHz -> PLL -> 400MHz -> (1/2 * 1/5 = 1/10) = 40MHz  */
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

	PSO_PeripheralEnable();
	PSO_GPIOConfig();
	PSO_UART0Config();
	PSO_Timers();
	PSO_ADCConfig();
	for (i = 0U; i < 10000; i++);
	pso_rpm_config();
	pso_pwm_config();
	pso_spi0_config();

	IntMasterEnable();

	PSO_LEDWhiteOff();

	while(1)
    {
		// -->
					scan_period_actual = TIMER3_TAR_R;

		if (g_timer_a0_scan_flag)
		{

			g_timer_a0_scan_flag = 0U;

			packet_data (&dp);
			copy_data(uart_tx_buffer, &dp);

			/* Recording data to the SD card */
			if (record_sd && enable_fifo_write)
			{
				/* Full ping fifo transferred to the buffer FatFs */
				if (!ping_flag)
				{
					aux1 = fifo_count_elements(&g_fifo_ping);
					for (i = 0U; i < aux1; i++)
						buffer[i] = (BYTE)fifo_get(&g_fifo_ping);
				}

				/* Full pong fifo transferred to the buffer FatFs */
				if (!pong_flag)
				{
					aux1 = fifo_count_elements(&g_fifo_pong);
					for (i = 0U; i < aux1; i++)
						buffer[i] = (BYTE)fifo_get(&g_fifo_pong);
				}

				/**********************************************************
				 * Whenever a fifo is fully filled (PING or PONG), it's
				 * time to send data through serial port.
				 * This rate depends on the size of the buffer, the ADC
				 * Sample Rate chose via Timer0 which is capable to
				 * triggers ADC sample and the hardware average made
				 * in ADC
				 * Example:
				 *     Timer period interrupt: 1/40k
				 *     Hardware average: 4
				 *     Effective ADC sampling rate: 1/10k
				 *     Time to fully fill the fifo (1024) = 5.12 ms
				 *
				 * Refer to the excel sheet "PSO_Comm_Protocol.xlsx for
				 * details.
				 *********************************************************/
				/* 2.560 ms spent in uartBatchWrite & f_write functions  */
				uartBatchWrite (UART0_BASE, uart_tx_buffer, PACKET_LENGTH);

				//res = f_write(&ftxt, &buffer, sizeof(buffer), &bw);

				record_sd = 0U;
			}

			/* System Finite State Machine (FSM) */
			switch (sys_state)
			{
			/* State 0: Waits for SW1 be pressed */
				case 0:

					/* Switch #1 */
					if (!(GPIO_PORTF_DATA_R & GPIO_PIN_4))
						sys_state = 1U;
					else
						sys_state = 0U;

					sd_stand_by();

					break;

			/* State 1: Starts SD card and register FatFs work area */
				case 1:
#if (FAT_FS09B)
					sprintf(filename,"daq.txt"); 	// (char *)filename

					puts("FsFAT Testing");

					memset(&fs, 0, sizeof(FATFS));

					// Register work area for each volume (Always succeeds regardless of disk status)
					res = f_mount(0, &fs[0]);

					if (res != FR_OK)
						printf("res = %d f_mount\n", res);

					// Create destination file on the drive 0
					res = f_open(&ftxt, filename, FA_CREATE_ALWAYS | FA_WRITE);

					if (res == FR_OK)
					{
						enable_fifo_write = 1U;
						sd_ok ();
						sys_state = 2U;
					}
					else
					{
						sys_state = 5U;
					}


#endif
					break;
			/* State 2: Starts to fill fifos and write data to the memory*/
				case 2:

					/* Turn on */
					GPIO_PORTF_DATA_R |= GPIO_PIN_2; /* Blue LED on PF2 */

					res = f_write(&ftxt, &uart_tx_buffer, sizeof(uart_tx_buffer), &bw);
					/* Turn off */
					GPIO_PORTF_DATA_R &= ~GPIO_PIN_2;    /* Blue LED on PF2 */

					ret_func = fun_trapezoid ();
					/* Switch #2  - Finish the recording */
					if ( !(GPIO_PORTF_DATA_R & GPIO_PIN_0) || (ret_func) )
						sys_state = 3U; /* SW2 pressed: stops recording */
					else
						sys_state = 2U;  /* SW2 non pressed */

					break;
				case 3:
					// Close open file
					f_close(&ftxt);
					// Unregister work area prior to discard it
					f_mount(0, NULL);

					enable_fifo_write = 0U; /* Disables write to the fifo */

					sys_state = 4U;
					break;
				case 4:
					for (i = 0U; i < 10U; i++)
						sd_finish_record();     /* Turn on white LED */

					sys_state = 0U;
					break;

				case 5: /* SD error state */
					for (i = 0U; i < 5U; i++)
						sd_error ();

					sys_state = 0U;
					break;
				default:
					/* code */
					break;

			}

			/* Ping-Pong FIFO FSM */
			switch (state)
			{
			/* State 0: Initializes the variables */
				case 0:
					ping_flag = 1U; /* Enables ping fifo to be filled */
					pong_flag = 0U; /* Disables pong fifo             */
					record_sd = 0U; /* It's not time to record data to SD*/
					state =  1U;    /* Fills the ping fifo */
			/* State 1: Starts to fill the PING fifo */
				case 1:
					for (i = 0U; i < PACKET_LENGTH; i++)
					{
						b_aux = fifo_put (&g_fifo_ping, (uint8_t)uart_tx_buffer[i]);
						if (b_aux == false)
						{
							fifo_put (&g_fifo_pong, (uint8_t)uart_tx_buffer[i]);
							ping_flag = 0U; /* Enables ping fifo to be filled */
							pong_flag = 1U; /* Disables pong fifo             */
							record_sd = 1U;
							state = 2U;
						}
					}
					break;
			/* State 2: Starts to fill the PONG fifo */
				case 2:
					for (i = 0U; i < PACKET_LENGTH; i++)
					{
						b_aux = fifo_put (&g_fifo_pong, (uint8_t)uart_tx_buffer[i]);
						if (b_aux == false)
						{
							fifo_put (&g_fifo_ping, (uint8_t)uart_tx_buffer[i]);
							ping_flag = 1U; /* Enables ping fifo to be filled */
							pong_flag = 0U; /* Disables pong fifo             */
							record_sd = 1U;
							state = 1U;
						}
					}
					break;
				default:
					/* code */
					break;

			}

// -->
			dp.index = TIMER3_TAR_R - scan_period_actual;


			/* Switch #1 & Switch #2 pressed together*/
//				if (!(GPIO_PORTF_DATA_R & GPIO_PIN_4) & !(GPIO_PORTF_DATA_R & GPIO_PIN_0))
//				{
//					 sys_state = 0U;
//				}

		} /* END 'if' */
	} /* END 'while' */

}

