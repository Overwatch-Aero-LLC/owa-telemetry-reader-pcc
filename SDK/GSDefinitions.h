/*! \file
	\brief Basic structure types used in the ground station.
*/

// GSDefinitions.h
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
#ifndef _GS_DEFINITIONS_H
#define _GS_DEFINITIONS_H

#include "Definitions.h"

enum NovatelOutputFormat
{
    NOVATEL_DGPS_OUTPUT_RTCA,
    NOVATEL_DGPS_OUTPUT_X,
    NOVATEL_DGPS_OUTPUT_X1
};

/*! Parameters required for differential corrections */
typedef struct
{
	double BaseLat;		    //!< Differential base latitude [rad]
	double BaseLon;		    //!< Differential base longitude [rad]
	float  BaseHeight;	    //!< Differential base altitude [m]
	BOOL   EnableDGPS;	    //!< DGPS enable flag
	BOOL   MovingBase;	    //!< Moving base enabled flag
	float  Offset[N3D];	    //!< NED offset in meters from antenna to TD
	float  Heading;		    //!< Impact heading in radians
	UInt8  AbsoluteHeading;	//!< True for heading to be absolute angle
	UInt8  Align;           //!< Align enabled flag
	UInt8  UplinkRate;	    //!< 1, 2, or 4Hz
	UInt8  RightHand;	    //!< TRUE for right hand approach
    UInt8  OutputFormat;    //!< Differential corrections type: RTCA, Novatel-X, Novatel-X1.
}DGPSParameter_t;


/*! Basic telemetry from the ground station */
typedef struct
{
	double Lat;					//!< Latitude [rad]
	double Lon;					//!< Longitude [rad]
	float  GroundSpeed;			//!< Speed [m/s]
	float  Direction;			//!< Direction [rad]
	float  Height;				//!< GPS altitude [m]
	UInt16 GPSStatus;			//!< Receiver status
	UInt16 year;				//!< Integer year value
	UInt8 month;				//!< Integer month of year 1-12
	UInt8 day;					//!< Integer day of month 1-31
	UInt8 hours;				//!< Integer hour of day 0-23
	UInt8 minutes;				//!< Integer minutes of hour 0-59
	UInt8 seconds;				//!< Integer seconds of minute 0-59
	UInt8 VisibleSats;			//!< Number of visible sats
	UInt8 TrackedSats;			//!< Number of tracked sats
	SystemDiagnostic_t System;	//!< Ground station diagnostic data
}GSTelemetry_t;


/*! Flags bits that describe radio functionality in the group address packet*/
typedef struct
{
	UInt16 BAIFullDuplexGS : 1;			//!< Set to indicate BAI ground station hardware
	UInt16 FreeWaveFullDuplexGS : 1;	//!< Set to indicate free wave full duplex radio
	UInt16 TeledesignHalfDuplexGS : 1;	//!< Set for teledesign half duplex
	UInt16 OIPortExternalRadio : 1;		//!< Set to indicate that OI comms are over external radio
	UInt16 ExtMHXHalfDuplexGS : 1;		//!< Set to indicate external microhard hardware
	UInt16 AGIG_C2_GS : 1;				//!< Set to indicate AGIG secondary link
	UInt16 Reserved3 : 1;
	UInt16 Dynamic : 1;					//!< Set if dynamic enumeration is enabled
	UInt16 RadioType :4 ;				//!< Enumerated radio type
	UInt16 Reserved4 : 1;
	UInt16 Reserved5 : 1;
	UInt16 Reserved6 : 1;
	UInt16 AckRatioExcluded : 1;		//!< Set if ack ratio is not included
}GSNetworkFlags_t;


/*! Status data from a dual antenna moving reference RTK system */
typedef struct
{
	UInt32 ITOW;		//!< GPS time of week for this data [ms]
	float Baseline;		//!< Distance between antennas [m]
	float BaselineSigma;//!< Confidence of baseline distance [m]
	float NetRoll;		//!< Roll angle of the net in radians
	float NetHeading;	//!< Heading angle of the net in radians [0-2PI]
	float LeverArm[N3D];//!< North, East, Down lever arm from reference antenna to net center
	UInt16 Lag;			//!< Lag in milliseconds of the moving baseline data
	UInt8 NumSats;		//!< Number of sats in the novatel baselien solution
	UInt8 SolnType;		//!< Novatel solution type for the baseline data
}DualAntStatus_t;

#endif //!_GS_DEFINITIONS_H

