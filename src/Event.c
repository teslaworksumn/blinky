//
//  Event.c
//  blinky
//
//  Created by Taylor Trimble.
//  Copyright (c) 2013 Taylor Trimble. All rights reserved.
//

#include "Event.h"
#include <stddef.h>

void InitEventStack() {
    _EventStack.size = 0;
}

Event *PushEvent() {
	Event *e = &_EventStack.events[_EventStack.size];
    _EventStack.size += 1;
    return e;
}

Event *PeekEvent() {
    return &_EventStack.events[_EventStack.size - 1];
}

Event *PopEvent() {
	if (_EventStack.size <= 0) {
		return NULL;
	}

    _EventStack.size -= 1;
    return &_EventStack.events[_EventStack.size];
}
