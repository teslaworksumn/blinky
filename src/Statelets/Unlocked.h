//
//  Unlocked.h
//  blinky
//
//  Created by Taylor Trimble.
//  Copyright (c) 2013 Taylor Trimble. All rights reserved.
//

#ifndef UNLOCKED_H_
#define UNLOCKED_H_

#include "../Event.h"
#include "../Statelet.h"

extern Statelet Unlocked;

void UnlockedRun();
void UnlockedHandleEvent(Event *e);

#endif
