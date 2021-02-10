/*! \file
    \brief Basic structure types common to Cloud Cap software
*/

// Definitions.h
///////////////////////////////////
// This file provided by:        //
// Cloud Cap Technology, Inc.    //
// 2621 Wasco St.                //
// PO Box 1500                   //
// Hood River, OR, 97031         //
// +1-541-387-2120    (voice)    //
// +1-541-387-2030    (fax)      //
// http://www.cloudcaptech.com   //
///////////////////////////////////

#ifndef _DEFINITIONS_H
#define _DEFINITIONS_H

#include "Types.h"
#include "Indices.h"

//! Representation of a calendar date
typedef struct
{
    UInt16 year; //!< Integer year value.
    UInt8 month; //!< Integer month of year 1-12.
    UInt8 day;   //!< Integer day of month 1-31.
}Date_t;


//! Firmware version type
typedef struct
{
    UInt8 MajorVersion; //!< Major version
    UInt8 MinorVersion; //!< Minor version
    UInt8 SubVersion;   //!< Sub version
    UInt8 PatchNum;     //!< Patch number
    UInt8 Released;     //!< Released or test version flag
    UInt16 Revision;    //!< Version control revision
    Date_t Date;        //!< Date of release
    char Description[64];//!< Version description information
}Version_t;


//! Hardware board configuration data
typedef struct
{
    UInt16 BoardSN;         //!< Serial number of the board
    UInt16 ProductSN;       //!< Serial number of the end product
    UInt16 FeatureCode;     //!< Feature code
    UInt16 Rev;             //!< Revision
    UInt32 Config;          //!< Configuration flags
    Date_t Date;            //!< Date of manufacture
}Board_t;


/** @name Bits descriptions for the config word in the Board_t */
//@{
// Bits 0-5 are used for ground station setup
#define GS_SETUP_BITS                   0xFC000000
#define BAI_FULL_DUPLEX_GS_HW           BIT0    //!< Bit 0 set to indicate BAI ground station hardware
#define FREE_WAVE_FULL_DUPLEX_GS_HW     BIT1    //!< Bit 1 set to indicate freewave full duplex
#define TELEDESIGN_HALF_DUPLEX_GS_HW    BIT2    //!< Bit 2 set for teledesign half duplex
#define OI_PORT_EXTERNAL_RADIO_HW       BIT3    //!< Bit 3 set to indicate that OI comms are over external radio
#define MHX_EXTERNAL_HALF_DUPLEX_HW     BIT4    //!< Bit 4 set to indicate external microhard radio
#define AGIG_C2_HW                      BIT5    //!< Bit 5 set to indicate AGIG C2 link

// Bits 6-7 define external sensor systems
#define UBLOX_GPS_HW        BIT6    //!< Bit 6 set to indicate uBlox GPS
#define BUILT_IN_MAG_HW     BIT7    //!< Bit 7 set to indicate built in magnetometer

#define RADIO_CONFIG_BITS   0x00F00000  //!< Bits 8-11 define the type of radio system, see the enumeration in RadioTypes

#define SENSOR_CONFIG_BITS  0x000F0000  //!< Bits 12-15 define board, and IMU type
#define REV2_IMU_HW         BIT12   //!< Bit 12 set to indicate rev2 version of crista sensor head or LT IMU
#define PICCOLO_LT_IMU_HW   BIT13   //!< Bit 13 set to indicate PiccoloLT IMU hardware
#define CRISTA_IMU_HW       BIT14   //!< Bit 14 set to indicate crista IMU hardware
#define PICCOLO_SL565_IMU_HW (BIT13|BIT14)  //!< Bit 13 and 14 set to indicate 565SL IMU (must be 565 build)
#define REV3_IMU_HW         BIT15   //!< Bit 15 set to indicate rev3 version of crista sensor head or LT IMU

#define NO_AIR_DATA_HW      BIT16   //!< Bit 16 set to indicate no air data sensors installed

