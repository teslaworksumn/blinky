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
// Globals
// =============================================================================

// =============================================================================
// Threads
// =============================================================================

// =============================================================================
// main
// =============================================================================

static int protothread1_flag, protothread2_flag;

static int protothread1(struct pt *pt)
{
    PT_BEGIN(pt);

    while (1) {
        PT_WAIT_UNTIL(pt, protothread2_flag != 0);
        Board_LED_Set(0, true);
        protothread2_flag = 0;
        protothread1_flag = 1;
    }

    PT_END(pt);
}

static int protothread2(struct pt *pt)
{
    PT_BEGIN(pt);

    while(1) {
        protothread2_flag = 1;
        PT_WAIT_UNTIL(pt, protothread1_flag != 0);
        Board_LED_Set(1, true);
        protothread1_flag = 0;
    }

    PT_END(pt);
}

static struct pt pt1, pt2;

int main(void)
{
	Board_Init();
    PT_INIT(&pt1);
    PT_INIT(&pt2);

    while(1) {
        protothread1(&pt1);
        protothread2(&pt2);
    }
}
