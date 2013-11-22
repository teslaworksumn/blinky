//
//  Event.h
//  blinky
//
//  Created by Taylor Trimble.
//  Copyright (c) 2013 Taylor Trimble. All rights reserved.
//

#ifndef EVENT_H_
#define EVENT_H_

#define EVENT_STACK_SIZE 32

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

struct {
    int size;
    Event events[EVENT_STACK_SIZE];
} _EventStack;

void InitEventStack();
Event *PushEvent();
Event *PeekEvent();
Event *PopEvent();

#endif /* EVENT_H_ */
