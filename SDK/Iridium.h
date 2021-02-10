/*! \file
	\brief Platform Independent Iridium functions

	Hardware independent Iridium software
*/

#ifndef _IRIDIUM_H
#define _IRIDIUM_H

#define IRIDIUM_REFRESH_SEC 90			// Seconds before communications timeout and link is assumed lost

#define IRIDIUM_BAUD 2400 				// Iridium baud rate (DTE rate)
#define IRIDIUM_STATEMACHINE_ENABLED	// Comment out to disable the Iridium state machine.  You might want to do this when checking the data output rate. (test only).

#include "Types.h"
#include "Serial.h"
#include <stdlib.h>

//! Enumerate the Iridium modes
enum IridiumDefaultModes {
	HANGUP_OFF_MODE,			//!< Hangup to an idle state
	HANGUP_MODE,				//!< Hangup and resume dial or answer mode
	DIAL_MODE,					//!< Start out in dial mode
	AUTOANSWER_MODE,			//!< Start out in autoanswer mode
	CONNECTED_MODE				//!< Start out connected (and resume dial/answer mode as necessary)
};


#ifdef __cplusplus
extern "C" {
#endif

void EnableIridiumPower(void);				//!< Enable Iridium power
void DisableIridiumPower(void);				//!< Disable Iridium power

BOOL IridiumIsConnected(void);
BOOL IridiumIsEnabled(void);

void SetIridiumPhoneNumber(const char * pPhoneNumber);

BOOL bIridiumLostCarrier(Serial_t* pPort);

BOOL InitIridium(UInt8 chan, UInt32 QSize, int DefaultMode, const char* pNumber, Serial_t* pPort);
void InitIridiumExternalComm(Serial_t* pPort, int DefaultMode, const char* pNumber);
BOOL IridiumStateDriver(Serial_t* pPort, char* pNumber);
size_t GetIridiumStatusString(char* pString, size_t count);
size_t GetLastIridiumErrorString(char* pString, size_t count);

BOOL bIridiumCarrierDetectIsActive(Serial_t* pPort);
void SetIridiumDTRActive(Serial_t* pPort);
void SetIridiumDTRInactive(Serial_t* pPort);

void HangUpIridiumRedial(void);
void HangUpIridium(void);
void HangUpIridiumIdle(void);
void StartDialingIridium(void);

BOOL IridiumTimeoutHasExpired(void);
void RefreshIridiumTimeout(void);

#ifdef __cplusplus
}
#endif

#endif //_IRIDIUM_H
