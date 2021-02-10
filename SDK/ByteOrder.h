/*! \file
	\brief Network and host byte ordering functions.

	This module contains the functions to switch between network and host byte order.
*/

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

#ifndef BYTE_ORDER_H
#define BYTE_ORDER_H

#include "Types.h"

#ifdef __cplusplus
extern "C" {
#endif

UInt16 DataToUInt16(const UInt8* pData);
SInt16 DataToSInt16(const UInt8* pData);
UInt32 DataToUInt24(const UInt8* pData);
UInt32 DataToUInt32(const UInt8* pData);
SInt32 DataToSInt32(const UInt8* pData);
UInt64 DataToUInt64(const UInt8* pData);
SInt64 DataToSInt64(const UInt8* pData);
float DataToFloat(const UInt8* pData, BOOL* Invalid);
double DataToDouble(const UInt8* pData, BOOL* Invalid);
UInt32 UInt16ToData(UInt8* pData, UInt16 Value);
UInt32 SInt16ToData(UInt8* pData, SInt16 Value);
UInt32 UInt24ToData(UInt8* pData, UInt32 Value);
UInt32 UInt32ToData(UInt8* pData, UInt32 Value);
UInt32 SInt32ToData(UInt8* pData, SInt32 Value);
UInt32 UInt64ToData(UInt8* pData, UInt64 Value);
UInt32 SInt64ToData(UInt8* pData, SInt64 Value);
UInt32 FloatToData(UInt8* pData, float Value);
UInt32 DoubleToData(UInt8* pData, double Value);

UInt16 leDataToUInt16(const UInt8* pData);
SInt16 leDataToSInt16(const UInt8* pData);
UInt32 leDataToUInt32(const UInt8* pData);
SInt32 leDataToSInt32(const UInt8* pData);
UInt64 leDataToUInt64(const UInt8* pData);
double leDataToDouble(const UInt8* pData, BOOL* pInvalid);
float leDataToFloat(const UInt8* pData, BOOL* pInvalid);

BOOL IsValidFloat(UInt32 FloatCandidate);
BOOL IsValidDouble(UInt64 DoubleCandidate);

SInt32 RoundToInteger(register double Value);
SInt32 RoundToSInt32(double Value);
SInt16 RoundToSInt16(double Value);
SInt8  RoundToSInt8(double Value);
UInt32 RoundToUInt32(double Value);
UInt32 RoundToUInt24(double Value);
UInt16 RoundToUInt16(double Value);
UInt8  RoundToUInt8(double Value);

SInt32 RoundFloatToInteger(float Value);
SInt32 RoundFloatToSInt32(float Value);
SInt16 RoundFloatToSInt16(float Value);
SInt8  RoundFloatToSInt8(float Value);
UInt32 RoundFloatToUInt32(float Value);
UInt32 RoundFloatToUInt24(float Value);
UInt16 RoundFloatToUInt16(float Value);
UInt8  RoundFloatToUInt8(float Value);

UInt32 UInt16ToleData(UInt8* pData, UInt16 Value);
UInt32 SInt16ToleData(UInt8* pData, SInt16 Value);
UInt32 UInt32ToleData(UInt8* pData, UInt32 Value);
UInt32 SInt32ToleData(UInt8* pData, SInt32 Value);
UInt32 UInt64ToleData(UInt8* pData, UInt64 Value);
UInt32 FloatToleData(UInt8* pData, float Value);
UInt32 DoubleToleData(UInt8* pData, double Value);

UInt16 FloatToFloat16(float Data);
float Float16ToFloat(UInt16 Data);

float FloatValidate(UInt32 Data, BOOL *pInvalid);
double DoubleValidate(UInt64 Data, BOOL *pInvalid);

#ifdef __cplusplus
}
#endif

#endif // BYTE_ORDER_H
