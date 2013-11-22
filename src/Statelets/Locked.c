//
//  Locked.c
//  blinky
//
//  Created by Taylor Trimble.
//  Copyright (c) 2013 Taylor Trimble. All rights reserved.
//

#include "Statelets.h"
#include "board.h"
#include <stdio.h>

Statelet Locked = { LockedRun, LockedHandleEvent };

void LockedRun()
{
	Board_UARTPutSTR("Locked loaded.");
}

void LockedHandleEvent(Event *e)
{
}
