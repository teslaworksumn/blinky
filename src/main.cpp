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
#include "ring_buffer.h"
}
#include <stdio.h>

#include "../../ribsy/include/pt.h"

// =============================================================================
// Types
// =============================================================================

typedef enum _EventCode {
	EventCodeNone,
	EventCodeIRApproach,
	EventCodeIRLeave,
	EventCodeRFIDData,
	EventCodeLock,
	EventCodeUnlock,
	EventCodeDurationData,
	EventCodeLockout,
	EventCodeEndLockout,
	EventCodeOpen,
	EventCodeClosed,
	EventCodeHandleOpen,
	EventCodeHandleClose,
	EventCodePowerOn,
	EventCodePowerOff,
	EventCodeNetworkConnectionUp,
	EventCodeNetworkConnectionDown
} EventCode;

typedef struct _Event {
    EventCode code;
    void *data;
} Event;

typedef void (*Statelet)(Event *event);

// =============================================================================
// Globals & Defines
// =============================================================================

#define EVENT_STACK_SIZE 32

bool InterceptingUART = false;

// Event stack

struct {
    int size;
    Event events[EVENT_STACK_SIZE];
} _EventStack;

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

// Stateling stack

struct {
    int size;
    Statelet statelets[EVENT_STACK_SIZE];
    int eventDepth;
} _StateletStack;

static void Base(Event *e);

void InitStateletStack() {
    _StateletStack.size = 0;
    _StateletStack.statelets[0] = &Base;
}

void PushStatelet(Statelet s) {
    _StateletStack.statelets[_StateletStack.size] = s;
    _StateletStack.size += 1;
}

void TryStatelet(Event *e) {
	for (_StateletStack.eventDepth = 0; _StateletStack.eventDepth < _StateletStack.size; _StateletStack.eventDepth += 1) {
		_StateletStack.statelets[_StateletStack.eventDepth](e);
	}
	if (_StateletStack.eventDepth != _StateletStack.size) {
		_StateletStack.statelets[_StateletStack.size - 1](NULL);
	}
}

void Topple(EventCode code, Statelet s, Event *e) {
	if (e->code == code) {
		int i = _StateletStack.size - _StateletStack.eventDepth;
		_StateletStack.statelets[i] = s;
		_StateletStack.size = i + 1;
	}
}

// =============================================================================
// Threads
// =============================================================================

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

static void Unlocked(Event *e) {
	if (e == NULL) {
		Board_UARTPutSTR("Unlocked loaded.");
		return;
	}
}

static void Locked(Event *e) {
	if (e == NULL) {
		Board_UARTPutSTR("Locked loaded.");
		return;
	}
}

static void Base(Event *e) {
	if (e == NULL) {
		Board_UARTPutSTR("Base loaded.");
		return;
	}

	Topple(EventCodeLock, &Locked, e);
	Topple(EventCodeUnlock, &Unlocked, e);
}

static struct pt ptDriveEvents;
static struct pt ptUnlockDriver;
static struct pt ptLockDriver;
static struct pt ptRibsy;

void Setup() {
	// General board initialization
	Board_Init();

	// Serial debug
	Board_Debug_Init();

	// Stacks
	InitEventStack();
	InitStateletStack();

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
