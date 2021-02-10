/*! \file
    \brief Great circle utilties

    The great circle module provides utilities for doing great circle
    calculations and transferring between LLA and ECEF formats
*/

// GreatCircle.h
///////////////////////////////////
// This file provided by:        //
// Cloud Cap Technology, Inc.    //
// PO Box 1500                   //
// 2621 Wasco St.              //
// Hood River, OR, 97031         //
// +1-541-387-2120    (voice)    //
// +1-541-387-2030    (fax)      //
// http://www.cloudcaptech.com   //
///////////////////////////////////

#ifndef _GREATCIRCLE_H
#define _GREATCIRCLE_H

#include "Definitions.h"
#include "Indices.h"

#ifdef __cplusplus
extern "C" {
#endif

//! Convert LLA position and NED velocity to ECEF
void LLAandNEDtoECEF(const double PosLLA[NPOS], double PosECEF[NPOS], const float VelNED[N3D], float VelECEF[N3D]);

//! Convert ECEF position and velocity to LLA and NED
void ECEFtoLLAandNED(const double PosECEF[NPOS], double PosLLA[NPOS], const float VelECEF[N3D], float VelNED[N3D]);

//! Convert from LLA to ECEF position
void LLAtoECEF(const double LLA[NPOS], double ECEF[NPOS]);

//! Convert from ECEF to LLA position
void ECEFtoLLA(const double ECEF[NPOS], double LLA[NPOS]);

//! Convert from ECEF vector to NED vector
void ECEFtoNED(const double LLA[NPOS], const double ECEF[N3D], double NED[N3D]);

//! Convert from NED vector to ECEF vector
void NEDtoECEF(const double LLA[NPOS], const double NED[N3D], double ECEF[N3D]);

//! Convert from float NED vector to ECEF vector
void NEDtoECEFf(const double LLA[NPOS], const float Vel[N3D], float ECEF[N3D]);

//! Convert from float ECEF vector to NED vector
void ECEFtoNEDf(const double LLA[NPOS], const float ECEF[N3D], float Vel[N3D]);

//! Convert LLA position to ECEF position, using previously calculated trig
void LLAtoECEFEx(double Alt, double PosECEF[NPOS], double sinLat, double sinLon, double cosLat, double cosLon);

//! Convert ECEF position to LLA position, returning calculated trig
void ECEFtoLLAEx(const double PosECEF[NPOS], double PosLLA[NPOS], double* sinLat, double* cosLat);

//! Convert NED vector to ECEF vector, using previously calculated trig
void NEDtoECEFEx(const double NED[N3D], double ECEF[N3D], double sinLat, double sinLon, double cosLat, double cosLon);

//! Convert ECEF vector to NED vector, using previously calculated trig
void ECEFtoNEDEx(const double ECEF[N3D], double NED[N3D], double sinLat, double sinLon, double cosLat, double cosLon);

//! Convert float NED vector to float ECEF vector, using previously calculated trig
void NEDtoECEFExf(const float NED[N3D], float ECEF[N3D], double sinLat, double sinLon, double cosLat, double cosLon);

//! Convert float ECEF vector to float NED vector, using previously calculated trig
void ECEFtoNEDExf(const float ECEF[N3D], float NED[N3D], double sinLat, double sinLon, double cosLat, double cosLon);

//! Compute the heading (with respect to true North) of a vector given in ECEF
double ECEFHeading(const double ECEF[N3D], double sinLat, double sinLon, double cosLat, double cosLon);

//! Calcualte arc distance between two ECEF positions
double DistanceBetweenECEFCircle(const double Start[N3D], const double End[N3D]);

//! Calcualte stright distance between two ECEF positions
double DistanceBetweenECEF(const double Start[N3D], const double End[N3D]);

//! Calculate track relative position.
void TrackCoordinatePositionECEF(const double Pos[N3D], const double Start[N3D],
                                 const double End[N3D], double Track[N3D]);

//! Calculate track relative velocity.
void TrackCoordinateVelocityECEF(const float Vel[N3D], const double Start[N3D],
                                 const double End[N3D], float TrackVelocity[N3D]);

//! Calculate track relative position and velocity
void TrackCoordinateParametersECEF(const double Pos[N3D], const float Vel[N3D],
                                   const float Vel2[N3D], const double Start[N3D],
                                   const double End[N3D], double TrackPosition[N3D],
                                   float TrackVel1[N3D], float TrackVel2[N3D]);

//! Calculate track-relative position knowing the track-frame unit vectors
void TrackCoordinatePositionECEFEx(const double Pos[N3D], const double AlongTrackUnit[N3D],
        const double CrossTrackUnit[N3D], const double End[N3D], double TrackPosition[N3D]);

//! Calculate track-relative velocity knowing the track-frame unit vectors
void TrackCoordinateVelocityECEFEx(const float Vel[N3D], const double AlongTrackUnit[N3D],
                                const double CrossTrackUnit[N3D], float TrackVelocity[N3D]);

//! Calculate the track-frame unit vectors
void TrackRelativeVectors(const double Start[N3D], const double End[N3D],
                          double AlongTrackUnit[N3D], double CrossTrackUnit[N3D]);

//! Calculate the track relative position and velocity parameters including z
void TrackCoordinateParametersECEF3D(const double Pos[N3D], const float Vel[N3D],
                                   const double Start[N3D], const double End[N3D],
                                   double TrackPosition[N3D], float TrackVel[N3D]);

#ifdef __cplusplus
}
#endif

#endif // _GREATCIRCLE_H