#define BOARD_ENUM_BITS     0x000000FF
#define LEGACY_BOARD        0       //!< Look at SENSOR_CONFIG_BITS to understand board type
#define CRISTA_BOARD        1       //!< Piccolo Plus or Piccolo II
#define LTSL_BOARD          2       //!< Piccolo LT or Piccolo SL
#define SL565_BOARD         3       //!< Piccolo SL565
#define NANO565_BOARD       4       //!< Piccolo Nano565

//@}


//! GPS sattelite status information
typedef struct
{
    UInt16 Used : 1;    //!< Set if this satellite used in the solution.
    UInt16 Reserved : 5;//!< Reserved bits
    UInt16 Geometry : 1;//!< Set if we have azimuth and elevation for this satellite
    UInt16 Azimuth : 9; //!< Azimuth angle of the satellite in degrees form 0 to 359.
    SInt8 Elevation;    //!< Elevation angle of the satellite in degrees.
    UInt8 SVID;         //!< Satellite ID. If 0 then no satellite.
    UInt8 L1CNo;        //!< Satellite carrier to noise ratio dbHz (0-255) for L1.
    UInt8 L2CNo;        //!< Satellite carrier to noise ratio dbHz (0-255) for L2.
    UInt8 L1TimeOfLock; //!< Time of continuous carrier lock in 10s seconds for L1.  0 if not available
    UInt8 L2TimeOfLock; //!< Time of continuous carrier lock in 10s seconds for L2.  0 if not available
}SatStatus_t;

/*! Types of GPS hardware */
enum
{
    NO_GPSTYPE,     //!< No GPS type information
    M12_GPSTYPE,    //!< M12 GPS type
    UBLOX_GPSTYPE,  //!< Internal uBlox GPS
    NOV_GPSTYPE,    //!< Novatel GPS
    EXT_GPSTYPE,    //!< Externally supplied GPS data without type information
    GSSIP_GPSTYPE,  //!< GSSIP interfaced GPS
    NUM_GPSTYPES

};

//! Limited GPS data
typedef struct
{
    double Lat; //!< Latitude in radians
    double Lon; //!< Longitude in radians
    double Alt; //!< Altitude in meters
    float GeoidUndulation;  //!< Geoid undulation = WGS-84 - MSL in meters
    float VNorth;       //!< North groundspeed velocity component
    float VEast;        //!< East groundspeed velocity component
    float VDown;        //!< Down groundpseed velocity component
    float PDOP;         //!< Position dilution of precision
    float seconds;      //!< floating seconds
    float SampleTime;  //!< Rate at which GPS data are being updated
    float VelocityLag; //!< Expected velocity lag of the GPS system
    UInt32 SystemTime;  //!< System time in milliseconds when this data apply
    UInt32 ITOW;        //!< GPS time of week in milliseconds
    UInt16 Week;        //!< GPS week number
    UInt16 Status;      //!< GPS status code
    UInt16 year;        //!< Integer year value
    UInt8 month;        //!< Integer month of year 1-12
    UInt8 day;          //!< Integer day of month 1-31
    UInt8 hours;        //!< Integer hour of day 0-23
    UInt8 minutes;      //!< Integer minutes of hour 0-59
    UInt8 VisibleSats;  //!< Number of visible sats
    UInt8 TrackedSats;  //!< Number of tracked sats
    UInt8 SolnType;     //!< Solution type information
    UInt8 HasVDown : 1; //!< Indicator if VDown is valid or not
    UInt8 HasAlt : 1;   //!< Indicator if altitude is valid or not
    UInt8 Reserved : 6;
    UInt8 UsedInINS : 1;//!< Set if this GPS is feeding the INS
    UInt8 GPSType : 7;  //!< GPS hardware type

}GPSData_t;

//! Limited Align baseline data
typedef struct
{
	float BaseLength;	//!< Baseline length in m
	float Heading;		//!< Heading angle in rad
	float Pitch;		//!< Pitch angle in rad
	float HeadingSigma;	//!< Heading standard deviation in rad
	float PitchSigma;	//!< Pitch standard deviation in rad
    UInt32 ITOW;        //!< GPS time of week in milliseconds
    UInt16 Week;        //!< GPS week number
    UInt8 VisibleSats;  //!< Number of visible sats
    UInt8 TrackedSats;  //!< Number of tracked sats
    UInt8 SolnType;     //!< Solution type information

} AlignData_t;

