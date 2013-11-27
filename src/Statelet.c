//
//  Statelet.c
//  blinky
//
//  Created by Taylor Trimble.
//  Copyright (c) 2013 Taylor Trimble. All rights reserved.
//

#include "Event.h"
#include "Statelet.h"
#include <stddef.h>

void InitStateletStack(Statelet base) {
    _StateletStack.size = 1;
    _StateletStack.statelets[0] = base;
}

void PushStatelet(Statelet s) {
    _StateletStack.statelets[_StateletStack.size] = s;
    _StateletStack.size += 1;
}

void TryStatelet(Event *e) {
	for (_StateletStack.eventDepth = 0; _StateletStack.eventDepth < _StateletStack.size; _StateletStack.eventDepth += 1) {
		_StateletStack.statelets[_StateletStack.eventDepth].handleEvent(e);
	}
	if (_StateletStack.eventDepth == _StateletStack.size) {
		_StateletStack.statelets[_StateletStack.size - 1].run();
	}
}

void Topple(EventCode code, Statelet s, Event *e) {
	if (e->code == code) {
		int i = _StateletStack.size - _StateletStack.eventDepth;
		_StateletStack.statelets[i] = s;
		_StateletStack.size = i + 1;
	}
}
