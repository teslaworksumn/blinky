//
//  Unlocked.c
//  blinky
//
//  Created by Taylor Trimble.
//  Copyright (c) 2013 Taylor Trimble. All rights reserved.
//

#include "Statelets.h"
#include "board.h"
#include <stdio.h>

Statelet Unlocked = { UnlockedRun, UnlockedHandleEvent };

void UnlockedRun()
{
	Board_UARTPutSTR("Unlocked loaded.");
}

void UnlockedHandleEvent(Event *e)
{
}
