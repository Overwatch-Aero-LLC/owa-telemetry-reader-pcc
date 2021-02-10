/*! \file
	\brief Euler angle rotations.
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


#ifndef _ROTATIONS_H
#define _ROTATIONS_H

#include "Indices.h"
#include "Types.h"

#ifdef __cplusplus
extern "C" {
#endif

void EulerAngleLimits(double Euler[NAXIS]);
void CreateRotation(double Rotation[N3D][N3D], double Yaw, double Pitch, double Roll);
void CreateRotationEx(double Rotation[N3D][N3D], double Yaw, double Pitch, double Roll, UInt8 Order[N3D]);
void CreateYawRotation(double Rotation[N3D][N3D], double Yaw);
void CreatePitchRotation(double Rotation[N3D][N3D], double Pitch);
void CreateRollRotation(double Rotation[N3D][N3D], double Roll);
void InvertRotation(const double Source[N3D][N3D], double Invert[N3D][N3D]);
void ApplyRotation(const double S[N3D][N3D], const double Source[N3D], double Result[N3D]);

void EulerAngleLimitsf(float Euler[NAXIS]);
void CreateRotationf(float Rotation[N3D][N3D], float Yaw, float Pitch, float Roll);
void CreateRotationExf(float Rotation[N3D][N3D], float Yaw, float Pitch, float Roll, UInt8 Order[N3D]);
void CreateYawRotationf(float Rotation[N3D][N3D], float Yaw);
void CreatePitchRotationf(float Rotation[N3D][N3D], float Pitch);
void CreateRollRotationf(float Rotation[N3D][N3D], float Roll);
void InvertRotationf(const float Source[N3D][N3D], float Invert[N3D][N3D]);
void ApplyRotationf(const float S[N3D][N3D], const float Source[N3D], float Result[N3D]);

#ifdef __cplusplus
}
#endif
#endif


