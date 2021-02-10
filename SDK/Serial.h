/*! \file
    \brief Top level asynchronous serial interfaces for the MPC5XX

    Serial.c/h provides a highly abstracted, interrupt driven, interface to
    universal asynchronous receiver transmitters (UART).  The interface is
    designed to work with different underlying hardware including the SCI,
    and the TPU.  Flow control is not yet supported.

    Enumeration constants are provided for refering to the serial port channels
    The names have the following meanings:
    - COM1-COM4 refer to SCI1_a, SCI2_a, SCI1_b, and SCI2_b respectively.
    - COM5-COM10 are reserved
    - COM11-COM18 refer to serial ports created with pairs of TPU A
      channels.  The transmit channel is an even number and the receive
      channel is one greater than that (i.e. COM11 uses TPUA channel 0
      for transmit and TPUA channel 1 for receive).
    - COM19-COM26 refer to serial ports created with pairs of TPU B
      channels.  The transmit channel is an even number and the receive
      channel is one greater than that (i.e. COM19 uses TPUB channel 0
      for transmit and TPUB channel 1 for receive).
    - COM27-COM34 refer to serial ports created with pairs of TPU C
      channels.  The transmit channel is an even number and the receive
      channel is one greater than that (i.e. COM19 uses TPUC channel 0
      for transmit and TPUC channel 1 for receive).
*/

// Serial.h
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

#ifndef _SERIAL_H
#define _SERIAL_H

#include "Types.h"
#include "Queue.h"

enum
{
    NO_SERIAL_PORT, //!< Invalid serial port
    SCI_PORT,       //!< serial port on the SCI of the MPC555
    TPU_PORT,       //!< serial port on the TPU of the MPC555
    PC_PORT,        //!< serial port on a PC under windows or linux
    TCPCLIENT_PORT, //!< virtual serial port opened as an TCP client
    TCPSERVER_PORT, //!< virtual serial port opened as an TCP server
    IPMULTI_PORT,   //!< virtual serial port as IP multicast
    NUM_PORT_TYPES,
    INVALID_PORT = 0xFF //!< Also an invalid serial port
};

typedef struct
{
    UInt32 Handle;  //!< OS Handle for desktop OSs, or Module address for MPC5XX, or socket for virtual port
    UInt8 Type   ;  //!< Type of serial port device
    UInt8 RxChan ;  //!< Channel number for TPU receive channel
    UInt8 TxChan ;  //!< Channel number for TPU transmit channel
    UInt8 chan   ;  //!< Old style port enumerator
    Queue_t* pRxQ;  //!< Receive buffer
    Queue_t* pTxQ;  //!< Transmit buffer

    #ifndef __MWERKS__
    // Extra data that non-555 systems might need
    UInt32 BaudRate;
    UInt32 TransmitBufferEstimate;
    UInt32 TxQSize;
    UInt64 LastTransmitTime;
    UInt64 TimeOfLastData;
    UInt32 ThreadID;
    int ServeSocket;
    #endif

}Serial_t;


/** @name Asynchronous serial direction constants */
//@{
#define RX_DIR 1                //!< Receive direction
#define TX_DIR 2                //!< Transmit direciton
#define BOTH_DIR (RX_DIR|TX_DIR)//!< Both directions
//@}

/** @name Asynchronous serial parity constants */
//@{
#define NO_PARITY 0     //!< No parity checking
#define ODD_PARITY  1   //!< Odd parity checking
#define EVEN_PARITY 2   //!< Even parity checking
//@}

/** @name Asynchronous serial flow control constants */
//@{
#define FLOW_NONE   0x00000000  //!< No flow control
#define FLOW_SOFT   0x00000001  //!< Software flow control XON/XOFF
#define FLOW_HARD   0x00000002  //!< Hardware flow control RTS/CTS
//@}


