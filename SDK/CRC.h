/*! \file
	\brief ARC 16-bit cyclic redundancy check.

	ARC 16-bit cyclic redundancy check for blocks of 8-bit data of arbitrary 
	length.  Note that this is a reflected CRC on both the input bytes and
	output.  Useful CRC websites: http://www.ross.net/crc/download/crc_v3.txt
	and http://reveng.sourceforge.net/crc-catalogue/all.htm.  The check value 
	is based upon the following ASCII 9 bytes input: "123456789"

	Parameters:
	width=16 poly=0x8005 init=0x0000 refin=true refout=true xorout=0x0000 check=0xbb3d name="ARC"

	Alias: CRC-16, CRC-IBM, CRC-16/ARC, CRC-16/LHA
*/

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

#ifndef CRC_H
#define CRC_H

#include "Types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const UInt16 crctable[256];

UInt16 CRC16(const UInt8* pBuf, UInt32 len);
UInt16 CRC16OneByte(UInt8 Byte, UInt16 crc);

#ifdef __cplusplus
}
#endif

#endif // CRC_H
