//
//  Statelet.c
//  blinky
//
//  Created by Taylor Trimble.
//  Copyright (c) 2013 Taylor Trimble. All rights reserved.
//

#include "Event.h"
#include "Statelet.h"
#include "WeakStack.h"
#include <stddef.h>

Statelet *StateletStackInit(StateletStack *me) {
	me->stack = &me->_stack;
	WeakStackInit(me->stack, me->statelets, sizeof(Statelet), STATELET_STACK_SIZE);
	me->eventDepth = 0;

	Statelet *s = WeakStackReserve(me->stack);
	WeakStackCommit(me->stack);
	return s;
}

void StateletStackHandle(StateletStack *me, Event *e) {
	int stackHeight = me->stack->len;
	for (me->eventDepth = 0; me->eventDepth < stackHeight; me->eventDepth += 1) {
		Statelet *s = (Statelet *)WeakStackAccessIndex(me->stack, me->eventDepth);
		s->handleEvent(e);
	}
	if (me->eventDepth == stackHeight) {
		Statelet *s = (Statelet *)WeakStackAccess(me->stack);
		s->run();
	}
}

void StateletStackTopple(StateletStack *me, EventCode code, Statelet s, Event *e) {
	if (e->code == code) {
		WeakStackDecommitN(me->stack, me->eventDepth);
		Statelet *ns = (Statelet *)WeakStackReserve(me->stack);
		ns->run = s.run;
		ns->handleEvent = s.handleEvent;
		WeakStackCommit(me->stack);
	}
}
