/*! \file
	\brief The autopilot development comm packets.
	The file includes functions to form and decode Piccolo comm packets with special commands and
	data for autopilot development purposes. These are in a separate file from AutopilotPacket.c
	to prevent end-users from modifying the development parameters.
*/

///////////////////////////////////
// This file provided by:        //
// Cloud Cap Technology, Inc.    //
// 2621 Wasco Street             //
// PO Box 1500                   //
// Hood River, OR, 97031         //
// +1-541-387-2120    (voice)    //
// +1-541-387-2030    (fax)      //
// http://www.cloudcaptech.com   //
///////////////////////////////////

#ifndef _APDEV_PACKET_H
#define _APDEV_PACKET_H

#include "AutopilotPacket.h"


/*! Autopilot stream message types */
enum AutopilotDevPacketTypes
{
	DEV_GPS_ENABLE = 250,		//!< Command to force enable/disable use of GPS and/or barometer data
	DEV_GPS_POSVEL,				//!< GPS position and velocity sent regardless of the enable/disable status
	DEV_PACKET_RESERVED_252,
	DEV_PACKET_RESERVED_253,
	DEV_PACKET_RESERVED_254,
	DEV_PACKET_RESERVED_255
};

#ifdef __cplusplus
extern "C" {
#endif

//! Form a GPS position and velocity packet
void FormGPSPosVelPacket(AutopilotPkt_t* pPkt, double Lat, double Lon, double Alt, float VNorth, float VEast, float VDown);

//! Decode a GPS position and velocity packet
BOOL DecodeGPSPosVelPacket(const AutopilotPkt_t* pPkt, double* pLat, double* pLon, double* pAlt, float*pVNorth, float* pVEast, float* pVDown);

#ifdef __cplusplus
}
#endif

#endif
