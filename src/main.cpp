//
//  main.cpp
//  blinky
//
//  Created by NXP, modified by Taylor Trimble
//  Copyright (c) 2013 Taylor Trimble. All rights reserved.
//

#ifdef __USE_CMSIS
#include "LPC177x_8x.h"
#endif

// Variable to store CRP value in. Will be placed automatically by the linker when "Enable Code Read Protect" selected.
#include <cr_section_macros.h>
#include <NXP/crp.h>
__CRP extern const unsigned int CRP_WORD = CRP_NO_CRP ;

extern "C" {
#include "Event.h"
#include "Statelets/Statelets.h"
#include "WeakQueue.h"
}
#include "board.h"
#include <stdio.h>
#include "../../ribsy/include/pt.h"

// =============================================================================
// Globals & Defines
// =============================================================================

#define EVENT_STACK_SIZE 32

bool InterceptingUART = false;

static struct pt ptDriveEvents;
static struct pt ptUnlockDriver;
static struct pt ptLockDriver;
static struct pt ptRibsy;

Event _Events[EVENT_STACK_SIZE];
WeakQueue _EventQueue;
WeakQueue *EventQueue = &_EventQueue;

static EventCode MapCharacterToEventCode(int c);
static void Setup();

// =============================================================================
// Threads
// =============================================================================

static PT_THREAD(DriveEvents(struct pt *pt))
{
	static int c;

    PT_BEGIN(pt);

	while (1) {
		PT_WAIT_UNTIL(pt, (!InterceptingUART && (c = Board_UARTGetChar()) != EOF) && (c != 0xe7) && (c != 0xfe));
//		Board_UARTPutChar(c);
		Event *e = (Event *)WeakQueueReserve(EventQueue);
		e->code = MapCharacterToEventCode(c);
		WeakQueueCommit(EventQueue);
	}

    PT_END(pt);
}

static PT_THREAD(UnlockDriver(struct pt *pt))
{
    PT_BEGIN(pt);

    while (1) {
		PT_WAIT_UNTIL(pt, ((Event *)WeakQueueSnag(EventQueue))->code == EventCodeUnlock);
		Board_UARTPutChar('u');
		WeakQueueDecommit(EventQueue);
    }

    PT_END(pt);
}

static PT_THREAD(LockDriver(struct pt *pt))
{
    PT_BEGIN(pt);

    while (1) {
		PT_WAIT_UNTIL(pt, ((Event *)WeakQueueSnag(EventQueue))->code == EventCodeLock);
		Board_UARTPutChar('l');
		WeakQueueDecommit(EventQueue);
    }

    PT_END(pt);
}

static PT_THREAD(Ribsy(struct pt *pt))
{
	static Event *e;

    PT_BEGIN(pt);

    while (1) {
    	PT_WAIT_UNTIL(pt, (e = (Event *)WeakQueueSnag(EventQueue)) != NULL);
    	TryStatelet(e);
    	WeakQueueDecommit(EventQueue);
    }

    PT_END(pt);
}

// =============================================================================
// main
// =============================================================================

static EventCode MapCharacterToEventCode(int c) {
	EventCode ret = EventCodeNone;

	switch (c) {
	case 0x92:
	case 0xb2:
		ret = EventCodeUnlock;
		break;
	case 0x70:
		ret = EventCodeLock;
		break;
	}

	return ret;
}

static void Setup() {
	// General board initialization
	Board_Init();

	// Serial debug
	Board_Debug_Init();

	// Stacks
	WeakQueueInit(EventQueue, _Events, sizeof(Event), EVENT_STACK_SIZE);
	InitStateletStack(Base);

	// Initialize protothreads
    PT_INIT(&ptDriveEvents);
    PT_INIT(&ptUnlockDriver);
    PT_INIT(&ptLockDriver);
    PT_INIT(&ptRibsy);
}

int main(void)
{
	Setup();

	Board_UARTPutSTR("Ready; let's do this.\r\n");

    while (1) {
        PT_SCHEDULE(DriveEvents(&ptDriveEvents));
        PT_SCHEDULE(UnlockDriver(&ptUnlockDriver));
        PT_SCHEDULE(LockDriver(&ptLockDriver));
        PT_SCHEDULE(Ribsy(&ptRibsy));
    }
}
