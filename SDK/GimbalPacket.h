/*! \file
	\brief Gimbal packet encode and decode functionality
*/

///////////////////////////////////
// This file provided by:        //
// Cloud Cap Technology, Inc.    //
// 2621 Wasco St.                //
// PO Box 1500                   //
// Hood River, OR, 97031         //
// +1-541-387-2120    (voice)    //
// +1-541-387-2030    (fax)      //
// http://www.cloudcaptech.com   //
///////////////////////////////////

#ifndef _GIMBAL_PACKET_H
#define _GIMBAL_PACKET_H

#include "Queue.h"
#include "Serial.h"
#include "GimbalPacketGeneric.h"

#ifdef __cplusplus
extern "C" {
#endif

//! Look for a gimbal packet arriving over a serial port.
BOOL LookForGimbalPacket(GimbalPkt_t* pPkt, Serial_t* pPort);

//! Look for a gimbal packet in a ring buffer.
BOOL LookForGimbalPacketInQueue(GimbalPkt_t* pPkt, Queue_t* pQ);

//! Send a gimbal packet over a serial port.
void SendGimbalPacket(const GimbalPkt_t* pPkt, Serial_t* pPort);

#ifdef __cplusplus
}
#endif

#endif // _GIMBAL_PACKET_H
