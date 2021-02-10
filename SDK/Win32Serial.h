/*! \file
	\brief Asynchronous serial interfaces for Windows.

	Win32Serial is a low level serial interface module suitable for direct
	inclusion in a project, or as the low level of a more abstract serial

*/

// Win32Serial.h
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

#ifndef _WIN32SERIAL_H
#define _WIN32SERIAL_H

#include <WTypes.h>
#include <Winbase.h>
#include <vector>

/*! Windows serial interface driver */
class CWin32Serial
{
protected:
	unsigned char  m_ucChan;	// serial channel number
	int            m_iBaud;		// baud rate
	BYTE           m_ucParity;	// parity
	BYTE           m_ucBits;	// # of data bits
	BYTE           m_ucStop;	// # of stop bits

public:
	HANDLE  m_hCom;		//!< Windows handle to com resource

public:
	CWin32Serial(void);          // constructor
	virtual ~CWin32Serial(void); // destructor

	// Opens the port and initializes comm settings
	BOOL bOpen(BYTE  ucPort = 1,
		       DWORD ulBaud = 9600,
		       BYTE  ucParity = NOPARITY,
		       BYTE  ucBits = 8,
		       BYTE  ucStop = ONESTOPBIT,
			   DWORD RxBuf = 512,
			   DWORD TxBuf = 512);

	// Parity Types  | Stop bit values
	// EVENPARITY    | ONESTOPBIT   -> one stop bit
	// MARKPARITY    | ONE5STOPBITS -> 1.5 stop bits
	// NOPARITY      | TWOSTOPBITS  -> 2 stop bits
	// ODDPARITY     |
	// SPACEPARITY   |

	// Closes the port
	void Close(void);

	// Call to learn the port number
	BYTE ucGetPort(void){return m_ucChan;}

	// Call to check if the port is open
	BOOL bIsOpen(void);

	void PurgeRxQ(void); 	//!< Clear the Rx buffer
	void PurgeTxQ(void);	//!< Clear the Tx buffer

    // Call to check if Carrier Detect is active
    BOOL bCarrierDetectIsActive(void);
	
	void SetDTRActive(void);
	void SetDTRInactive(void);

	// Input and output functions
	BOOL  bRead(BYTE* ucpBuf, DWORD ulNumBytes, DWORD* ulpCount);
	BOOL  bWrite(BYTE* ucpBuf, DWORD ulNumBytes, DWORD* ulpCount);
	DWORD DataWaiting(void);

    // Returns a list of available ports
    static std::vector<int> CreateSerialPortList( bool fast );
}; // CWin32Serial

#endif