#define NUM_CAMERAS 4   //!< Maximum number of supported cameras. 
#define NUM_SATS 28     //!< Maximum number of simultaneous satellites
#define NUM_LASERS 4    //!< Maximum number of lasers possible
#define NUM_LENSES 4    //!< Maximum number of lenses possible

//! Number of temperatures and points related to each temperature for focus calibration tables
#define AF_CAL_TEMPS  5     //!< The number of temperatures over which cal table values are stored.
#define AF_CAL_POINTS 32    //!< The number of pointes stored for each temperature.

//! Complete GPS solution information
typedef struct
{
        // This section is exactly equivalent to GPSData_t, so that pointers
        //   to this structure can be interpeted as GPSData_t pointers
    double Latitude;    //!< Latitude in radians
    double Longitude;   //!< Longitude in radians
    double Altitude;    //!< Altitude in meters
    float GeoidUndulation;  //!< Geoid undulation = WGS-84 - MSL in meters
    float VNorth;       //!< North groundspeed velocity component
    float VEast;        //!< East groundspeed velocity component
    float VDown;        //!< Down groundpseed velocity component
    float PDOP;         //!< Position dilution of precision
    float seconds;      //!< floating seconds
    float SampleTime;   //!< Rate at which GPS data are being updated
    float VelocityLag;  //!< Expected velocity lag of the GPS system
    UInt32 SystemTime;  //!< System time in milliseconds when this data apply
    UInt32 ITOW;        //!< GPS time of week in milliseconds
    UInt16 Week;        //!< GPS week number
    UInt16 Status;      //!< GPS status code
    UInt16 year;        //!< Integer year value
    UInt8 month;        //!< Integer month of year 1-12
    UInt8 day;          //!< Integer day of month 1-31
    UInt8 hours;        //!< Integer hour of day 0-23
    UInt8 minutes;      //!< Integer minutes of hour 0-59
    UInt8 VisibleSats;  //!< Number of visible sats
    UInt8 TrackedSats;  //!< Number of tracked sats
    UInt8 SolnType;     //!< Solution type information
    UInt8 HasVDown : 1; //!< Indicator if VDown is valid or not
    UInt8 HasAlt : 1;   //!< Indicator if altitude is valid or not
    UInt8 Reserved : 6;
    UInt8 UsedInINS : 1;//!< Set if this GPS is feeding the INS
    UInt8 GPSType : 7;  //!< GPS hardware type

        // More time information
    UInt32 Leapmilliseconds;    //!< Number of leap milliseconds
    UInt32 VelITOW;     //!< GPS time of week in milliseconds for velocity
    SInt32 Timediff;    //!< ITOW - SystemTime (milliseconds)

        // ECEF information
    double PosECEF[N3D];//!< Position in ECEF frame
    float VelECEF[N3D]; //!< Velocity in ECEF frame

        // Alternate velocity format
    float Direction;    //!< Ground speed direction
    float Speed;        //!< Speed magnitude

        // Satellite data
    SatStatus_t Sat[NUM_SATS]; //!< Satellite specific information

        // More information about the solution
    float HortAccuracy;     //!< Horizontal accuracy estimate in meters
    float VertAccuracy;     //!< Vertical accuracy estimate in meters

}GPS_t;

