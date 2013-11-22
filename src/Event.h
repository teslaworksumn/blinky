//
//  Event.h
//  blinky
//
//  Created by Taylor Trimble.
//  Copyright (c) 2013 Taylor Trimble. All rights reserved.
//

#ifndef EVENT_H_
#define EVENT_H_

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

#endif /* EVENT_H_ */
