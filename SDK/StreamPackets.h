/*! \file
	\brief Stream packet utilities

	This module provides the functions required to parse ground station,
	pilot, autopilot, and CAN stream packets from input buffers.
*/

// StreamPackets.h
///////////////////////////////////
// This file provided by:        //
// Cloud Cap Technology, Inc.    //
// PO Box 1500                   //
// No. 8 Fourth St.              //
// Hood River, OR, 97031         //
// +1-541-387-2120    (voice)    //
// +1-541-387-2030    (fax)      //
// http://www.cloudcaptech.com   //
///////////////////////////////////

#ifndef _STREAM_PACKET_H
#define _STREAM_PACKET_H

#include "Queue.h"


#define MAX_SIMPLE_DATA		255  //!< The maximum amount of data that a simple packet can contain is 255 bytes
#define SIMPLE_PKT_HEADER	4UL  //!< Simple stream packets have 4 byte headers
#define SIMPLE_PKT_OVERHEAD	6UL  //!< Simple stream packets have 6 bytes of overhead
#define SIMPLE_SYNC			0xA0 //!< First simple stream packet synchronization character
#define SIMPLE_SYNC1		0x05 //!< Second simple stream packet synchronization character


/*! Simple packet structure */
typedef struct
{
	UInt8 Sync;					//!< First packet synchronization character.
	UInt8 Sync1;				//!< Second packet synchronization character.
	UInt8 PktType;				//!< Packet identifier.
	UInt8 Size;					//!< Amount of data in the packet.
	UInt8 Data[MAX_SIMPLE_DATA+2];	//!< Space for data plus 2 crc.

	#ifndef DOXYGEN_SHOULD_IGNORE_THIS
	// Fields used for receive state machine
	UInt32 State;					// State of the packet in progress
	UInt32 i;						// Index into data in packet in progress
	#endif // DOXYGEN_SHOULD_IGNORE_THIS
}SimplePkt_t;


/*! Generic ground station stream packet structure */
typedef SimplePkt_t GroundStationPkt_t;

/*! Generic autopilot stream packet structure */
typedef SimplePkt_t AutopilotPkt_t;

/*! Generic pilot packet stream packet structure */
typedef SimplePkt_t PilotPkt_t;

/*! Pilot in the loop stream message types */
enum PilotPacketTypes
{
	RESERVED,
	PILOT_COMMAND_PULSE,		//!< Pilot command packet containing pulse widths
	GROUND_STATION_SWITCHES		//!< Ground station switches
};


#define MAX_CAN_DATA		8    //!< The maximum amount of data that a CAN stream packet can contain is 8 bytes
#define CAN_PKT_HEADER		10UL //!< CAN stream packets have 10 byte headers
#define CAN_PKT_OVERHEAD	12UL //!< CAN stream packets have 12 bytes of overhead
#define CAN_SYNC 			0x05 //!< First CAN stream packet synchronization character
#define CAN_SYNC1			0xA0 //!< Second CAN stream packet synchronization character

/*! Generic can packet stream packet structure */
typedef struct
{
	UInt8 Sync;							//!< First packet synchronization character.
	UInt8 Sync1;						//!< Second packet synchronization character.
	UInt8 ID_0;							//!< Most significant byte of can message ID.
	UInt8 ID_1;							//!< Next most significant byte of can message ID.
	UInt8 ID_2;							//!< Next most significant byte of can message ID.
	UInt8 ID_3;							//!< Least significant byte of can message ID.
	UInt8 Time_0;						//!< Most significant byte of can message time stamp.
	UInt8 Time_1;						//!< Least significant byte of can message time stamp.
	UInt8 Bus;							//!< Indicates which bus to use, 0 for A, 1 for B, etc.
	UInt8 Size;							//!< Amount of data in the packet.
	UInt8 Data[MAX_CAN_DATA+2];			//!< Space for data plus 2 crc.

	#ifndef DOXYGEN_SHOULD_IGNORE_THIS
	// Fields used for receive state machine
	UInt32 State;					// State of the packet in progress
	UInt32 i;						// Index into data in packet in progress
	#endif // DOXYGEN_SHOULD_IGNORE_THIS
}CANPacket_t;

#ifdef __cplusplus
extern "C" {
#endif

// Prototypes
BOOL LookForCANPacket(Queue_t* pRxQ, CANPacket_t* pPkt);
BOOL LookForAutopilotPacket(Queue_t* pRxQ, AutopilotPkt_t* pPkt);
BOOL LookForPilotPacket(Queue_t* pRxQ, PilotPkt_t* pPkt);
BOOL LookForGroundStationPacket(Queue_t* pRxQ, GroundStationPkt_t* pPkt);
BOOL LookForAutopilotPacketInBuf(UInt8* pData, AutopilotPkt_t* pPkt, UInt32 Size, UInt32* pUsed);
BOOL LookForPilotPacketInBuf(UInt8* pData, PilotPkt_t* pPkt, UInt32 Size, UInt32* pUsed);
BOOL LookForGroundStationPacketInBuf(UInt8* pData, GroundStationPkt_t* pPkt, UInt32 Size, UInt32* pUsed);
BOOL LookForSimplePacket(Queue_t* pRxQ, SimplePkt_t* pPkt);
BOOL LookForSimplePacketInBuf(UInt8* pData, SimplePkt_t* pPkt, UInt32 Size, UInt32* pUsed);

SInt32 LookForCANPacketInByte(UInt8 Byte, CANPacket_t* pPkt);
SInt32 LookForAutopilotPacketInByte(UInt8 Byte, AutopilotPkt_t* pPkt);
SInt32 LookForPilotPacketInByte(UInt8 Byte, PilotPkt_t* pPkt);
SInt32 LookForGroundStationPacketInByte(UInt8 Byte, GroundStationPkt_t* pPkt);
SInt32 LookForSimplePacketInByte(UInt8 Byte, SimplePkt_t* pPkt);
void MakeCANPacket(CANPacket_t* pPkt, UInt32 ID, UInt16 Time, UInt8 Bus, UInt8 DataSize);
void MakeAutopilotPacket(AutopilotPkt_t* pPkt, UInt8 PktType, UInt8 DataSize);
void MakePilotPacket(PilotPkt_t* pPkt, UInt8 PktType, UInt8 DataSize);
void MakeGroundStationPacket(GroundStationPkt_t* pPkt, UInt8 PktType, UInt8 DataSize);
void MakeSimplePacket(SimplePkt_t* pPkt, UInt8 PktType, UInt8 DataSize);

#ifdef __cplusplus
}
#endif

#endif // _SIMPLE_PACKET_H