/*! Aggregate data type for holding GPS information for users. */
typedef struct
{
    double Latitude;    //!< Latitude in radians
    double Longitude;   //!< Longitude in radians
    double Altitude;    //!< Altitude in meters, relative to WGS-84
    double PosECEF[N3D];//!< Position in ECEF frame
    float VNorth;       //!< North groundspeed velocity component
    float VEast;        //!< East groundspeed velocity component
    float VDown;        //!< Down groundspeed velocity component
    float VelECEF[N3D]; //!< Velocity in ECEF frame
    float Direction;    //!< Ground speed direction
    float Speed;        //!< Speed magnitude
    float PDOP;         //!< Position dilution of precision
    float seconds;      //!< floating seconds of minute 0-59.9
    float Accuracy;     //!< 3D accuracy estimate in units of meters
    UInt32 ITOW;        //!< GPS time of week in milliseconds
    UInt16 Week;        //!< GPS week number
    UInt16 Status;      //!< Receiver status
    UInt16 year;        //!< Integer year value
    UInt8 month;        //!< Integer month of year 1-12
    UInt8 day;          //!< Integer day of month 1-31
    UInt8 hours;        //!< Integer hour of day 0-23
    UInt8 minutes;      //!< Integer minutes of hour 0-59
    UInt8 VisibleSats;  //!< Number of visible sats
    UInt8 TrackedSats;  //!< Number of tracked sats
    UInt8 SolnType;     //!< Solution type information
    UInt8 UsedInINS : 1;//!< Set if this GPS is feeding the INS
    UInt8 GPSType : 7;  //!< GPS hardware type
    SatStatus_t Sat[NUM_SATS]; //!< Satellite specific information

}GPSUser_t;


/** @name Bit codes for the status word in the GPS_t */
//@{
// Bits 0-2 have the following interpretations
#define FIX3D       0xE000 //!< Bits 0,1,2 set for 3D fix
#define FIX2D       0xC000 //!< Bits 0,2 set for 2D fix
#define ACQUIRING   0x6000 //!< Bits 1,2 set when acquiring signal

// Bit 3 for bad serial interface
#define GPS_SERIAL_BAD 0x1000 //!< Bit 3 set when serial is bad

// Bit 4 for SBAS
#define SBAS_FIX    0x0800 //!< Set if fix includes satellite based augmentation

// Bit 5 for SAASM verification
#define SAASM_CHECK 0x0400 //!< Set if fix has been verified by SAASM receiver

// Bit 6 for SAASM source
#define GPS_SAASM   0x0200 //!< Set if fix comes from SAASM receiver

// Bits 8-12 have are individual states
#define COLD_START  0x0080 //!< Set for cold start
#define DGPS_FIX    0x0040 //!< Set if differentially corrected
#define POS_LOCK    0x0020 //!< Set if position lock mode enabled
#define NO_SATS     0x0008 //!< Set if insufficient satellites visible

// Bits 13-14 define the antenna sense states
#define ANT_OK      0x0000 //!< Bits 13,14 clear if antenna is OK
#define ANT_OC      0x0002 //!< Set if over current (short)
#define ANT_UC      0x0004 //!< Set if under current (open)
#define ANT_NV      0x0006 //!< Bits 13,14 set if no antenna power

// Bit 15 describes the GPS data source
#define GPS_EXTERNAL 0x0001 //!< Bit 15 is set for an external GPS (not SAASM)
//@}


/*! Position / Velocity type used for more in depth information of GPS solution.
    Novatel OEM6 Firmware Reference Manual Rev 3 page 303. */
enum
{
    NONE_PVTYPE = 0,                //!< No solution
    FIXEDPOS_PVTYPE,                //!< Fixed position solution
    FIXEDHEIGHT_PVTYPE,             //!< Fixed height solution (i.e. 2D)
    DOPPLER_VELOCITY_PVTYPE = 8,    //!< Velocity from doppler
    SINGLE_PVTYPE = 16,             //!< Single point solution
    PSRDIFF_PVTYPE,                 //!< Pseudo range differential solution
    SBAS_PVTYPE,                    //!< Satellite based augmentation system solution
    PROPAGATED_PVTYPE,              //!< Solution propagated by receiver
    OMNISTAR_VBS_PVTYPE,            //!< Omnistar VBS solution
    L1_FLOAT_PVTYPE =32,            //!< L1 only floating RTK solution
    IONOFREE_FLOAT_PVTYPE,          //!< No ionospheric corrections
    NARROW_FLOAT_PVTYPE,            //!< L1/L2 floating RTK solution
    L1_INTEGER_PVTYPE = 48,         //!< L1 integer (high performance) RTK solution
    NARROW_INTEGER_PVTYPE = 50,          //!< L1/L2 narrow lane integer RTK solution (the best!)
    OMNISTAR_HP_PVTYPE = 64,        //!< Ominstar HP solution
    OMNISTAR_XP_PVTYPE              //!< Ominstar XP solution
};


