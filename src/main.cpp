//
//  main.cpp
//  blinky
//
//  Created by NXP, modified by Taylor Trimble
//  Copyright (c) 2013 Taylor Trimble. All rights reserved.
//

#ifdef __USE_CMSIS
#include "LPC177x_8x.h"
#endif

// Variable to store CRP value in. Will be placed automatically by the linker when "Enable Code Read Protect" selected.
#include <cr_section_macros.h>
#include <NXP/crp.h>
__CRP extern const unsigned int CRP_WORD = CRP_NO_CRP ;

#include "board.h"
#include <stdio.h>

#include "../../ribsy/include/pt.h"

// =============================================================================
// Constants
// =============================================================================

#define TICKRATE1_HZ 1
#define TICKRATE2_HZ 10

// =============================================================================
// Interrupt Request Handlers
// =============================================================================

#if defined (__cplusplus)
extern "C" {
#endif

void TIMER1_IRQHandler(void)
{
    static bool On = false;

    if (Chip_TIMER_MatchPending(LPC_TIMER1, 1)) {
        Chip_TIMER_ClearMatch(LPC_TIMER1, 1);
        On = (bool) !On;
        Board_LED_Set(0, On);
    }
}

void TIMER2_IRQHandler(void)
{
    static bool On = false;

    if (Chip_TIMER_MatchPending(LPC_TIMER2, 1)) {
        Chip_TIMER_ClearMatch(LPC_TIMER2, 1);
        On = (bool) !On;
        Board_LED_Set(1, On);
    }
}

#if defined (__cplusplus)
} // extern "C"
#endif

// =============================================================================
// main
// =============================================================================

int main(void)
{
    uint32_t timerFreq;

    Board_Init();

    DEBUGSTR("Blinky example using timers 1 and 2!\r\n");

    /* Enable timer 1 clock and get clock rate */
    Chip_TIMER_Init(LPC_TIMER1);
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

    // Enable timer interrupt
    NVIC_EnableIRQ(TIMER1_IRQn);
    NVIC_ClearPendingIRQ(TIMER1_IRQn);

    DEBUGOUT("Timer 1 clock     = %d Hz\r\n", timerFreq);
    DEBUGOUT("Timer 1 tick rate = %d Hz\r\n", TICKRATE1_HZ);

    // Enable timer 2 clock and get clock rate
    Chip_TIMER_Init(LPC_TIMER2);
#if defined(CHIP_LPC175X_6X)
    timerFreq = Chip_Clock_GetPeripheralClockRate(SYSCTL_PCLK_TIMER2);
#else
    timerFreq = Chip_Clock_GetPeripheralClockRate();
#endif

    Chip_TIMER_Reset(LPC_TIMER2);
    Chip_TIMER_MatchEnableInt(LPC_TIMER2, 1);
    Chip_TIMER_SetMatch(LPC_TIMER2, 1, (timerFreq / TICKRATE2_HZ));
    Chip_TIMER_ResetOnMatchEnable(LPC_TIMER2, 1);
    Chip_TIMER_Enable(LPC_TIMER2);

    // Enable timer interrupt
    NVIC_EnableIRQ(TIMER2_IRQn);
    NVIC_ClearPendingIRQ(TIMER2_IRQn);

    DEBUGOUT("Timer 2 clock     = %d Hz\r\n", timerFreq);
    DEBUGOUT("Timer 2 tick rate = %d Hz\r\n", TICKRATE2_HZ);

    while (1) {
        __WFI();
    }
}