/*! Serial port names */
enum SerialPortNames
{
    COM1 = 0,   // SCI1_a
    COM2,       // SCI2_a
    COM3,       // SCI1_b
    COM4,       // SCI2_b
#ifndef __MWERKS__
    COM5,
    COM6,
    COM7,
    COM8,
    COM9,
    COM10,
#endif
    COM11,      // TPUA - 0,1
    COM12,      // TPUA - 2,3
    COM13,      // TPUA - 4,5
    COM14,      // TPUA - 6,7
    COM15,      // TPUA - 8,9
    COM16,      // TPUA - 10,11
    COM17,      // TPUA - 12,13
    COM18,      // TPUA - 14,15
    COM19,      // TPUB - 0,1
    COM20,      // TPUB - 2,3
    COM21,      // TPUB - 4,5
    COM22,      // TPUB - 6,7
    COM23,      // TPUB - 8,9
    COM24,      // TPUB - 10,11
    COM25,      // TPUB - 12,13
    COM26,      // TPUB - 14,15
    COM27,      // TPUC - 0,1
    COM28,      // TPUC - 2,3
    COM29,      // TPUC - 4,5
    COM30,      // TPUC - 6,7
    COM31,      // TPUC - 8,9
    COM32,      // TPUC - 10,11
    COM33,      // TPUC - 12,13
    COM34,      // TPUC - 14,15
    NUM_COM
};

#ifdef __cplusplus
extern "C" {
#endif

// Prototypes
BOOL OpenCOMM(UInt8 chan, UInt8 dir, UInt32 baud, UInt8 parity,
              UInt8 data, UInt32 flow, UInt32 QSize, Serial_t* pPort);
BOOL OpenCOMMEx(UInt8 chan, UInt8 dir, UInt32 baud, UInt8 parity,
                UInt8 data, UInt32 flow, UInt32 QSize, BOOL Loop, Serial_t* pPort);
BOOL OpenCOMMDev(const char* sDevice, UInt8 dir, UInt32 baud, UInt8 parity,
                 UInt8 data, UInt32 flow, UInt32 QSize, Serial_t* pPort);
void GetSerialPortDescriptor(UInt8 chan, Serial_t* pPort);
void CloseCOMM(Serial_t* pPort);
BOOL IsCOMMOpen(Serial_t* pPort);
UInt32 GetBaud(Serial_t* pPort);
void ChangeBaud(Serial_t* pPort, UInt32 baud);
void WriteByteQuick(Serial_t* pPort, UInt8 data);
SInt16 ReadByte(Serial_t* pPort);
SInt16 ReadByteQuick(Serial_t* pPort);
UInt32 ReadBlockQuick(Serial_t* pPort, UInt8* pData, UInt32 Size);
UInt32 ReadString(Serial_t* pPort, char* pData, UInt32 Size);
UInt32 ReadStringUntil(Serial_t* pPort, char* pData, UInt32 MaxSize, const char *pString);
void ReverseSerialRxQueue(Serial_t* pPort, UInt32 Size);
void WriteBlockQuick(Serial_t* pPort, const UInt8* pData, UInt32 Size);
void WriteFromQueueQuick(Serial_t* pPort, Queue_t* pQ, UInt32 Size);
UInt32 WriteStringQuick(Serial_t* pPort, const char* pData);
UInt32  RxQAvailable(Serial_t* pPort);
UInt32  RxQHolding(Serial_t* pPort);
UInt32  TxQAvailable(Serial_t* pPort);
UInt32  TxQHolding(Serial_t* pPort);
void PurgeRxQ(Serial_t* pPort);
void PurgeTxQ(Serial_t* pPort);
void AssertBreak(Serial_t* pPort);
void ClearBreak(Serial_t* pPort);
BOOL CheckForResponse(Serial_t* pPort, const char *pString);
BOOL CheckForMultipleResponse(Serial_t* pPort, const char *pString, UInt32 NumResponse);
BOOL CheckForResponseEx(Serial_t* pPort, const char *pString, BOOL Clear);
UInt32 CheckForMultipleResponseEx(Serial_t* pPort, const char *pString, UInt32 NumResponse, BOOL Clear);
Serial_t GetNullSerialPort(void);

// These routines are not supported on the MPC555
BOOL bCarrierDetectIsActive(Serial_t* pPort);
void SetDTRActive(Serial_t* pPort);
void SetDTRInactive(Serial_t* pPort);

#ifdef __cplusplus
}
#endif

#endif // _SERIAL_H

