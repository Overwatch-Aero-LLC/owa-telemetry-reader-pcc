
///////////////////////////////////
// This file provided by:        //
// Cloud Cap Technology, Inc.    //
// PO Box 1500                   //
// No. 8 Fourth St.              //
// Hood River, OR, 97031         //
// +1-541-387-2120    (voice)    //
// +1-541-387-2030    (fax)      //
// vaglient@gorge.net (e-mail)   //
// http://www.gorge.net/cloudcap //
///////////////////////////////////

#ifndef _SERIAL_PC_H
#define _SERIAL_PC_H

#include "Serial.h"

#define SERIAL_TCP_CLIENT_MASK 0x40
#define SERIAL_TCP_SERVER_MASK 0x80
#define SERIAL_MULTICAST_MASK 0xC0
#define SERIAL_IP_MASK 0xC0
#define SERIAL_CHAN_MASK 0x3F

#ifdef __cplusplus
extern "C" {
#endif

void PrintCommPortData(UInt8 Port);
BOOL TestForMulticast(void);
void SetSerialIPEmulationData(const char* ServerAddress, UInt16 TCPBase, UInt16 MCPBase);
int CreateTCPClient(const char* ServerAddress, UInt16 Port);
int CreateUDPClient(const char* ServerAddress, UInt16 Port);

#ifdef __cplusplus
}
#endif

#endif // _SERIAL_PC_H