#ifndef NAV_STATUS_T
#define NAV_STATUS_T
#ifndef HI_TECH_C
//! GPS/INS Navigation filter status information.
typedef struct
{
    UInt16 PositionGood : 1;    //!< Set if GPS position measurement is good
    UInt16 VelocityGood : 1;    //!< Set if GPS velocity measurement is good
    UInt16 BaroGood     : 1;    //!< Set if baro altitude measurement is good
    UInt16 TASGood      : 1;    //!< Set if true airspeed measurement is good
    UInt16 AGLGood      : 1;    //!< Set if AGL measurement is good
    UInt16 MagGood      : 1;    //!< Set if magnetometer measurement is good
    UInt16 HeadingGood  : 1;    //!< Set if heading measurement is good
    UInt16 AttitudeGood : 1;    //!< Set if attitude estimates are good
    UInt16 GyroBiasGood : 1;    //!< Set if gyro bias estimates are good
    UInt16 AccBiasGood  : 1;    //!< Set if accelerometer bias estimates are good
    UInt16 MagBiasGood  : 1;    //!< Set if magnetic field bias estimates are good
    UInt16 WindGood     : 1;    //!< Set if wind speed estimates are good
    UInt16 Mode         : 4;    //!< Mode of the navigation filter
}NavStatus_t;
#endif // HI_TECH_C

//! The nav filter can be in one of these modes of operation
enum NavModes
{
    FILTER_INIT,                //!< The filter is initializing
    FILTER_BUFFERING,           //!< The filter is collecting IMU data
    FILTER_AHRS,                //!< The filter is in pure AHRS mode
    FILTER_BOUND_VEL,           //!< The filter is in bounded-velocity AHRS mode
    FILTER_NORMAL,              //!< The filter is in GPS/INS mode
    FILTER_ALIGN,               //!< The filter is in GPS/INS alignment mode
    NUM_FILTER_MODES
};
#endif


#ifndef HI_TECH_C
/*! System diagnostic information */
typedef struct
{
    float InputVoltage;                 //!< Main input voltage [V]
    float InputCurrent;                 //!< Main input current [A]
    float ServoVoltage;                 //!< Servo input voltage [V]
    float ServoCurrent;                 //!< Servo input current [A]
    float BoardTemp;                    //!< Board temperature [K]
    float RSSI;                         //!< Receive signal strength indicator from internal radio, dBm
    float VSWR;                         //!< VSWR from MHX radio, less than 1.0 is NA
    UInt32 FirstStageBIT : 1;           //!< Set if first stage voltage is bad
    UInt32 FiveDigitalBIT : 1;          //!< Set if five volt digital is bad
    UInt32 FiveAnalogBIT : 1;           //!< Set if five volt analog is bad
    UInt32 CPUVoltageBIT : 1;           //!< Set if CPU voltage is bad
    UInt32 GPSVoltageBIT :1;            //!< Set if GPS voltage is bad
    UInt32 RadioVoltageBIT : 1;         //!< Set if internal Radio voltage is bad
    UInt32 TempBIT : 1;                 //!< Set if board temperature is too hot or cold
    UInt32 TempDisagreeBIT : 1;         //!< Set if temperatures do not agree
    UInt32 NoInternalGPS : 1;           //!< Set if no internal GPS detected
    UInt32 PrimaryIMUBIT : 1;           //!< Set if primary IMU is bad
    UInt32 SecondaryIMUBIT : 1;         //!< Set if secondary IMU is bad
    UInt32 TertiaryIMUBIT : 1;          //!< Set if tertiary IMU is bad
    UInt32 PrimaryPdynamicBIT:1;        //!< Set if primary dynamic pressure sensor is bad
    UInt32 PrimaryPstaticBIT:1;         //!< Set if primary static pressure sensor is bad
    UInt32 PrimaryAirDataReversed:1;    //!< Set if primary air data lines are reversed
    UInt32 SecondaryPdynamicBIT:1;      //!< Set if secondary dynamic pressure sensor is bad
    UInt32 SecondaryPstaticBIT:1;       //!< Set if secondary static pressure sensor is bad
    UInt32 SecondaryAirDataReversed:1;  //!< Set if secondary air data lines are reversed
    UInt32 TertiaryPdynamicBIT:1;       //!< Set if tertiary dynamic pressure sensor is bad
    UInt32 TertiaryPstaticBIT:1;        //!< Set if tertiary static pressure sensor is bad
    UInt32 TertiaryAirDataReversed:1;   //!< Set if tertiary air data lines are reversed
    UInt32 Reserved : 11;               //!< Reserved data bits
    UInt8 CPULoad;                      //!< CPU Load from 0 to 100%
    UInt8 UplinkRatio;                  //!< Uplink ratio from 0 to 100%
    float VibrationX;                   //!< X acceleration vibration statistic
    float VibrationY;                   //!< Y acceleration vibration statistic
    float VibrationZ;                   //!< Z acceleration vibration statistic
    float VibrationP;                   //!< Roll rate vibration statistic
    float VibrationQ;                   //!< Pitch rate vibration statistic
    float VibrationR;                   //!< Yaw rate vibration statistic
}SystemDiagnostic_t;
#endif // HI_TECH_C


