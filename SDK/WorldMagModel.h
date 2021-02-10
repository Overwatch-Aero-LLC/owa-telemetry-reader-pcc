/*! \file
	\brief The World Magnetic Model
	The World Magnetic Model calculates the Earth's magnetic field at a given geographic location
	and at a known date.
	The magnetic field is modeled as the negative gradient of a potential expressed in terms of spherical harmonics. 
	The model is presented in detail at the National Geophysical Data Center's website: http://www.ngdc.noaa.gov/seg/WMM/DoDWMM.shtml
*/

#ifndef _WORLD_MAG_MODEL_H
#define _WORLD_MAG_MODEL_H

//! Array size for storing a 13x13 triangular matrix
#define NUM_ARRAY_WMM	(2+3+4+5+6+7+8+9+10+11+12+13)

#ifdef __cplusplus
extern "C" {
#endif

//! The geodetic position array includes latitude, longitude and altitude
enum
{
	GEO_PHI,
	GEO_LAMBDA,
	GEO_H,
	NUM_GEO
};

// GLOBAL FUNCTION PROTOTYPES

//! Initialize the magnetic model persistent data
void InitWMM(float Gnm[NUM_ARRAY_WMM], float Hnm[NUM_ARRAY_WMM], float Time);

//! Run the magnetic model
void RunWMM(float B[NUM_GEO], const float Gnm[NUM_ARRAY_WMM], const float Hnm[NUM_ARRAY_WMM], const double Pos[NUM_GEO]);

#ifdef __cplusplus
}
#endif

#endif