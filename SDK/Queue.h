/*! \file
    \brief First in first out ring buffer queues for 8-bit data.

    The queue module defines a basic ring buffer format and functions for
    adding data (EnQ) and removing data (DeQ).
*/

// queue.h
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

#ifndef QUEUE_H
#define QUEUE_H

#include "Types.h"

/*! First in first out ring buffer type*/
typedef struct
{
    UInt8*  pBuf;   //!< Pointer to the actual buffer storage
    UInt32  In;     //!< The index of the next data to be EnQ'd
    UInt32  Out;    //!< The index of the next data to be DeQ'd
    UInt32  Size;   //!< The size of the buffer
} Queue_t;

// extern C linkage is needed for the DigiVSM project (change to #ifdef DIGICONNECTME if needed).
#ifdef __cplusplus
extern "C" {
#endif

// Prototypes for Q functions
Queue_t*    CreateQ(UInt32 Size);
void    DestroyQ(Queue_t* pQ);
SInt16  DeQ(Queue_t* pQ);
SInt16  DeQFast(Queue_t* pQ);
UInt32  BlockDeQ(Queue_t* pQ, UInt8* pData, UInt32 Size);
UInt32  BlockDeQFast(Queue_t* pQ, UInt8* pData, UInt32 Size);
UInt32  ReverseDeQ(Queue_t* pQ, UInt32 Size);
UInt32  ReverseDeQFast(Queue_t* pQ, UInt32 Size);
void    EnQFast(Queue_t* pQ, UInt8 Data);
void    EnQSuperFast(Queue_t* pQ, UInt8 Data);
void    EnQ(Queue_t* pQ, UInt8 Data);
UInt32  BlockEnQ(Queue_t* pQ, const UInt8* pData, UInt32 Size);
UInt32  BlockEnQFast(Queue_t* pQ, const UInt8* pData, UInt32 Size);
UInt32  BlockQTransfer(Queue_t* pRxQ, Queue_t* pTxQ, UInt32 Size);
UInt32  BlockQTransferFast(Queue_t* pRxQ, Queue_t* pTxQ, UInt32 Size);
void    BlockEnQSmall(Queue_t* pQ, const UInt8* pData, UInt32 Size);
void    BlockEnQSmallFast(Queue_t* pQ, const UInt8* pData, UInt32 Size);
UInt32  ReverseEnQ(Queue_t* pQ, UInt32 Size);
UInt32  ReverseEnQFast(Queue_t* pQ, UInt32 Size);
UInt32  QAvailable(Queue_t* pQ);
UInt32  QAvailableFast(Queue_t* pQ);
UInt32  QHolding(Queue_t* pQ);
UInt32  QHoldingFast(Queue_t* pQ);
BOOL    IsQFull(Queue_t* pQ);
BOOL    IsQFullFast(Queue_t* pQ);
void    ResetQ(Queue_t* pQ);
void    ResetQEx(Queue_t* pQ, UInt32 Value);
UInt32  BackupQueueIndex(UInt32 In, UInt32 Out, UInt32 Size, UInt32 Backup);

#ifdef __cplusplus
}
#endif

#endif // QUEUE_H
