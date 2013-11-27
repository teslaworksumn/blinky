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

extern StateletStack *kStateletStack;

Statelet Base = { BaseRun, BaseHandleEvent };

void BaseRun() {
	Board_UARTPutSTR("Base loaded.");
}

void BaseHandleEvent(Event *e) {
	StateletStackTopple(kStateletStack, EventCodeLock, Locked, e);
	StateletStackTopple(kStateletStack, EventCodeUnlock, Unlocked, e);
}
