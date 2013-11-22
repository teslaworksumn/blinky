//
//  Base.c
//  blinky
//
//  Created by Taylor Trimble.
//  Copyright (c) 2013 Taylor Trimble. All rights reserved.
//

#include "Statelets.h"
#include "board.h"
#include <stdio.h>

Statelet Base = { BaseRun, BaseHandleEvent };

void BaseRun() {
	Board_UARTPutSTR("Base loaded.");
}

void BaseHandleEvent(Event *e) {
	Topple(EventCodeLock, Locked, e);
	Topple(EventCodeUnlock, Unlocked, e);
}
