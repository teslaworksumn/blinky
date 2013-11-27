//
//  WeakStack.h
//  blinky
//
//  Created by Taylor Trimble.
//  Copyright (c) 2013 Taylor Trimble. All rights reserved.
//

#ifndef WEAKSTACK_H_
#define WEAKSTACK_H_

typedef struct _WeakStack {
    void *backingStore;
    int len;
    int typeSize;
    int capacity;
} WeakStack;

void WeakStackInit(WeakStack *me, void *backingStore, int typeSize, int capacity);

void *WeakStackReserve(WeakStack *me);
void WeakStackCommit(WeakStack *me);

void *WeakStackAccess(WeakStack *me);
void *WeakStackAccessIndex(WeakStack *me, int i);
void WeakStackDecommit(WeakStack *me);
void WeakStackDecommitN(WeakStack *me, int n);

#endif
