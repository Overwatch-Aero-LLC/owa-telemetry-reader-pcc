/*! \file
	\brief Iridium Ground utils

	This module provides utilities for the Iridium state machine for ground based systems
*/
// IridiumGround.h
#ifndef _IRIDIUMGROUND_H
#define _IRIDIUMGROUND_H

#include "Serial.h"

enum IridiumCommModes {
	DIRECT_SERIAL_MODE,
	CLIENT_MODE,
	SERVER_MODE,
	IRIDIUM_DIRECT_DIAL_MODE,
	IRIDIUM_DIRECT_AUTOANSWER_MODE,
	IRIDIUM_TELNET_DIAL_MODE,
	N_MAX_COMM_MODES
};

#ifdef __cplusplus
extern "C" {
#endif

void DeinitIridium(Serial_t* pPort);
void DeinitIridiumExternalComm(Serial_t* pPort);
char* GetIridiumPhoneNumberPtr(void);

#ifdef __cplusplus
}
#endif

#endif
