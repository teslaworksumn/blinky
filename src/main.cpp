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

#include "board.h"
extern "C" {
#include "Event.h"
#include "Statelet.h"
#include "ring_buffer.h"
}
#include <stdio.h>

#include "../../ribsy/include/pt.h"

// =============================================================================
// Globals & Defines
// =============================================================================

bool InterceptingUART = false;

static struct pt ptDriveEvents;
static struct pt ptUnlockDriver;
static struct pt ptLockDriver;
static struct pt ptRibsy;

static EventCode MapCharacterToEventCode(int c);

static void BaseRun();
static void BaseHandleEvent(Event *e);
static void UnlockedRun();
static void UnlockedHandleEvent(Event *e);
static void LockedRun();
static void LockedHandleEvent(Event *e);

Statelet Base = { BaseRun, BaseHandleEvent };
Statelet Unlocked = { UnlockedRun, UnlockedHandleEvent };
Statelet Locked = { LockedRun, LockedHandleEvent };

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
		Event *e = PushEvent();
		e->code = MapCharacterToEventCode(c);
	}

    PT_END(pt);
}

static PT_THREAD(UnlockDriver(struct pt *pt))
{
    PT_BEGIN(pt);

    while (1) {
		PT_WAIT_UNTIL(pt, PeekEvent()->code == EventCodeUnlock);
		PopEvent();
		Board_UARTPutChar('u');
    }

    PT_END(pt);
}

static PT_THREAD(LockDriver(struct pt *pt))
{
    PT_BEGIN(pt);

    while (1) {
		PT_WAIT_UNTIL(pt, PeekEvent()->code == EventCodeLock);
		PopEvent();
		Board_UARTPutChar('l');
    }

    PT_END(pt);
}

static PT_THREAD(Ribsy(struct pt *pt))
{
	static Event *e;

    PT_BEGIN(pt);

    while (1) {
    	PT_WAIT_UNTIL(pt, (e = PopEvent()) != NULL);
    	TryStatelet(e);
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

static void UnlockedRun() {
	Board_UARTPutSTR("Unlocked loaded.");
}

static void UnlockedHandleEvent(Event *e) {
}

static void LockedRun() {
	Board_UARTPutSTR("Locked loaded.");
}

static void LockedHandleEvent(Event *e) {
}

static void BaseRun() {
	Board_UARTPutSTR("Base loaded.");
}

static void BaseHandleEvent(Event *e) {
	Topple(EventCodeLock, Locked, e);
	Topple(EventCodeUnlock, Unlocked, e);
}

void Setup() {
	// General board initialization
	Board_Init();

	// Serial debug
	Board_Debug_Init();

	// Stacks
	InitEventStack();
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

    while(1) {
        PT_SCHEDULE(DriveEvents(&ptDriveEvents));
        PT_SCHEDULE(UnlockDriver(&ptUnlockDriver));
        PT_SCHEDULE(LockDriver(&ptLockDriver));
        PT_SCHEDULE(Ribsy(&ptRibsy));
    }
}
