/*! \file
    \brief Flight plan manipulation

    The flight plan module provides definitions and routines for working on
    flight plans.  Flight plans are made of linked lists of waypoints.  Each
    waypoint encodes latitude, longitude, altitude, and the index of the next
    waypoint to be used.  The avionics has storage for 1000 waypoints.  These
    waypoints can be used to make one flight plan which is 1000 waypoints long,
    or 1000 flight plans with one waypoint each, or any combination between.
    The key concept is that all flight plans must close.  This means that at
    some point the next waypoint must point to a waypoint which was previously
    in the list, or must point to itself and be an indefinite orbit.

    Since an entire flight plan might occupy up to 1000 waypoints, with each
    taking approximately 16 bytes of data, a whole flight plan cannot fit
    into a single autopilot packet.  Hence information about waypoints is
    often sent in compressed lists, called masks, which have 1 bit for each
    waypoint.  The flight plan module has functions for working with these
    masks.
*/

// FlightPlan.h
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

#ifndef _FLIGHTPLAN_H
#define _FLIGHTPLAN_H

#include "PiccoloDefinitions.h"

// Flight plan data occupies a lot of memory, so we have to compress the
//   data.  Remember that since we are using an array of points, and some
//   of the members are 4 bytes wide each element will start on a 4-byte
//   boundary, hence it is worthwhile to go through the effort to fit it
//   into a multi of four structure size


typedef FPPoint_t FPlan_t[NUM_FP_POINTS];

#ifdef __cplusplus
extern "C" {
#endif

//! Return TRUE if the flight plan point is valid
BOOL IsFPPointValid(const FPPoint_t *pFPPoint);

//! Get all the points in the closed flight plan
UInt16 GetPointsInPlan(const FPlan_t Plan, FPMask_t Mask, UInt16 Next);

//! Add up all the points in this fligth plan mask
UInt16 NumPointsInFPMask(const FPMask_t Mask);

//! Clear points from the flight plan
void ClearPointsFromPlan(FPlan_t Plan, FPMask_t Mask);

//! Check closure rules for a flight plan
BOOL CheckFlightPlanClosure(FPlan_t Plan, UInt16 Start);

//! Check closure rules for a flight plan and auto-close if needed
BOOL CheckFlightPlanClosureEx(FPlan_t Plan, UInt16 Start, float Radius, BOOL Right);

//! Find the first set bit in the flight plan mask
SInt32 FindFirstFPMaskBitSet(const FPMask_t Mask);

//! Find the next set bit in the flight plan mask
SInt32 FindNextFPMaskBitSet(const FPMask_t Mask, UInt32 i);

//! Find the first set bit in the flight plan mask
SInt32 FindFirstFPMaskBitSetAndClear(FPMask_t Mask);

//! Find the next set bit in the flight plan mask
SInt32 FindNextFPMaskBitSetAndClear(FPMask_t Mask, UInt32 i);

//! Set a bit in the flight plan mask
void SetFPMaskBit(FPMask_t Mask, UInt32 Point);

//! Clear a bit in the flight plan mask
void ClearFPMaskBit(FPMask_t Mask, UInt32 Point);

//! Get the status for a bit in the flight plan mask
BOOL GetFPMaskBit(const FPMask_t Mask, UInt32 Point);

//! Clear all bits in the flight plan mask
void ClearAllFPMaskBits(FPMask_t Mask);

//! Set all bits in the flight plan mask
void SetAllFPMaskBits(FPMask_t Mask);

//! Invert all bits in the flight plan mask
void InvertAllFPMaskBits(FPMask_t Mask);

//! Invert all bits in the flight plan mask
void InvertAllFPmaskBits_Ex(FPMask_t Dest, const FPMask_t Source);

//! OR all bits in two flight plan masks
void FPMaskOR(FPMask_t Mask1, const FPMask_t Mask2);

//! OR all bits in two flight plan masks
void FPMaskOR_Ex(FPMask_t Return, const FPMask_t Mask1, const FPMask_t Mask2);

//! AND all bits in two flight plan masks
void FPMaskAND(FPMask_t Mask1, const FPMask_t Mask2);

//! AND all bits in two flight plan masks
void FPMaskAND_Ex(FPMask_t Return, const FPMask_t Mask1, const FPMask_t Mask2);

//! Subtract two flight plan masks to find the difference
void FPMaskSubtract(FPMask_t Return, const FPMask_t Mask1, const FPMask_t Mask2);

//! Compare two flight plan masks
SInt32 CompareFPMask(const FPMask_t First, const FPMask_t Second);

//! Copy a flight plan mask
void CopyFPMask(const FPMask_t Source, FPMask_t Destination);

//! Determine if the mas is clear
BOOL IsFPMaskClear(const FPMask_t Mask);

#ifdef __cplusplus
}
#endif

#endif // _FLIGHTPLAN_H
