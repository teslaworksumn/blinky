//
//  Statelet.h
//  blinky
//
//  Created by Taylor Trimble.
//  Copyright (c) 2013 Taylor Trimble. All rights reserved.
//

#ifndef STATELET_H_
#define STATELET_H_

#define STATELET_STACK_SIZE 32

typedef struct _Statelet {
	void (*run)();
	void (*handleEvent)(Event *);
} Statelet;

struct {
    int size;
    Statelet statelets[STATELET_STACK_SIZE];
    int eventDepth;
} _StateletStack;

void InitStateletStack(Statelet base);
void PushStatelet(Statelet s);
void TryStatelet(Event *e);
void Topple(EventCode code, Statelet s, Event *e);

#endif
