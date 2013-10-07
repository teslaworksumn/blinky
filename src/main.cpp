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
// main
// =============================================================================

int main(void)
{
    Board_Init();

    Board_LED_Set(0, true);
    Board_LED_Set(1, false);

    while (1) {
        __WFI();
    }
}
