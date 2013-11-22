//
//  Base.h
//  blinky
//
//  Created by Taylor Trimble.
//  Copyright (c) 2013 Taylor Trimble. All rights reserved.
//

#ifndef BASE_H_
#define BASE_H_

#include "../Event.h"
#include "../Statelet.h"

extern Statelet Base;

void BaseRun();
void BaseHandleEvent(Event *e);

#endif
