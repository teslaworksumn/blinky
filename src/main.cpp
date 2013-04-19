/*
 ===============================================================================
 Name        : main.cpp
 Author      : 
 Version     :
 Copyright   : Copyright (C)
 Description : main definition
 ===============================================================================
 */

#ifdef __USE_CMSIS
#include "LPC177x_8x.h"
#endif

#include <cr_section_macros.h>
#include <NXP/crp.h>

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP extern const unsigned int CRP_WORD = CRP_NO_CRP;

// TODO: insert other include files here
#include "board.h"
#include <stdio.h>

// TODO: insert other definitions and declarations here
#define TICKRATE1_HZ 10
#define TICKRATE2_HZ 7
#define TICKRATE_HZ TICKRATE2_HZ

/**
 * @brief	Handle interrupt from 32-bit timer
 * @return	Nothing
 */
void TIMER1_IRQHandler(void) {
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

int main(void) {

	// TODO: insert code here
	uint32_t timerFreq = 1;

	Board_Init();

	/* Enable timer 1 clock and reset it */
	Chip_TIMER_Init (LPC_TIMER1);
//	Chip_RGU_TriggerReset (RGU_TIMER1_RST);
//	while (Chip_RGU_InReset(RGU_TIMER1_RST)) {
//	}
	Chip_SYSCON_PeriphReset(SYSCON_RESET_TIMER1);

	/* Get timer 1 peripheral clock rate */
//	timerFreq = Chip_Clock_GetRate(CLK_MX_TIMER1);
	Chip_SYSCON_GetPCLKRateFromClk(SYSCON_CLK_TIMER1);
	/* Timer setup for match and interrupt at TICKRATE_HZ */
	Chip_TIMER_Reset(LPC_TIMER1);
	Chip_TIMER_MatchEnableInt(LPC_TIMER1, 1);
	Chip_TIMER_SetMatch(LPC_TIMER1, 1, (timerFreq / TICKRATE_HZ));
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER1, 1);
	Chip_TIMER_Enable(LPC_TIMER1);

	/* Enable timer interrupt */
	NVIC_EnableIRQ(TIMER1_IRQn);
	NVIC_ClearPendingIRQ(TIMER1_IRQn);

	DEBUGSTR("Blinky example using timer 1!\r\n");
	DEBUGOUT("Timer 1 clock     = %d Hz\r\n", timerFreq);
	DEBUGOUT("Timer 1 tick rate = %d Hz\r\n", TICKRATE_HZ);

	while (1) {
		__WFI();
	}

	// force the counter to be placed into memory
	volatile static int i = 0;

	// Enter an infinite loop, just incrementing a counter
	while (1)
		i++;
	return 0;
}
