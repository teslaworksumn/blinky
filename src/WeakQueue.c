//
//  WeakQueue.c
//  blinky
//
//  Created by Taylor Trimble.
//  Copyright (c) 2013 Taylor Trimble. All rights reserved.
//

#include "WeakQueue.h"
#include <stddef.h>

int Cap(int v, int cap) {
	return v % cap;
}

void WeakQueueInit(WeakQueue *me, void *backingStore, int typeSize, int capacity)
{
	me->backingStore = backingStore;
	me->start = 0;
	me->end = 0;
	me->typeSize = typeSize;
	me->capacity = capacity;
}

void *WeakQueueReserve(WeakQueue *me)
{
	if (Cap(me->end + 1, me->capacity) == me->start) {
		return NULL;
	}

	return me->backingStore + (me->end * me->typeSize);
}

void WeakQueueCommit(WeakQueue *me)
{
	me->end = Cap(me->end + 1, me->capacity);
}

void *WeakQueueSnag(WeakQueue *me)
{
	if (me->start == me->end) {
		return NULL;
	}

	return me->backingStore + (me->start * me->typeSize);
}

void WeakQueueDecommit(WeakQueue *me)
{
	me->start = Cap(me->start +1, me->capacity);
}
