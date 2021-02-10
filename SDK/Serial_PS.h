
///////////////////////////////////
// This file provided by:        //
// Cloud Cap Technology, Inc.    //
// 2621 Wasco Street.            //
// PO Box 1500                   //
// Hood River, OR, 97031         //
// +1-541-387-2120    (voice)    //
// +1-541-387-2030    (fax)      //
// http://www.cloudcaptech.com   //
///////////////////////////////////

#ifndef _SERIAL_PS_H
#define _SERIAL_PS_H

#include "Types.h"

#ifndef INVALID_HANDLE_VALUE
    #define INVALID_HANDLE_VALUE ((UInt32)-1)
#endif

// We define these constants here so that use of this module does not require
//   importing serial.h


/** @name Asynchronous serial direction constants */
//@{
#ifndef RX_DIR
#define RX_DIR 1				//!< Receive direction
#endif

#ifndef TX_DIR
#define TX_DIR 2				//!< Transmit direciton
#endif

#ifndef BOTH_DIR
#define BOTH_DIR (RX_DIR|TX_DIR)//!< Both directions
#endif
//@}


/** @name Asynchronous serial parity constants */
//@{
#ifndef NO_PARITY
#define NO_PARITY 0		//!< No parity checking
#endif

#ifndef ODD_PARITY
#define ODD_PARITY  1	//!< Odd parity checking
#endif

#ifndef EVEN_PARITY
#define EVEN_PARITY 2	//!< Even parity checking
#endif
//@}


/** @name Asynchronous serial flow control constants */
//@{
#ifndef FLOW_NONE
#define FLOW_NONE	0x00000000	//!< No flow control
#endif

#ifndef FLOW_SOFT
#define FLOW_SOFT	0x00000001	//!< Software flow control XON/XOFF
#endif

#ifndef FLOW_HARD
#define FLOW_HARD	0x00000002	//!< Hardware flow control RTS/CTS
#endif
//@}

#ifdef __cplusplus
extern "C" {
#endif

void psInitCOMM(void);
UInt32 psOpenCOMM(UInt8 chan, UInt8 dir, UInt32 baud, UInt8 parity,
			  UInt8 data, UInt32 flow, UInt32 QSize);
UInt32 psOpenCOMMDev(const char* sDevice, UInt8 dir, UInt32 baud, UInt8 parity,
                     UInt8 data, UInt32 flow, UInt32 QSize);
void psCloseCOMM(UInt32 Handle);
SInt16 psReadByteQuick(UInt32 Handle);
UInt32 psReadBlockQuick(UInt32 Handle, UInt8* pData, UInt32 Size);
UInt32 psWriteBlockQuick(UInt32 Handle, const UInt8* pData, UInt32 Size);
BOOL psIsCOMMOpen(UInt32 Handle);
void psChangeBaud(UInt32 Handle, UInt32 baud);
BOOL psIsCarrierDetectActive(UInt32 Handle);
void psSetDTRActive(UInt32 Handle);
void psSetDTRInactive(UInt32 Handle);
void psAssertBreak(UInt32 Handle);
void psClearBreak(UInt32 Handle);
void psPurgeRxQ(UInt32 Handle);
void psPurgeTxQ(UInt32 Handle);

#ifdef __cplusplus
}
#endif

#endif // _SERIAL_PS_H
