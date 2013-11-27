//
//  WeakStack.c
//  blinky
//
//  Created by Taylor Trimble.
//  Copyright (c) 2013 Taylor Trimble. All rights reserved.
//

#include "WeakStack.h"
#include <stddef.h>

void WeakStackInit(WeakStack *me, void *backingStore, int typeSize, int capacity)
{
	me->backingStore = backingStore;
	me->len = 0;
	me->typeSize = typeSize;
	me->capacity = capacity;
}

void *WeakStackReserve(WeakStack *me)
{
	if (me->len == me->capacity) {
		return NULL;
	}

	return me->backingStore + (me->len * me->typeSize);
}

void WeakStackCommit(WeakStack *me)
{
	me->len += 1;
}

void *WeakStackAccess(WeakStack *me)
{
	if (me->len == 0) {
		return NULL;
	}

	return me->backingStore + ((me->len - 1) * me->typeSize);
}

void *WeakStackAccessIndex(WeakStack *me, int i)
{
	if (me->len == 0) {
		return NULL;
	}

	return me->backingStore + (i * me->typeSize);
}

void WeakStackDecommit(WeakStack *me)
{
	me->len -= 1;
}

void WeakStackDecommitN(WeakStack *me, int n)
{
	me->len -= n;
}
