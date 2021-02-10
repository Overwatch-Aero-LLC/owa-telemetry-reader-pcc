/*! \file
    \brief Math utilities used in the robotics development kit
*/

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

#ifndef _MATH_UTIL_H
#define _MATH_UTIL_H

// Integer types
#include "Types.h"
#include "Indices.h"


// Function prototypes
#ifdef __cplusplus
extern "C" {
#endif

// Type-agnostic square and cube macros
#define SQR(x) ((x)*(x))
#define CUB(x) ((x)*(x)*(x))

// Square of a double
double sqr(double x);

// Cube of a double
double cub(double x);

// Sign of a double
double sign(double x);
float signf(float x);

// Min/Max limiting
double minimum(double x1, double x2);
double maximum(double x1, double x2);
float minimumf(float x1, float x2);
float maximumf(float x1, float x2);

//! return the middle of three numbers in an array
float MedianOfTriple(const float Value[3]);

//! return the middle of three numbers
float MedianOfTripleEx(float Value0, float Value1, float Value2);

#ifndef WIN32
#ifndef max
#define max(a,b) (((a)>(b))?(a):(b))
#endif
#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif
#endif

// Saturation limits
double lim(double x, double xmin, double xmax);
float limf(float x, float xmin, float xmax);
int limi(int x, int xmin, int xmax );

// Angle functions
double SubtractAngles(double First, double Second);
float SubtractAnglesf(float First, float Second);
double AddAngles(double First, double Second);
float AddAnglesf(float First, float Second);
double LimitAngle(double Angle);
float LimitAnglef(float Angle);
double LimitAngleEx(double Angle, double Wrap);
float LimitAngleExf(float Angle, float Wrap);

// Vector functions.  Each function has four possible types, 3D, 2D, double or float
double VectorLength3D(const double Vector[N3D]);
double VectorLengthSquared3D(const double Vector[N3D]);
double DotProduct3D(const double Vector1[N3D], const double Vector2[N3D]);
void CrossProduct3D(const double Vector1[N3D], const double Vector2[N3D], double Result[N3D]);
void SubtractVectors3D(const double Vector1[N3D], const double Vector2[N3D], double Result[N3D]);
void AddVectors3D(const double Vector1[N3D], const double Vector2[N3D], double Result[N3D]);
void MultiplyVectorByScaler3D(double Scaler, const double Vector[N3D], double Result[N3D]);
double UnitVector3D(double Vector[N3D]);
double ChangeVectorSize3D(double Size, double Vector[N3D]);
double CosInteriorAngle3D(const double Vector1[N3D], const double Vector2[N3D]);
double InteriorAngle3D(const double Vector1[N3D], const double Vector2[N3D]);
double VectorLength2D(const double Vector[2]);
double VectorLengthSquared2D(const double Vector[2]);
double DotProduct2D(const double Vector1[2], const double Vector2[2]);
void SubtractVectors2D(const double Vector1[2], const double Vector2[2], double Result[2]);
void AddVectors2D(const double Vector1[2], const double Vector2[2], double Result[2]);
void MultiplyVectorByScaler2D(double Scaler, const double Vector[2], double Result[2]);
double UnitVector2D(double Vector[2]);
double ChangeVectorSize2D(double Size, double Vector[2]);
double InteriorAngle2D(const double Vector1[2], const double Vector2[2]);
float VectorLength3Df(const float Vector[N3D]);
float VectorLengthSquared3Df(const float Vector[N3D]);
float DotProduct3Df(const float Vector1[N3D], const float Vector2[N3D]);
void CrossProduct3Df(const float Vector1[N3D], const float Vector2[N3D], float Result[N3D]);
void SubtractVectors3Df(const float Vector1[N3D], const float Vector2[N3D], float Result[N3D]);
void AddVectors3Df(const float Vector1[N3D], const float Vector2[N3D], float Result[N3D]);
void MultiplyVectorByScaler3Df(float Scaler, const float Vector[N3D], float Result[N3D]);
float UnitVector3Df(float Vector[N3D]);
float ChangeVectorSize3Df(float Size, float Vector[N3D]);
float CosInteriorAngle3Df(const float Vector1[N3D], const float Vector2[N3D]);
float InteriorAngle3Df(const float Vector1[N3D], const float Vector2[N3D]);
float VectorLength2Df(const float Vector[2]);
float VectorLengthSquared2Df(const float Vector[2]);
float DotProduct2Df(const float Vector1[2], const float Vector2[2]);
void SubtractVectors2Df(const float Vector1[2], const float Vector2[2], float Result[2]);
void AddVectors2Df(const float Vector1[2], const float Vector2[2], float Result[2]);
void MultiplyVectorByScaler2Df(float Scaler, const float Vector[2], float Result[2]);
float UnitVector2Df(float Vector[2]);
float ChangeVectorSize2Df(float Size, float Vector[2]);
float InteriorAngle2Df(const float Vector1[2], const float Vector2[2]);

// Look up table functions
double LookUpValue(double Ind, const double IndependentAxis[],
                   const double DependentAxis[], int NumPoints);
float LookUpValuef(float Ind, const float IndependentAxis[],
                   const float DependentAxis[], int NumPoints);
double Interpolate(double Ind, double IndHigh, double IndLow, double DepHigh, double DepLow);
float Interpolatef(float Ind, float IndHigh, float IndLow, float DepHigh, float DepLow);
SInt16 InterpolateSInt16(SInt16 Ind, SInt16 IndHigh, SInt16 IndLow, SInt16 DepHigh, SInt16 DepLow);

int BinarySearch(double key, const double Array[], int n);
int BinarySearchIncreasing(double key, const double Array[], int n);
int BinarySearchDecreasing(double key, const double Array[], int n);
int BinarySearchf(float key, const float Array[], int n);
int BinarySearchIncreasingf(float key, const float Array[], int n);
int BinarySearchDecreasingf(float key, const float Array[], int n);
int BinarySearchUInt16(UInt16 key, const UInt16 Array[], int n);
int BinarySearchIncreasingUInt16(UInt16 key, const UInt16 Array[], int n);

//! Search through an array of 16-bit numbers to find the index of the key
int BinarySearchDecreasingUInt16(UInt16 key, const UInt16 Array[], int n);

//! Convert a string of ASCII decimal digits to a 32-bit signed integer
int StringToSInt32Base10(const char* pString, int MaxCount, SInt32* pValue);

//! Convert a string of ASCII decimal digits to a 64-bit double.
int StringToDoubleBase10(const char* pString, int MaxCount, double* pValue);

//! Convert a string of ASCII decimal digits to a 32-bit float.
int StringToFloatBase10(const char* pString, int MaxCount, float* pValue);

//! Convert a string of ASCII hex digits to a 32-bit integer.
int StringToUInt32Base16(const char* pString, int MaxCount, UInt32* pValue);

//! Return an 8-bit value containing the value indicated by the two passed ASCII hex digits.
UInt8 ASCIItoHex1(char UpperNibble, char LowerNibble);

//! Convert an ASCII character interpreted as a hex nibble to the correct value in memory.
UInt8 NibbleValue(char Nibble);

//! Limit the magnitude of a 2-D vector while preserving its direction.
BOOL LimitVector2D(double* pX, double* pY, double Limit);

//! Limit the magnitude of a 2-D vector while preserving its direction. Single precision implementation.
BOOL LimitVector2Df(float* pX, float* pY, float Limit);

#ifdef __cplusplus
}
#endif

#endif

