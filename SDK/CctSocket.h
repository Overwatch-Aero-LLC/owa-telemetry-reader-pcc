/*! \file
    \brief A class encapsulating buffered socket access.

    The CctSocket class is intended to provide a transparent interface to a
    socket regardless of the underlying transport layer protocol (TCP or UDP).
*/
// CctSocket.h
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

#ifndef CCTSOCKET_H
#define CCTSOCKET_H

#include "Queue.h"

//! Socket type enumeration
typedef enum
{
    SOCKET_TYPE_NULL,   //!< NULL socket (i.e. invalid)
    SOCKET_TYPE_TCP,    //!< TCP stream socket (currently unsupported)
    SOCKET_TYPE_UDP     //!< UDP datagram-based socket
} SocketType_t;

/*! IP socket interface class. */
class CctSocket
{
public:
    //! Constructor
    CctSocket(void);

    //! Destructor
    ~CctSocket(void);

    //! Open a new socket
    BOOL open(UInt32 Address, SocketType_t Type,
        UInt16 RemotePort, UInt16 LocalPort,
        UInt32 TxBufSize, UInt32 RxBufSize);

    //! Close this socket
    void close(void);

    //! Test for 
    BOOL isOpen(void) { return m_Type != SOCKET_TYPE_NULL; }

    //! Read bytes from an opened socket
    SInt32 read(UInt8 *pData, UInt32 MaxLength);

    //! Write bytes to an opened socket
    SInt32 write(UInt8 *pData, UInt32 Length, BOOL More);

protected:
    struct sockaddr_in *m_pTxSockAddr;
    UInt32              m_Handle;
    SocketType_t        m_Type;
    Queue_t            *m_pRxQ;
    Queue_t            *m_pTxQ;
    UInt8              *m_pRxBuf;
    UInt8              *m_pTxBuf;
};

#endif // CCTSOCKET_H