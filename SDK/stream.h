/*! \file
	\brief Stream packet protocol for the Piccolo system

	The data sent to or from any one avionics are made up of multiple
	bi-directional streams which are multiplexed onto the communications
	channel.  Each stream represents an endpoint in the avionics, which is
	either defined by software or by a physical port on the avionics.  The
	stream module defines a basic stream packet format, and provides low
	level stream packet formatting and decoding routines.

	Note that the stream packet protocol definition changed from software
	versions 1.1.x to 1.2.x.  They changes are not backwards compatible
*/

// Stream.h
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

#ifndef _STREAM_H
#define _STREAM_H

#include "Types.h"
#include "Queue.h"
#include "Serial.h"

// Stream packet protocol
// BYTE 	  | NAME	  | MEANING
// 0		  | SYNC	  | Synchronization character, delimits start of packet
// 1		  | SYNC1	  | Synchronization character, delimits start of packet
// 2		  | DestHi    | Most significant byte of destination address
// 3		  | DestLo    | Least significant byte of destination address
// 4		  | SourceHi  | Most significant byte of source address
// 5		  | SourceLo  | Least significant byte of source address
// 6		  | SeqHi     | Most significant byte of the sequence number
// 7		  | SeqLo     | Least significant byte of the sequence number
// 8		  | AckHi	  | Most significant byte of the acknowledge number
// 9		  | AckLo	  | Least significant byte of the acknowledge number
// 10		  | Stream	  | Stream identifier
// 11		  | Flags	  | Special packet flags
// 12		  | Size	  | The number of payload bytes
// 13		  | HeaderChk | Checksum, XOR checksum of bytes 0...6
// 14-Size+13 | Data	  | Size data bytes
// Size+14	  | CRCHi	  | Most significant byte of CRC
// Size+15	  | CRCLo	  | Least significant byte of CRC

/*! Stream packets have 16 bytes of overhead */
#define STREAM_PKT_OVERHEAD 16

/*! Stream packets have 14 byte headers */
#define STREAM_PKT_HEADER (STREAM_PKT_OVERHEAD-2)

/*! The maximum amount of data that a stream packet can contain is 255 bytes */
#define MAX_STREAM_DATA_SIZE 255

/*! The entire stream packet can be no more than 271 bytes */
#define MAX_STREAM_PKT_SIZE (MAX_STREAM_DATA_SIZE+STREAM_PKT_OVERHEAD)

#define STREAM_SYNC 0x5A	//!< First packet synchronization character
#define STREAM_SYNC1 0xA5	//!< Send packet synchronication character

#define BROADCAST_ADDRESS	  0xFFFF	//!< Address for broadcasting to all avionics
#define OI_ADDRESS			  0xFFFE	//!< Address for all operator interfaces
#define GS_ADDRESS			  0x0000	//!< Address for all ground stations
#define MIN_AUTOPILOT_ADDRESS 0x0001    //!< Minimum address for avionics
#define MAX_AUTOPILOT_ADDRESS 0xFFF0    //!< Maximum address for avionics

/*! Generic stream packet structure */
typedef struct
{
	UInt8 Sync;			//!< First packet synchronization character
	UInt8 Sync1;		//!< Second packet synchronization character
	UInt8 DestHi;		//!< Upper byte of 16-bit destination address
	UInt8 DestLo;		//!< Lower byte of 16-bit destination address
	UInt8 SourceHi;		//!< Upper byte of 16-bit source address
	UInt8 SourceLo;		//!< Lower byte of 16-bit source address
	UInt8 SeqHi;		//!< Upper byte of 16-bit Sequence counter
	UInt8 SeqLo;		//!< Lower byte of 16-bit Sequence counter
	UInt8 AckHi;		//!< Upper byte of 16-bit Acknowledge counter
	UInt8 AckLo;		//!< Lower byte of 16-bit Acknowledge counter
	UInt8 Stream;		//!< Stream ID
	UInt8 Flags;		//!< Special packet flags
	UInt8 Size;			//!< Size of data
	UInt8 HdrChk;		//!< Checksum on header
	UInt8 Data[MAX_STREAM_DATA_SIZE+2];	//!< Data plus 2 CRC

	#ifndef DOXYGEN_SHOULD_IGNORE_THIS
		// Fields used for receive state machine
	UInt32 State;					// State of the packet in progress
	UInt32 i;						// Index into data in packet in progress
	#endif // DOXYGEN_SHOULD_IGNORE_THIS
} StreamPkt_t;


// Prototypes
#ifdef __cplusplus
extern "C" {
#endif

BOOL LookForStreamPacket(StreamPkt_t* pPkt, Serial_t* pPort);
BOOL LookForStreamPacketInQueue(StreamPkt_t* pPkt, Queue_t* pQ);
SInt32 LookForStreamPacketInByte(StreamPkt_t* pPkt, UInt8 Byte);
void MakePacketHeader(StreamPkt_t* pPkt, UInt16 DestinationID, UInt16 SourceID,
				UInt8 Stream, UInt8 Flags, UInt16 Sequence, UInt16 Acknowledge,
				UInt16 DataSize);
void MakePacket(StreamPkt_t* pPkt, UInt16 DestinationID, UInt16 SourceID,
				UInt8 Stream, UInt8 Flags, UInt16 Sequence, UInt16 Acknowledge,
				const UInt8* pData, UInt16 DataSize);
void CRCPacket(StreamPkt_t* pPkt);

#ifdef __cplusplus
}
#endif

#endif // _STREAM_H
