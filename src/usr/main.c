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



int main(void)
{
	uint8_t pos = 0U;
	uint16_t aux1 = 0;
	uint16_t aux2 = 0;
	uint8_t start_test = 0U;
	uint16_t ramp = 0U;
	uint16_t i;
	UINT bw;

	/* 16MHz -> PLL -> 400MHz -> (1/2 * 1/5 = 1/10) = 40MHz  */
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

	PSO_PeripheralEnable();
	PSO_GPIOConfig();
	PSO_UART0Config();
	PSO_Timers();
	PSO_ADCConfig();
	pso_rpm_config();
	pso_pwm_config();
	pso_spi0_config();

	IntMasterEnable();

//    GPIO_PORTA_DATA_R &= ~(GPIO_PIN_3);  /* CS on PA3 */ /* MMC CS = L */
//    GPIO_PORTA_DATA_R |= (GPIO_PIN_3);   /* CS on PA3 */ /* MMC CS = H */
//    GPIO_PORTA_DATA_R &= ~(GPIO_PIN_3);  /* CS on PA3 */ /* MMC CS = L */


	/* Give a work area to the default drive */
//	f_mount(&FatFs, "", 0);

	/* Create a file */
//	if (f_open(&Fil, "newfile.txt", FA_WRITE | FA_CREATE_ALWAYS) == FR_OK)
//	{
		/* Write data to the file */
//		f_write(&Fil, "It works!\r\n", 11, &bw);

		/* Close the file */
//        f_close(&Fil);
//	}


#if (FAT_FS09B)

	for (i = 0; i < 2014; i++)
	{
		buffer[i] = i;
	}
		sprintf(filename,"flight.txt"); 	// (char *)filename

		puts("FsFAT Testing");

		memset(&fs, 0, sizeof(FATFS));

		// Register work area for each volume (Always succeeds regardless of disk status)
	    res = f_mount(0, &fs[0]);

		if (res != FR_OK)
	        printf("res = %d f_mount\n", res);

	    // Create destination file on the drive 0
	    res = f_open(&ftxt, filename, FA_CREATE_ALWAYS | FA_WRITE);
//	    if (res) sd_error(res);

        res = f_write(&ftxt, &buffer, sizeof(buffer), &bw);
	    //res = f_write(&ftxt, Gumstix_Message_out, 50, &bw); sizeof(vector)
//	    if (res) sd_error(res);

		// Close open file
	    f_close(&ftxt);

	    // Unregister work area prior to discard it
		f_mount(0, NULL);
#endif

/* ff11a */
#if (FAT_FS11A)
		f_mount(&FatFs, "", 0);		/* Give a work area to the default drive */

		if (f_open(&Fil, "newfile.txt", FA_WRITE | FA_CREATE_ALWAYS) == FR_OK) {	/* Create a file */

			f_write(&Fil, "It works!\r\n", 11, &bw);	/* Write data to the file */

			f_close(&Fil);								/* Close the file */

			if (bw == 11) {		/* Lights green LED if data written well */
				GPIO_PORTF_DATA_R ^= GPIO_PIN_2;	/* Toggle LED on PF2 */
			}
		}
#endif

	while(1)
    {

		if (g_timer_a0_scan_flag)
		{

			g_timer_a0_scan_flag = 0U;


#if (PWM_DRIVE_MODE)    /* MANUAL */

			start_test = 1U;
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

#else                   /* AUTO */

			/* Switch #1 => Start test */
			if ( !(GPIO_PORTF_DATA_R & GPIO_PIN_4) )
			{
				start_test = 1U;
			}

			/* Switch #2 => Stop test immediately */
			if ( !(GPIO_PORTF_DATA_R & GPIO_PIN_0) )
			{
				set_pwm_position(0U);
        		start_test = 0U;
        		ramp = 0U;
			}

#endif


            if (start_test)
            {
            	/* f = 200 Hz => 1 second has 200 cycles */
//            	if (ramp < 600U) /* Wait 3s before the ramp up */
//           	{
//           		set_pwm_position(0U);
//            		ramp++;
//            	}
//
//           	/* From 0 to 100% throttle within 5s */
//            	if ((ramp >= 600U) && (ramp < 1600U))
//           	{
//            		if (!(ramp % 10U))
//            			set_pwm_position(pos++);
//
//            		ramp++;
//            	}

            	/* Remains with full power (100%) for 10s */
//            	if ((ramp >= 1600U) && (ramp < 3600U))
//            	{
//            		set_pwm_position(100U);
//            		ramp++;
//            	}

            	/* From 100 to 0% throttle within 5s */
//            	if ((ramp >= 3600U) && (ramp < 4600U))
//            	{
//            		if (!(ramp % 10U))
//            			set_pwm_position(pos--);
//
//            		ramp++;
//            	}

//            	if (ramp >= 4600U) /* Stops the motor completely */
//            	{
//            		set_pwm_position(0U);
//            		ramp++;
//            	}

//            	if (ramp >= 5000U) /* Stops the motor completely */
//            	{
//            		start_test = 0U;
//            		ramp = 0U;
//
//            	}

            	SSI0_DR_R = aux1;


            	GPIO_PORTF_DATA_R ^= GPIO_PIN_1;    /* Red LED on PF1 */

				read_raw_data (&g_uart0_data);
				copy_raw_data (&g_tx_buffer_uart, &g_uart0_data);
				uart_write( );
				packet_data (&dp);
				uart_write2(uart_tx_buffer, &dp);
				uartBatchWrite (UART0_BASE, uart_tx_buffer, 20);

            	aux2 = SSI0_DR_R;
            	aux1++;
            }
		} /* END 'if' */
	} /* END 'while' */

}

