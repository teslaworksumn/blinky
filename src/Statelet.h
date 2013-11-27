//
//  Statelet.h
//  blinky
//
//  Created by Taylor Trimble.
//  Copyright (c) 2013 Taylor Trimble. All rights reserved.
//

#ifndef STATELET_H_
#define STATELET_H_

#include "WeakStack.h"

#define STATELET_STACK_SIZE 32

typedef struct _Statelet {
	void (*run)();
	void (*handleEvent)(Event *);
} Statelet;

typedef struct _StateletStack {
    Statelet statelets[STATELET_STACK_SIZE];
    WeakStack _stack;
    WeakStack *stack;
    int eventDepth;
} StateletStack;

Statelet *StateletStackInit(StateletStack *me);
void StateletStackHandle(StateletStack *me, Event *e);
void StateletStackTopple(StateletStack *me, EventCode code, Statelet s, Event *e);

#endif
