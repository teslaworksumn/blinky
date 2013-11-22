//
//  WeakQueue.h
//  blinky
//
//  Created by Taylor Trimble.
//  Copyright (c) 2013 Taylor Trimble. All rights reserved.
//

#ifndef WEAKQUEUE_H_
#define WEAKQUEUE_H_

typedef struct _WeakQueue{
    void *backingStore;
    int start;
    int end;
    int typeSize;
    int capacity;
} WeakQueue;

void WeakQueueInit(WeakQueue *me, void *backingStore, int typeSize, int capacity);

void *WeakQueueReserve(WeakQueue *me);
void WeakQueueCommit(WeakQueue *me);

void *WeakQueueSnag(WeakQueue *me);
void WeakQueueDecommit(WeakQueue *me);

#endif
