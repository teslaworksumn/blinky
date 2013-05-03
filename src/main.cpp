/*
 * @brief Blinky example using a timer and interrupt
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#ifdef __USE_CMSIS
#include "LPC177x_8x.h"
#endif

#include <cr_section_macros.h>
#include <NXP/crp.h>

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP extern const unsigned int CRP_WORD = CRP_NO_CRP ;

#include "board.h"

#include <stdio.h>
static inline void delay_MS(uint16_t Milliseconds)
{
	while (Milliseconds--)
	{
		volatile uint32_t i;

		for (i = 0; i < (4 * 1000); i++) { /* This logic was tested. It gives app. 1 micro sec delay */
			;
		}
	}
}
/** @defgroup EXAMPLES_PERIPH_17XX40XX_BLINKY LPC17xx/40xx Simple Blinky example
 * @ingroup EXAMPLES_PERIPH_17XX40XX
 * <b>Example description</b><br>
 * The Blinky example is known as the "Hello, World!" of embedded programming. This
 * example will flash an LED on your board at a periodic rate and output the
 * timer information on the UART port.<br>
 *
 * <b>Special connection requirements</b><br>
 * There are no special connection requirements for this example.<br>
 *
 * <b>Build procedures:</b><br>
 * @ref LPCOPEN_17XX40XX_BUILDPROCS_KEIL<br>
 * @ref LPCOPEN_17XX40XX_BUILDPROCS_IAR<br>
 * @ref LPCOPEN_17XX40XX_BUILDPROCS_XPRESSO<br>
 *
 * <b>Supported boards and board setup:</b><br>
 * @ref LPCOPEN_17XX40XX_BOARD_EA1788<br>
 * @ref LPCOPEN_17XX40XX_BOARD_EA4088<br>
 *
 * <b>Submitting LPCOpen issues:</b><br>
 * @ref LPCOPEN_COMMUNITY
 * @{
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

//#define TICKRATE1_HZ 20
//#define TICKRATE2_HZ 10

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

#if defined (__cplusplus)
extern "C" {
#endif
/*
 * @brief   Handle interrupt from 32-bit timer 1
 * @return  Nothing

/void TIMER1_IRQHandler(void)
{
    static bool On = false;

    if (Chip_TIMER_MatchPending(LPC_TIMER1, 1)) {
        Chip_TIMER_ClearMatch(LPC_TIMER1, 1);
        On = (bool) !On;
        Board_LED_Set(0, On);
    }
}



  @brief   Handle interrupt from 32-bit timer 2
  @return  Nothing


void TIMER2_IRQHandler(void)
{
    static bool On = false;

    if (Chip_TIMER_MatchPending(LPC_TIMER2, 1)) {
        Chip_TIMER_ClearMatch(LPC_TIMER2, 1);
        On = (bool) !On;
        Board_LED_Set(1, On);
    }
}*/
#if defined (__cplusplus)
} // extern "C"
#endif

/*
 * @brief   main routine for blinky example
 * @return  Function should not exit.
 */
int main(void)
{
    //uint32_t timerFreq;

    Board_Init();

    DEBUGSTR("Blinky example using timers 1 and 2!\r\n");

    /* Enable timer 1 clock and get clock rate */
    //Chip_TIMER_Init(LPC_TIMER1);
    Board_Buttons_Init();
    Board_LED_Test(26);
    //Board_LED_Init(void);
    uint32_t status = Buttons_GetStatus();
    DEBUGOUT("status %d\n",status);
    //BUTTONS_BUTTON1 = 1;
    	/*
#if defined(CHIP_LPC175X_6X)
    timerFreq = Chip_Clock_GetPeripheralClockRate(SYSCTL_PCLK_TIMER1);
#else
    timerFreq = Chip_Clock_GetPeripheralClockRate();
#endif

    Chip_TIMER_Reset(LPC_TIMER1);
    Chip_TIMER_MatchEnableInt(LPC_TIMER1, 1);
    Chip_TIMER_SetMatch(LPC_TIMER1, 1, (timerFreq / TICKRATE1_HZ));
    Chip_TIMER_ResetOnMatchEnable(LPC_TIMER1, 1);
    Chip_TIMER_Enable(LPC_TIMER1);

     Enable timer interrupt

    //DEBUGOUT("Timer 1 clock     = %d Hz\r\n", timerFreq);
    //DEBUGOUT("Timer 1 tick rate = %d Hz\r\n", TICKRATE1_HZ);

    // Enable timer 2 clock and get clock rate

     */

    while (1) {
       // __WFI();
        int status = Buttons_GetStatus();
        DEBUGOUT("status %d\n",status);
        if(status==BUTTONS_BUTTON1)
        {
        	 Board_LED_Set(1,true);
        	 delay_MS(2000);
        	 Board_LED_Set(1,false);
        }
       }

}
