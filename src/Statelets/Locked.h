//
//  Locked.h
//  blinky
//
//  Created by Taylor Trimble.
//  Copyright (c) 2013 Taylor Trimble. All rights reserved.
//

#ifndef LOCKED_H_
#define LOCKED_H_

#include "../Event.h"
#include "../Statelet.h"

extern Statelet Locked;

void LockedRun();
void LockedHandleEvent(Event *e);

#endif
