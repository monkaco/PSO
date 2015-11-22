/******************************************************************************
* FILENAME:    led.c
*
* DESCRIPTION:
*       Functions to turn on/off the three LEDs.
*
* FUNCTIONS:
*    void LEDRedOn    (void);
*    void LEDRedOff   (void);
*    void LEDGreenOn  (void);
*    void LEDGreenOff (void);
*    void LEDBlueOn   (void);
*    void LEDBlueOff  (void);
*    void LEDWhiteOn  (void);
*    void LEDWhiteOff (void);
*
* NOTES:
*       None.
*
* REFERENCES:
*       None.
*
* START DATE:    16 Aug 2015
*
* CHANGES :
*
* VERSION DATE        WHO                    DETAIL
* 1.0     16 Aug 2015 Rogerio Lima         Start-up coding.
*
* -----------------------------------------------------------------------------
* 2.0
******************************************************************************/


/*****************************************************************************
 * GENERAL INFORMATION
 *     PF0: SW2
 *     PF1: Red LED   (0x02)
 *     PF2: Blue LED  (0x04)
 *     PF3: Green LED (0x08)
 *     PF4: SW1
 *
 *****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"
#include "hw_memmap.h"      /* Macros defining the memory map of the device. */
#include "tm4c123gh6pm.h"	/* Interrupt and register assignments on the Tiva C LauchPad board */
#include "pso_pwm.h"



/* Turn off buzzer */

/*******************************************************************************
* Function Name  : set_pwm_position
* Input          : uint8_t pos
* Output         : None
* Return         : 1 if success, 0 if fail
* Description    : Defines the position (servo) or throttle (ESC) in a percen-
*                  tual scale from 0 to 100.
*******************************************************************************/
uint8_t set_pwm_position (uint8_t pos)
{
	uint32_t duty_cycle;
	uint8_t returnval;
    /**************************************************************************
    * Minimum -> 1.0 ms : 1.0ms / 25ns = 40000 = 0x0000.9C40
    * Mid     -> 1.5 ms : 1.5ms / 25ns = 60000 = 0x0000.EA60
    * Maximum -> 2.0 ms : 2.0ms / 25ns = 80000 = 0x0001.3880
    **************************************************************************/

	if( pos >= 0U & pos <= 100U)
	{
	    duty_cycle = 400*pos + 40000;
	    WTIMER1_TBMATCHR_R = duty_cycle;
	    GPIO_PORTF_DATA_R ^= GPIO_PIN_2;

	    returnval = 1U;
    }
	else
	{
        returnval = 0U;
	}

    return returnval;
}


