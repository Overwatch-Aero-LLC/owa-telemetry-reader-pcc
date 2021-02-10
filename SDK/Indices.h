/*! \file
	\brief Enumerated indices for different types of arrays.

	This file is used to define names for the different axis in several three
	dimensional systems, including LLA (latitude, longitude, altitude), XYZ,
	Euler angles (roll, pitch, yaw), NED (North, East, Down) and SWU (South,
	West, Up).
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

#ifndef _INDICES_H
#define _INDICES_H

// Indices

/*!	Position indices for LLA (Latitude, Longitude, Altitude) */
enum PosIndices
{
	LAT,	//!< Latitude
	LON,	//!< Longitude
	ALT,	//!< Altitude
	NPOS	//!< Number of position indices
};

/*!	Cartesian coordinate indices for XYZ */
enum XYZIndices
{
	X_AXIS,	//!< X axis
	Y_AXIS,	//!< Y axis
	Z_AXIS,	//!< Z axis
	N3D	  	//!< Number of XYZ indices
};

/*!	Euler angle indices */
enum EulerIndices
{
	ROLL_AXIS,	//!< Roll axis
	PITCH_AXIS,	//!< Pitch axis
	YAW_AXIS,	//!< Yaw axis
	NAXIS		//!< Number of euler angles
};

/*!	Alternative Euler angle names */
enum AltEulerIndices
{
	PHI,	//!< Roll axis
	THETA,	//!< Pitch axis
	PSI	//!< Yaw axis
};

/*!	Direction indices for NED (North, East, Down) */
enum NEDIndices
{
	NORTH,	//!< North direction
	EAST,	//!< East direction
	DOWN,	//!< Down direction
	NDIR	//!< Number of directions
};

/*!	Inverted direction indices for SWU (South, West, Up) */
enum SWUIndices
{
	SOUTH,	//!< South direction
	WEST,	//!< West direction
	UP,		//!< Up direction
	NDIRINV	//!< Number of directions
};

/*!	Wind direction indices */
enum WindIndices
{
	WIND_SOUTH,	//!< Wind from the south to the north [m/s]
	WIND_WEST,	//!< Wind from the west to the east [m/s]
	WIND_ERROR,	//!< error in wind estimate [m/s]
	NWIND
};


/*! Gimbaled camera axis indices */
enum GimbalIndices
{
	GIMBAL_PAN,		//!< Gimbal pan axis
	GIMBAL_TILT,	//!< Gimbal tilt axis
	GIMBAL_ROLL,	//!< Gimbal roll axis
	NGIMBAL	//!< Number of gimbal axis
};

/*! Gimbaled inner/outer nodes */
enum GimbalInnerOuterNodes
{
	GIMBAL_INNER_PAN,	//!< Gimbal inner pan node
	GIMBAL_INNER_TILT,	//!< Gimbal inner tilt node
    GIMBAL_OUTER_PAN,	//!< Gimbal outer pan node
    GIMBAL_OUTER_TILT, 	//!< Gimbal outer tilt node
    NGIMBALINOUT        //!< Number of gimbal inner/outer axes nodes
};

//! Only the pan and tilt axis in this enumeration
#define NGIMBAL_SHORT 2

/*! Quaternion indices (e0, ex, ey, ez) */
enum QuatIndices
{
	E0,		//!< The "e0" or "q1" quaternion component
	EX,		//!< The "ex" or "q2" quaternion component
	EY,		//!< The "ey" or "q3" quaternion component
	EZ,		//!< The "ez" or "q4" quaternion component
	N_QUAT	//!< Size of quaterion vector is 4
};

/*! Direction cosine matrix elements (Cnn) */
enum DCMIndices
{
	C11,    //!< First row, first column
	C12,    //!< First row, second column
	C13,    //!< First row, third column
	C21,    //!< Second row, first column
	C22,    //!< Second row, second column
	C23,    //!< Second row, third column
	C31,    //!< Third row, first column
	C32,    //!< Third row, second column
	C33,    //!< Third row, third column
	N_DCM   //!< Size of a 3x3 matrix
};

#endif
