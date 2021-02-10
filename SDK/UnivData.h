/*! \file
    \brief Define universal constants.

    This file is used to define names for the universal constants.
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

#ifndef _UNIV_DATA_H
#define _UNIV_DATA_H


// Define universal constants

/** @name Ideal gas constant */
//@{
#define     IDEAL_GAS_R                     287
//@}

/** @name Ideal gas constant */
//@{
#define     CELSIUS_OFFSET                  273.15
#define     CELSIUS_OFFSETf                 273.15f
//@}

/** @name Density used for IAS calculations (nominally at sea level) */
//@{
#define     DENSITY_FOR_IAS                 1.225
#define     DENSITY_FOR_IASf                1.225f
//@}

//!{ Standard water density, used for ship buoyancy calculations
#define     DENSITY_WATER                   1000.0
#define     DENSITY_WATERf                  1000.0f
//!}

/** @name Earth constants */
//@{
#define     EQUATORIAL_RADIUS               6378137
#define     ANGULAR_VELOCITY                7.292115E-5
#define     GRAVITATIONAL_CONST             3.986005E+14
#define     SECOND_GRAVITATIONAL_CONST      1.08263E-3
#define     THIRD_GRAVITATIONAL_CONST      -2.53215306E-6
#define     FLATTENING                      298.257223563
#define     POLAR_RADIUS                    6356752.31424518
#define     FIRST_ECCENTRICITY              0.0818191908426
#define     SECOND_ECCENTRICITY             0.0820944379496
#define     GRAVITY_AT_EQUATOR              9.7803267714
#define     GRAVITY_FORMULA_CONST           0.00193185138639
#define     NORMAL_GRAVITY                  9.7976446561
#define     NORMAL_GRAVITYf                 9.7976446561f
#define     EARTH_RADIUS                    ((EQUATORIAL_RADIUS + POLAR_RADIUS)/2.0)
//@}

/** @name Angle constants and conversions */
//@{
#ifndef PI
#define     PI                              3.1415926535897932384626433832795
#endif
#define     PIf                             3.1415926535897932384626433832795f
#define     RAD_TO_DEG                      (180.0/PI)
#define     DEG_TO_RAD                      (PI/180.0)
#define     RAD_TO_DEGf                     (180.0f/PIf)
#define     DEG_TO_RADf                     (PIf/180.0f)
#define     RAD_TO_RPM                      (60.0/(2.0*PI))
#define     RPM_TO_RAD                      ((2.0*PI)/60.0)
//@}

/** @name Distance conversions */
//@{
#define     FT_TO_M                         0.3048
#define     M_TO_FT                         3.28084
#define     NM_TO_FT                        6076.11548556
//@}

/** @name Velocity conversions */
//@{
#define     MS_TO_KTS                       1.94253867
#define     MS_TO_KTSf                      1.94253867f
#define     KTS_TO_MS                       0.51479027
#define     KTS_TO_MSf                      0.51479027f
//@}

/** @name Other program constants */
//@{
#define     SMALL_ARRAY                     20
#define     MEDIUM_ARRAY                    200
#define     LARGE_ARRAY                     2000
//@}

#endif
