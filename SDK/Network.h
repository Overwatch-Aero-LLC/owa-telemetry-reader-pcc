/*! \file
	\brief Stream multiplex logic for Piccolo

	The network module sits atop the stream module and provides functionality
	for dealing with the multiple streams that move over stream packets
*/
// Network.h
///////////////////////////////////
// This file provided by:        //
// Cloud Cap Technology, Inc.    //
// PO Box 1500                   //
// 2621 Wasco St.                //
// Hood River, OR, 97031         //
// +1-541-387-2120    (voice)    //
// +1-541-387-2030    (fax)      //
// http://www.cloudcaptech.com   //
///////////////////////////////////

#ifndef _NETWORK_H
#define _NETWORK_H

#include "Types.h"
#include "Queue.h"
#include "stream.h"
#include "Timer.h"

/*!	Stream identifiers */
enum StreamIdentifiers
{
	POLLING_STREAM,		//!< Flow control stream
	DGPS_STREAM,		//!< Differential corrections stream
	PILOT_STREAM,		//!< Manual pilot command stream
	AUTOPILOT_STREAM,	//!< Autopilot command and telemetry stream
	APP_STREAM,         //!< Autopilot application stream
	GIMBAL_STREAM,		//!< Camera gimbal stream
	PAYLOAD_STREAM,		//!< Data from the payload comm port
	PAYLOAD2_STREAM,	//!< Data from the second payload comm port
	DEBUG_STREAM,		//!< Stream for moving debug data
	CAN_STREAM,			//!< Stream for moving CAN pass through data
	ADSB_STREAM,        //!< Stream for downlinking ADS-B receiver data
	NUM_STREAMS,		//!< The number of streams
	
	USER_LOG_STREAM = 0xFD, //!< Stream for logging custom binary data from UI to tel file
	KEEPALIVE_STREAM = 0xFE,
	ENUMERATION_STREAM = 0xFF
};


/*! When talking to a ground station*/
#define GS_STREAM 3

#define AUTOPILOT_STREAM_SIZE 4096
#define GIMBAL_STREAM_SIZE 1024

#if (AUTOPILOT_STREAM_SIZE > GIMBAL_STREAM_SIZE)
	#define MAX_STREAM_SIZE AUTOPILOT_STREAM_SIZE
#else
	#define MAX_STREAM_SIZE GIMBAL_STREAM_SIZE
#endif

/*! Data for a single bi-directional stream */
typedef struct
{
	Queue_t* pTxQ;		//!< Pointer to transmit buffer
	Queue_t* pRxQ;		//!< Pointer to receive buffer
	Timer_t	AckTimer;	//!< Local ack timer
	Timer_t LastAckTimeout;	//!< Remote ack timer
	UInt16 LastSeq;		//!< Sequence value of last packet
	UInt16 RemoteSeq;	//!< The sender's sequence number
	UInt16 RemoteAck;	//!< The sender's acknowledge number
	UInt16 LocalSeq;	//!< Our sequence number
	UInt16 LastAck;		//!< Our acknowledge number
	UInt16 NumberOfUnAcks;//!< Number of times the ack timer has elapsed
	UInt8  Flags;		//!< Flags for this stream
	UInt8  Send;		//!< Send flag
} Stream_t;


/*! Data for a network of multiple bi-directional streams */
typedef struct
{
	Stream_t Streams[NUM_STREAMS]; //!< Array of stream data objects
	Timer_t	 KeepAliveTimer;//!< Keep alive timer
	UInt32   DataExpected;	//!< Amount of data we expected to received based on sequence number changes
	UInt32   DataReceived;	//!< Amount of data we received based on cumulative sizes
	UInt16	 DestID;		//!< Network address of the other side
	UInt16	 MyID;			//!< My network address
	UInt16   LastKeepAliveSeq;	//!< Keep alive sequence from CommSDK
	UInt8	 NumStreams;	//!< Size of the array
	UInt8	 PollCounter;	//!< Incrementing polling counter
	UInt8	 PollAck;		//!< Last poll acknowledgement
	UInt8	 LastStream;	//!< Last stream serviced by transmit
	BOOL	 DataWaiting;	//!< Flag to indicate if data are waiting
}Network_t;


// Prototypes
#ifdef __cplusplus
extern "C" {
#endif

BOOL InitNetwork(Network_t* pNet, UInt16 DestID, UInt16 MyID);
void CloseNetwork(Network_t* pNet);
BOOL OpenStream(Network_t* pNet, UInt8 Stream, UInt8 dir, UInt32 QSize);
void CloseStream(Network_t* pNet, UInt8 Stream);
void ResetNetworkBuffers(Network_t* pNet);
void QueueDataForTransmit(Stream_t* pStream, const UInt8* pBuf, UInt32 len);

#ifdef __cplusplus
}
#endif

#endif // _NETWORK_H