//! Types of radios understood by CCT hardware
enum RadioTypes
{
    MHX_910,        //!< Microhard MHX 910
    MHX_2400,       //!< Microhard MHX 2400
    MHX_920,        //!< Microhard MHX 920
    MHX_2420,       //!< Microhard MHX 2420
    MHX_425,        //!< Microhard MHX 425
    MHX_320,        //!< Microhard MHX 320
    MHX_1320,       //!< Microhard MHX 1320
    DDL_2450,       //!< Microhard Nano DDL 2.45 GHz
    MHX_1700,       //!< Microhard MHX 1700
    TS_4000,        //!< TS 4000 radio
    BAI_FW_1300,    //!< Free wave 1.3 GHz (one way mode)
    FW_2400_FD,     //!< Free wave full duplex 2.4 GHz
    EXT_MHX_X20,    //!< Externally connected microhard X20 radio
    AGIG,           //!< Proprietary radio
    PICO_900,       //!< Microhard P900
    PICO_2400,      //!< Microhard P2400
    NUM_RADIO_TYPES
};


//! Structure defining radio characteristics
typedef struct
{
    float  Power;       //!< The power setting to use, in units of watts
    float  Freq;        //!< The frequency in MHz
    UInt8  Channel;     //!< The communications channel to use
    UInt8  Type;        //!< The radio type
    UInt8  Speed;       //!< Speed flag for the radio, use zero for default speed
    UInt8  AESMode;     //!< 0, 1, 2, or 3 for AES off, 128, 192, or 256 bit
    char   Key[33];     //!< 32-byte encryption key, plue a NULL character
}RadioSettings_t;


//! Structure defining signal strength of a radio signal
typedef struct
{
    SInt8 Average;      //!< The average signal strength in dBm
    SInt8 Max;          //!< The maximum signal strength in dBm
}SignalStrength_t;

#define NUM_SIGNAL_CHANNELS 255
typedef struct
{
    SignalStrength_t Signal[NUM_SIGNAL_CHANNELS];   //!< Signal characteristics for 127 channel
    UInt8 NumSamples;                               //!< Reserved space
}SpectrumAnalyzer_t;

#define NUM_SPECTRUM_SWEEP 246  //!< Num measurements in an MHX-420 spectrum sweep
typedef struct
{
    float StartFreq;            //!< Starting frequency of the sweep in MHz
    float Spacing;              //!< Measurement spacing in MHz
    UInt16 Dwell;               //!< Dwell time for each measurement in milliseconds
    UInt8  Reserved;            //!< Reserved
    UInt8  NumMeasurements;     //!< Number of measurements made (up to NUM_SPECTRUM_SWEEP)
    SInt16 Strength[NUM_SPECTRUM_SWEEP];    // Signal strength of each measurement in dBm
}SpectrumSweep_t;

#endif //!_DEFINITIONS_H

