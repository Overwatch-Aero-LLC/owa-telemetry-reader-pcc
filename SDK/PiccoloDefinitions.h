/*! \file
    \brief Basic structure types used in the Piccolo avionics.
*/

// PiccoloDefinitions.h
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
#ifndef _PICCOLO_DEFINITIONS_H
#define _PICCOLO_DEFINITIONS_H

#include "Definitions.h"
#include "Indices.h"
#include "UnivData.h"


/*! Bandwidth mode */
typedef struct
{
    UInt8  Link;            //!< 0 for internal wireless link, 1-5 refer to the 5 external serial ports
    UInt32 LoRes : 1;       //!< True to enable low resolution packets
    UInt32 RegularMode : 7; //!< Mode of operation 0-6
    UInt32 NavEnable : 1;   //!< Navigation development packets enable
    UInt32 NavMode : 7;     //!< Navigation dev packets mode 0-6
    UInt32 ConEnable : 1;   //!< Controller development packets enable
    UInt32 ConMode : 7;     //!< Controller dev packets mode 0-6
}BandwidthMode_t;


/*! Failure modes and status used for flight termination */
typedef struct
{
    UInt8 IgnoreTimeoutsInPrelaunch     : 1;//!< If set the GPS and Comm timeouts are always refreshed in pre-launch
    UInt8 LowFuelCausesAutoLand         : 1;//!< If set then fuel below low fuel limit will trigger an autoland
    UInt8 CritFuelCausesAbort           : 1;//!< If set then fuel below crit fuel limit will trigger an abort
    UInt8 FTCausesAbort                 : 1;//!< If set then the vehicle will abort for flight termination (only makes sense for VTOL)
    UInt8 FTCausesParachute             : 1;//!< Set if flight termination causes parachute deploy
    UInt8 GPSandCommCauseFT             : 1;//!< If set GPS and Communications timeouts together cause flight termination
    UInt8 GPSCausesFT                   : 1;//!< If set GPS timeout causes flight termination
    UInt8 CommCausesFT                  : 1;//!< If set Communications timeout causes flight termination
    UInt8 FlightTimeandCommCauseLanding : 1;//!< Set if elapsed flight timer plus lost comms causes automatic landing
    UInt8 FTCausesAeroTerm              : 1;//!< Set to enable aerodynamic termination if flight termination
    UInt8 FTDropsDeadman                : 1;//!< Set to drop deadman if flight termination
    UInt8 BoundaryCausesFT              : 1;//!< If set boundary violations cause flight termination
    UInt8 HighAltitudeCausesFT          : 1;//!< If set high altitude limit violations cause flight termination
    UInt8 LowAltitudeCausesFT           : 1;//!< If set low altitude limit violations cause flight termination
    UInt8 EngineKillsDeadman            : 1;//!< If set drop the deadman if engine kill is asserted
    UInt8 FTKillsEngine                 : 1;//!< If set kill the engine if flight termination
}Failure_t;


typedef struct
{
    UInt16 AltitudeHigh : 1;    //!< Set if absolute altitude is too high
    UInt16 AltitudeLow : 1;     //!< Set if absolute altitude is too low
    UInt16 IASHigh : 1;         //!< Set if airspeed is too high
    UInt16 IASLow : 1;          //!< Set if airspeed is too low
    UInt16 RollHigh : 1;        //!< Set if roll angle is excessive
    UInt16 PitchHigh : 1;       //!< Set if pitch angle is excessive
    UInt16 RPMLow : 1;          //!< Set if RPM is too low
    UInt16 RPMHigh : 1;         //!< Set if RPM is too high
    UInt16 WindsHigh: 1;        //!< Set if winds are excessive
    UInt16 AGLLow : 1;          //!< Set if too close to the ground
    UInt16 Reserved2 : 2;
    UInt16 TerMaskedByState : 1;//!< Set if terrain alarm (computed at user) can be ignored because of autopilot state
    UInt16 AltMaskedByState : 1;//!< Set if altitude alarm can be ignored because of autopilot state
    UInt16 IASMaskedByState : 1;//!< Set if IAS alarm can be ignored because of autopilot state
    UInt16 RPMMaskedByState : 1;//!< Set if RPM alarm can be ignored because of autopilot state
}TelemLimits_t;


/*! Structure used to define limitations on the mission. */
typedef struct
{
    UInt32 CommTimeout;             //!< Maximum time without communications before lost communication is asserted (ms)
    UInt32 PilotTimeout;            //!< Maximum time without pilot in the loop commands before the autopilot is turned on (ms)
    UInt8  Reserved0;               //!< Reserved data space
    Failure_t Failure;              //!< Flight termination options
    UInt8 UseMaxAltForLostComm : 1; //!< Option to fly lost comm at max alt.
    UInt8 ReservedBits         : 7;
    UInt32 GPSTimeout;              //!< Maximum time without GPS before lost GPS is asserted (ms)
    UInt16 MinFuel;                 //!< Min fuel or battery voltage used to trigger autoland (kg/100 or Volt/100)
    SInt16 AltitudeMin;             //!< Minimum altitude in meters
    UInt16 CritFuel;                //!< Critical fuel or battery voltage used to trigger an abort (kg/100 or Volt/100)
    SInt16 AltitudeMax;             //!< Maximum altitude in meters
    UInt16 LostCommWaypoint;        //!< Waypoint to track when lost communications is asserted
    UInt16 AutolandWaypoint;        //!< Default waypoint for automatic landing
    UInt32 FlightTimeout;           //!< Flight timeout value in seconds
}MissionLimits_t;


/*! Control output indices for the manual and auto pilot */
enum ControlOutputs
{
    CO_ROLL,    //!< Output controlling the roll axis
    CO_PITCH,   //!< Output controlling the pitch axis
    CO_THRUST,  //!< Output controlling the energy
    CO_YAW,     //!< Output controlling the yaw axis
    CO_FLAP,    //!< Output controlling the energy
    NUM_CONTROL_AXIS    //!< Number of control outputs
};


/*! Calibration table relating control suface angle to pulse width */
typedef struct
{
    #define NUM_ACTUATOR_POINTS 10      //!< The number of points in a actuator calibration table
    float Angle[NUM_ACTUATOR_POINTS];   //!< radians, or for throttle 0.0 to 1.0
    UInt16 Width[NUM_ACTUATOR_POINTS];  //!< Pulse width in micro-seconds
}ActuatorTable_t;

#define NUM_ACTUATORS 16                //!< The number of actuators
#define NUM_SURFACES NUM_ACTUATORS      //!< Legacy name for number of actuators
#define NUM_SURFACE_POINTS NUM_ACTUATOR_POINTS //!< Legacy name for number of points in actuator table


/*! Doublet command structure */
typedef struct
{
    UInt8 PosAndNeg : 1;    //!< Set to make doublet go both positive and negative
    UInt8 APOff     : 1;    //!< Set to disable entire autopilot during doublet
    UInt8 Axis      : 6;    //!< Doublet axis enumerator
    UInt8 Duration;         //!< Duration of doublet in seconds
    UInt16 PulseTime;       //!< Duration of doublet deflection in milliseconds
    float Center;           //!< Center position of doublet axis
    float Delta;                //!< Delta from center of doublet aaxis
}DoubletCmd_t;


//! Frequency sweep command structure
typedef struct
{
    UInt8 OnAxisAPOff  : 1; //!< Enable autopilot on the swept axis
    UInt8 OffAxisAPOff : 1; //!< Enable autopilot on the off axes
    UInt8 Axis         : 6; //!< Swept axis enumerator
    UInt8 TotalTime;        //!< Total time for the frequency sweep record, in seconds
    UInt8 TrimTime;         //!< Trim hold time, in seconds
    float StartFrequency;   //!< Low frequency, rad/s
    float EndFrequency;     //!< High frequency, rad/s
    float StartAmplitude;   //!< Low frequency amplitude, fraction of full range
    float EndAmplitude;     //!< High frequency amplitude, fraction of full range
} FrequencySweepCmd_t;


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Definitions for flight planning

/*! Waypoint position */
typedef struct
{
    double Lat; //!< Latitude in radians
    double Lon; //!< Longitude in radians
    double Alt; //!< Altitude in meters
}Waypoint_t;

/*! Flight plan waypoint */
typedef struct
{
    union
    {
        Waypoint_t Point;   //!< Position data
        double Pos[NPOS];   //!< Alternate position data semantics
    };

    float OrbitRadius;      //!< Radius of the orbit in meters
    float Heading;          //!< Heading command for hover or flight plan leg in rad (VTOL only)
    UInt16 OrbitTime;       //!< Seconds spent in the orbit, 0 to orbit forever
    UInt16 OrbitBelowAlt : 1; //!< Set to orbit while below altitude
    UInt16 OrbitAboveAlt : 1; //!< Set to orbit while above altitude
    UInt16 Hover         : 1; //!< This point is used for hovering (VTOL only)
    UInt16 AGL           : 1; //!< Altitude is AGL
    UInt16 PointInward   : 1; //!< Point vehicle toward center of orbit (VTOL only)
    UInt16 HeadingSet    : 1; //!< Set heading command for hover or flight plan leg (VTOL only)
    UInt16 Reserved      : 2; //!< Reserved bits
    UInt16 Parachute     : 1; //!< True to deploy parachute
    UInt16 Drop          : 1; //!< True to deploy drop
    UInt16 Direction     : 1; //!< Orbit direction, 0 for left, 1 for right
    UInt16 WindFind      : 1; //!< Set to do wind finding on this segment
    UInt16 Land          : 1; //!< True if this is a touch-down point
    UInt16 Slope         : 1; //!< True to enable slope control; else constant altitude
    UInt16 Lights        : 1; //!< True to turn lights on when tracking this waypoint
    UInt16 PreTurn       : 1; //!< True to allow pre-turn for a waypoint
    UInt16 Next;    //!< Next waypoint in list
    UInt16 This;    //!< This waypoint in list
    UInt8 User;     //!< User defined byte of storage
}FPPoint_t;

/*! The maximum number of waypoints that the avionics can hold*/
#define NUM_FP_POINTS 1000

/*! The previous maximum number of waypoints, still applies to older APs */
#define NUM_LEGACY_FP_POINTS 100

/*! The previous maximum number of waypoints, still applies to older APs */
#define LEGACY_FP_MASK_BYTES (((NUM_LEGACY_FP_POINTS-1)/8)+1)

/*! Number of bytes required to hold 1 bit for each waypoint */
#define FP_MASK_BYTES (((NUM_FP_POINTS-1)/8)+1)

/*! Number of 32-bit words required to hold 1 bit for each waypoint */
#define FP_MASK_WORDS (((FP_MASK_BYTES-1)/4)+1)

/*! The flight plan mask is an array of bytes with one bit for each waypoint */
typedef UInt8 FPMask_t[(FP_MASK_WORDS*4)];

/*! The number of extra bits in a mask */
#define EXTRA_FP_MASK_BITS ((FP_MASK_BYTES*8)-NUM_FP_POINTS)

/*! The maximum number of points in a boundary*/
#define NUM_BOUNDARY 40

/*! Boundary structure */
typedef struct
{
    UInt8 N;                //!< Number of points in the boundary (up to 40)
    double lat;             //!< Anchor latitude in radians
    double lon;             //!< Anchor longitude in radians
    float North[NUM_BOUNDARY];  //!< Distance North of anchor in meters
    float East[NUM_BOUNDARY];   //!< Distance East of anchror in meters
}Boundary_t;


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Definitions for autopilot control and setup



/*! Autopilot loop states */
enum APLoopStates
{
    AP_OFF,         //!< Loop disabled
    AP_ON,          //!< Loop enabled
    AP_AUTO,        //!< Loop enabled, with automatic command
    NUM_LOOP_STATES //!< Number of loop states
};


typedef struct
{
	UInt16 Reserved : 7;
	UInt16 AirspeedSource : 1;	 //!< Source of the Airspeed for control (0 = Sensor, 1 = Controller)
    UInt16 IMUExternal : 1;      //!< Set if IMU is external
    UInt16 GPSExternal : 1;      //!< Set if GPS is external
    UInt16 AirDataExternal : 1;  //!< Set if air data is external
    UInt16 MagInstalled : 1;     //!< Set if magnetometer is installed
    UInt16 AGLInstalled : 1;     //!< Set if an AGL sensor is installed
    UInt16 AGLInRange : 1;       //!< Set if an AGL measurement is being provided
	UInt16 AltControlType : 2;   //!< Source of altitude data for control (0 = barometer, 1 = GPS)

}DataSource_t;


typedef struct
{
    UInt16 SolnType  : 8;	//!< Navigation / GPS solution type (Novatel solution type)
    UInt16 Reserved  : 8;
}NovatelStatus_t;


/*! Navigation diagnostic information */
typedef struct
{
    float GyroBias[NAXIS];      //!< Gyro bias coming from nav filter
    float AccelBias[NAXIS];     //!< Accelerometer bias coming from nav filter
    float MagBias[NAXIS];       //!< Magnetometer bias coming from nav filter

    // Position standard deviations (currently as reported by Novatel RTK GPS).
    float HortSigma_m;          //!< Horizontal position standard deviation, m
    float VertSigma_m;          //!< Vertical position standard deviation, m

    // Geoid undulation from GPS receiver
    float GeoidUndulation;      //!< meters of geoid undulation

    DataSource_t Source;        //!< Source of data
    NavStatus_t NavStatus;      //!< Navigation status information
    NovatelStatus_t NovStatus;  //!< Novatel status information

}NavDiagnostic_t;


/*! Generic actions bit fields */
typedef struct
{
    UInt8 Action0 : 1;      //!< action on/off
    UInt8 Action1 : 1;      //!< action on/off
    UInt8 Action2 : 1;      //!< action on/off
    UInt8 Action3 : 1;      //!< action on/off
    UInt8 Action4 : 1;      //!< action on/off
    UInt8 Action5 : 1;      //!< action on/off
    UInt8 Action6 : 1;      //!< action on/off
    UInt8 Action7 : 1;      //!< action on/off

} GenericActionBits_t;

/*! Raw actions bit fields */
typedef struct
{
    UInt16 Brakes: 1;           //!< Brakes on or off
    UInt16 LaunchAction1: 1;    //!< Launch action 1 on/off
    UInt16 LaunchAction2: 1;    //!< Launch action 2 on/off
    UInt16 Drop: 1;             //!< Drop action on/off
    UInt16 Lights: 1;           //!< Lights action on/off
    UInt16 EngineKill: 1;       //!< Engine kill action on/off
    UInt16 Parachute: 1;        //!< Parachute deploy on/off
    UInt16 AeroTermination: 1;  //!< Aero termination action on/off
	UInt16 LaunchAction3: 1;	//!< Launch action 3 on/off
	UInt16 LaunchAction4: 1;	//!< Launch action 4 on/off
    UInt16 CameraTrigger: 1;    //!< Trigger a camera based on distance flown on/off
    UInt16 Abort: 1;            //!< Flight termination causes abort on/off
	UInt16 ReservedActions : 4;	//!< Reserved

    GenericActionBits_t User;   //!< User actions on/off

	UInt8 MoreActions;			//!< Reserved for future expansion

} ActionBits_t;


/*! Autopilot status structure */
typedef struct
{
    #define MAX_CONTROLLER_LOOPS 8
    UInt16 LoopStatus0 : 2;     //!< 1st loop status
    UInt16 LoopStatus1 : 2;     //!< 2nd loop status
    UInt16 LoopStatus2 : 2;     //!< 3rd loop status
    UInt16 LoopStatus3 : 2;     //!< 4th loop status
    UInt16 LoopStatus4 : 2;     //!< 5th loop status
    UInt16 LoopStatus5 : 2;     //!< 6th loop status
    UInt16 LoopStatus6 : 2;     //!< 7th loop status
    UInt16 LoopStatus7 : 2;     //!< 8th loop status

    UInt16 LoopTelemMin0 : 1;   //!< 1st loop less than min limit
    UInt16 LoopTelemMax0 : 1;   //!< 1st loop more than max limit
    UInt16 LoopTelemMin1 : 1;   //!< 2nd loop less than min limit
    UInt16 LoopTelemMax1 : 1;   //!< 2nd loop more than max limit
    UInt16 LoopTelemMin2 : 1;   //!< 3rd loop less than min limit
    UInt16 LoopTelemMax2 : 1;   //!< 3rd loop more than max limit
    UInt16 LoopTelemMin3 : 1;   //!< 4th loop less than min limit
    UInt16 LoopTelemMax3 : 1;   //!< 4th loop more than max limit
    UInt16 LoopTelemMin4 : 1;   //!< 5th loop less than min limit
    UInt16 LoopTelemMax4 : 1;   //!< 5th loop more than max limit
    UInt16 LoopTelemMin5 : 1;   //!< 6th loop less than min limit
    UInt16 LoopTelemMax5 : 1;   //!< 6th loop more than max limit
    UInt16 LoopTelemMin6 : 1;   //!< 7th loop less than min limit
    UInt16 LoopTelemMax6 : 1;   //!< 7th loop more than max limit
    UInt16 LoopTelemMin7 : 1;   //!< 8th loop less than min limit
    UInt16 LoopTelemMax7 : 1;   //!< 8th loop more than max limit

    UInt16 GlobalOn      : 1;   //!< Set if autopilot is on, else manual
    UInt16 Reserved0     : 2;   //!< Reserved bits
    UInt16 NumLoops      : 4;   //!< Number of loops supported by the controller
    UInt16 APMode        : 5;   //!< AP mode
    UInt16 AssistMode    : 4;   //!< Assist mode: off, lateral, longitudinal+lateral, all, gimbal

    UInt16 WaypointFrom;        //!< Index of the from waypoint
    UInt16 WaypointTo;          //!< Index of the to waypoint
    UInt16 ETA;                 //!< ETA to waypoint or time remaining in orbit (seconds)

    UInt16 TrackerStatus : 2;   //!< Tracker loop status
    UInt16 Orbit         : 1;   //!< Set if the system is orbiting
    UInt16 BoundaryIsViolated:1;//!< Set if airspace boundary is violated
    UInt16 ParachutePendingRPM:1;//!< Parachute deployment pending RPM
    UInt16 FlightTermination: 1;//!< Set if flight termination is asserted
    UInt16 Deadman      : 1;    //!< Set if deadman output is on
    UInt16 GPSTimeout   : 1;    //!< Set if GPS timeout has elapsed
    UInt16 CommTimeout  : 1;    //!< Set if communications timeout has elpased
    UInt16 ManualEngineKill:1;  //!< Set if manual pilot has killed engine
    UInt16 AutoEngineKill:1;    //!< Set if auto pilot has killed engine
    UInt16 OperatorEngineKill:1;//!< Set if operator has killed engine
    UInt16 FlightTimerElapsed:1;//!< Set if flight timer has elapsed
    UInt16 AGLGood : 1;         //!< AGL measurement health as determined by the controller
    UInt16 Waypoint1AGL : 1;    //!< Set if altitude of waypoint 1 is AGL
    UInt16 Waypoint2AGL : 1;    //!< Set if altitude of waypoint 2 is AGL

    Waypoint_t Waypoint1;       //!< Location of the start of the track segment
    Waypoint_t Waypoint2;       //!< Location of the end of the track segment
    ActionBits_t ActionStates;  //!< Status of actions
    ActionBits_t ActionsDefined;//!< Are actions defined
    UInt8  ConType;             //!< Controller type enumeration
    UInt8  ConVersion;          //!< Controller version number

    UInt8  SurfaceTestIndex : 5;//!< Index of control surface test
    UInt8  MaxWaypt1000     : 1;//!< Set if maximum waypoint is 1000 versus 100
    UInt8  AutoFlareDisable : 1;//!< Set if auto flare has been disabled
    UInt8  ConfigLock    : 1;   //!< Set if configuration is locked

    UInt8  SurfaceTestTime;     //!< Seconds left in surface test, 0 if no test
    float OrbitRadius;          //!< Radius of the orbit, if Orbit bit is set [m]
    float LoopTargets[MAX_CONTROLLER_LOOPS];//!< Up to eight possible loop targets
    float MinLimits[MAX_CONTROLLER_LOOPS];  //!< Up to eight minimum loop limits
    float MaxLimits[MAX_CONTROLLER_LOOPS];  //!< Up to eight maximum loop limits
    UInt32 ElapsedTime;         //!< Time since prelaunch mode in seconds
    UInt32 FlightTime;          //!< Flight timeout value in seconds
    float PilotPercentage;      //!< Frequency of good pilot data from 0.0 to 100.0
    float PilotRate;            //!< Frequency of manual pilot updates in Hz.

} AutopilotStatus_t;


/*! Moving baseline landing status structure. */
typedef struct
{
    double TDPos[N3D];  //!< Projected touchdown position in LLA (rad,rad,m)
    float Baseline[N3D];//!< RTK baseline solution in meters NED
    float CrossTrack;   //!< Cross track value in meters
    float BelowTrack;   //!< Below track value in meters
    float Heading;      //!< Touchdown heading in radians
    float ETA;          //!< Estimated time to intercept
    float Error;        //!< Error of the baseline in meters
    UInt32 SystemTime;  //!< System time at which the baseline applies
    UInt16 Lag;         //!< Lag in milliseconds of the moving baseline data
    UInt8 State;        //!< Autopilot state
    UInt8 SolnType;     //!< Novatel solution type for the baseline data
    UInt8 NumSats;      //!< Number of sats in the novatel baselien solution
    UInt8 RightHand;    //!< True for a right hand pattern
    BOOL IsPPS;         //!< Set if the DGPS solution is PPS (SAASM)

}MovingBaseStatus_t;



/*! Manual assisted states*/
enum APManualAssistStates
{
    MA_STANDARD,    //!< No manual assistance
    MA_LATERAL,     //!< Roll control plus rudder (VTOL: Velocity)
    MA_LAT_LON,     //!< lateral plus pitch/vertical velocity
    MA_ALL,         //!< lateral, longitudinal, and throttle (VTOL: Attitude)
    GA_SENSOR_NAV,  //!< Navigation based upon sensor
    NUM_MA_STATES
};


/*! Autopilot loop command structure */
typedef struct
{
    UInt8 Loop;         //!< Loop identifier (one of APLoopIndices)
    UInt8 Control;      //!< Loop state (one of APLoopStates)
    UInt8 Reserved1;
    UInt8 Reserved2;
    float Value;        //!< Loop command (intepretation depends on loop identifier)
} AutopilotLoopCmd_t;



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Payload control definitions

/*! Communication protocols indices*/
enum CommunicationProtocols
{
    DEAD_PROTOCOL,                  //!< No valid protocol
    PICCOLO_COMM_WITH_FLOW_PROTOCOL,//!< Piccolo communications with flow control
    PICCOLO_COMM_PROTOCOL,          //!< Standard piccolo communications
    PAYLOAD_PROTOCOL,               //!< Payload stream protocol
    NMEA_OUT_PROTOCOL,              //!< NMEA output
    MA_TRANSPONDER_PROTOCOL,        //!< Microair transponder device protocol
    PAYLOAD2_PROTOCOL,              //!< Second payload stream protocol
    HMR_2300_PROTOCOL,              //!< Honeywell 2300 magnetometer
    HMR_3400_PROTOCOL,              //!< Honeywell 3400 magnetometer
    VCI_PROTOCOL,                   //!< VCI communications protocol
    GIMBAL_PROTOCOL,                //!< Camera gimbal protocol
    RESERVED_PROTOCOL,
    RTCA_OUT_PROTOCOL,              //!< Output of any received RTCA correction data
    NOVATEL_GPS_PROTOCOL,           //!< Novatel GPS protocol
    IRIDIUM_COMM_PROTOCOL,          //!< Iridium modem communications
    IRIDIUM_CONTROL_PROTOCOL,       //!< Iridium modem control
    LAT_ENG_AGL_PROTOCOL,           //!< Latitude engineering laser range finder
    SONY_CAMERA_PROTOCOL,           //!< Sony camera protocol
    RESERVED1_PROTOCOL,						
    PICCOLO_COMM_FAST_PROTOCOL,     //!< Fast by default no flow control
    PICCOLO_COMM_NO_LOST_PROTOCOL,  //!< Standard comms, but no refresh of lost comm timer
    MA_TRANSPONDER_NO_ALT_PROTOCOL, //!< Microair transponder device protocol when used with external altitude encoder
    RESERVED2_PROTOCOL,             //!< Raptor interface protocol
    VPS_PROTOCOL,                   //!< Video processor protocol
    OS_5000_PROTOCOL,               //!< OS 5000 magnetometer protocol
    LASER_ACE_AGL_PROTOCOL,         //!< Laser ace AGL sensor protocol
    RS800_AGL_PROTOCOL,             //!< Opti-Logic RS-800 AGL sensor protocol
    UBLOX_RELAY_PROTOCOL,           //!< uBlox GPS relay protocol
    ST_TRANSPONDER_PROTOCOL,        //!< Sagetech transponder protocol
    EDC_EMU_PROTOCOL,               //!< Protocol for engine data converter
    GSSIP_PROTOCOL,                 //!< GSSIP protocol as documented in GPS-ICD_153
	MHX_REFLASH_PROTOCOL,			//!< Protocol for re-flashing internal microhard radios
	PICCOLO_COMM_OUT_PROTOCOL,		//!< Output only piccolo communications
    VLS_PROTOCOL,                   //!< Vision landing system
    CLARITY_PROTOCOL,               //!< Sagetech Clarity ADS-B receiver protocol
    NUM_SERIAL_PROTOCOLS,           //!< Number of serial protocols
    PICCOLO_TTCSUCOMM_OVER_CAN_PROTOCOL = 200 //!< Piccolo TTCSU comms over CAN
};


/*! External serial port control */
typedef struct
{
    #define NUM_EXT_COM 8
    UInt32 baud[NUM_EXT_COM];
    UInt8  protocol[NUM_EXT_COM];
}ExternalSerial_t;


/*! External IO modes */
enum ExternalIOModes
{
    DEFAULT,                        //!< No output function
    DISCRETE_OUT,                   //!< Discrete output
    DISCRETE_IN,                    //!< Discrete input
    PULSE_PERIOD_OUT,               //!< Generate a rectangle wave with user specifiable period and high time
    INVERTED_PULSE_PERIOD_OUT,      //!< Generate a rectangle wave with user specifiable period and low time
    PULSE_IN,                       //!< Measure the time between rising and falling edges
    INVERTED_PULSE_IN,              //!< Measure the time between falling and rising edges
    PERIOD_IN,                      //!< Measure the time between rising edges
    INVERTED_PERIOD_IN,             //!< Measure the time between falling edges
    LIGHTS_DISCRETE,                //!< Used to assert/deassert an IO line to turn lights on or off
    PARACHUTE_SERVO,                //!< Used to actuate a servo to trigger parachute release
    PARACHUTE_DISCRETE,             //!< Used to assert/deassert an IO line to trigger parachute release
    DROP_SERVO,                     //!< Used to actuate a servo to trigger drop mechanism
    DROP_DISCRETE,                  //!< Used to assert/deassert an IO line to trigger drop mechanism
    RESERVED_MODE,                  //!< Not used anymore
    BRAKES_DISCRETE,                //!< Used to assert/deassert an IO line to engage brakes
    AGL_TRIGGER,                    //!< Used to trigger the sonic AGL sensor
    AGL_SENSE,                      //!< Used to sense the sonic AGL sensor
    AGL_BLANK_INHIBIT,              //!< Used to reduce blank inhibit time for the AGL sensor
    PARACHUTE_SERVO_ENGINE_ON,      //!< Actuate a servo to trigger parachute release, engine remains on
    PARACHUTE_DISCRETE_ENGINE_ON,   //!< Assert/deassert an IO line to trigger parachute release, engine remains on
    LAUNCH_INHIBIT,                 //!< Input line used to hold autopilot in prelaunch mode prior to launch
    IRIDIUM_POWER_CONTROL,          //!< Used to control power for Iridium (Piccolo II)
    LAUNCH_ACTION1_SERVO,           //!< Used to actuate a servo to trigger launch action 1
    LAUNCH_ACTION1_DISCRETE,        //!< Used to assert/deassert an IO line to trigger launch action 1
    LAUNCH_ACTION2_SERVO,           //!< Used to actuate a servo to trigger launch action 2
    LAUNCH_ACTION2_DISCRETE,        //!< Used to assert/deassert an IO line to trigger launch action 2
    LAUNCH_DETECT,                  //!< Input line used to signal autopilot to leave prelaunch mode.
    LIGHTS_SERVO,                   //!< Used to actuate a servo to turn lights on or off
    LEFT_RPM,                       //!< Used to measure left RPM
    RIGHT_RPM,                      //!< Used to measure right RPM
    PAN_SERVO,                      //!< Servo used for pan control (pan over tilt)
    TILT_SERVO,                     //!< Servo used for tilt control (pan over tilt)
    GIMBAL_DEPLOY_SERVO,            //!< Servo used for gimbal retracting or deploying
    INV_ROLL_SERVO,                 //!< Servo used to output inverse roll signal
    INV_PITCH_SERVO,                //!< Servo used to output inverse pitch signal
    CAMERA_SERVO,                   //!< Used to actuate a servo to trigger a camera
    CAMERA_DIST_TRIGGER_DISCRETE,   //!< Triggers a discrete line based on distance flown
    GROUND_CONTACT_SWITCH,          //!< Input for determining if we are on the ground
    JR_MANUAL_INPUT,                //!< Manual pilot input
    FUEL_FLOW_INPUT,                //!< Used to measure fuel flow
    WATER_SPEED_INPUT,              //!< Used to measure water speed
    FUEL_LEVEL_INPUT,               //!< Used to measure fuel level
    SERVO_USER_ACTION_0,            //!< User action for general servo control
    SERVO_USER_ACTION_1,            //!< User action for general servo control
    SERVO_USER_ACTION_2,            //!< User action for general servo control
    SERVO_USER_ACTION_3,            //!< User action for general servo control
    SERVO_USER_ACTION_4,            //!< User action for general servo control
    SERVO_USER_ACTION_5,            //!< User action for general servo control
    SERVO_USER_ACTION_6,            //!< User action for general servo control
    SERVO_USER_ACTION_7,            //!< User action for general servo control
    DISCRETE_USER_ACTION_0,         //!< User action for general discrete control
    DISCRETE_USER_ACTION_1,         //!< User action for general discrete control
    DISCRETE_USER_ACTION_2,         //!< User action for general discrete control
    DISCRETE_USER_ACTION_3,         //!< User action for general discrete control
    DISCRETE_USER_ACTION_4,         //!< User action for general discrete control
    DISCRETE_USER_ACTION_5,         //!< User action for general discrete control
    DISCRETE_USER_ACTION_6,         //!< User action for general discrete control
    DISCRETE_USER_ACTION_7,         //!< User action for general discrete control
    DISCRETE_USER_IN_0,             //!< User input for general discrete monitoring
    DISCRETE_USER_IN_1,             //!< User input for general discrete monitoring
    DISCRETE_USER_IN_2,             //!< User input for general discrete monitoring
    DISCRETE_USER_IN_3,             //!< User input for general discrete monitoring
    DISCRETE_USER_IN_4,             //!< User input for general discrete monitoring
    DISCRETE_USER_IN_5,             //!< User input for general discrete monitoring
    DISCRETE_USER_IN_6,             //!< User input for general discrete monitoring
    DISCRETE_USER_IN_7,             //!< User input for general discrete monitoring
    SONAR_PWM_INPUT,                //!< AGL sensing with PWM output from a sonar sensor with a max range of 5m
    MAXBOTICS_MB1043_PWM,           //!< AGL sensing with PWM output from MaxBotics ultrasonic AGL sensor
    LAUNCH_ACTION3_SERVO,           //!< Used to actuate a servo to trigger launch action 3
    LAUNCH_ACTION3_DISCRETE,        //!< Used to assert/deassert an IO line to trigger launch action 3
    LAUNCH_ACTION4_SERVO,           //!< Used to actuate a servo to trigger launch action 4
    LAUNCH_ACTION4_DISCRETE,        //!< Used to assert/deassert an IO line to trigger launch action 4
    EAGLE_TREE_RPM_IN,              //!< Eagle Tree RPM sensor input (electric brushless motor only)
    NUM_EXTERNAL_IO_MODES           //!< The number of external IO modes
};

//! The number of digital I/O pins
#define NUM_EXTERNAL_DIO_PINS 16

//! The total number of pins on the Piccolo, Piccolo Plus
#define NUM_PAYLOAD_IO_PINS 12

//! The number of additional pins (digital + analog) on the Piccolo II
#define NUM_PAYLOAD2_IO_PINS 8

//! The number of external ADC pins
#define NUM_EXTERNAL_ADC_PINS 4

/*! Names and ussage of IO pins.  Idx is the payload IO index and the
 *  servo index, Pin is the physical pin number on the external connector,
 *  and Sig is the signal from the processor.
Idx | Piccolo   | Piccolo II| Piccolo LT| Piccolo SL| Pic SL 565  Pic OEM
    | Pin Sig   | Pin Sig   | Pin Sig   | Pin Sig   | Pin Sig   | Pin Sig   |
0   | 44  PWM0  | 44a PWM0  | 19  TPUA0 | 21  TPUA0 | 21  TPUA0 | 56  TPUA0 |
1   | 43  PWM1  | 43a PWM1  | 16  TPUA1 | 22  TPUA1 | 22  TPUA1 | 54  TPUA1 |
2   | 42  PWM2  | 42a PWM2  | 13  TPUA2 | 23  TPUA2 | 23  TPUA2 | 52  TPUA2 |
3   | 41  PWM3  | 41a PWM3  | 37  TPUA3 | 24  TPUA3 | 24  TPUA3 | 50  TPUA3 |
4   | 40  PWM16 | 40a PWM16 | 34  TPUA4 | 25  TPUA4 | 25  TPUA4 | 48  TPUA4 |
5   | 39  TPUA0 | 39a TPUA0 | 22  TPUB3 | 47  TPUB3 | 47  TPUB3 | 46  TPUA5 |
6   | 38  TPUA1 | 38a TPUA1 |  4  TPUB2 | 30  TPUB2 | 30  TPUB2 | 32  TPUA6 |
7   | 37  TPUA2 | 37a TPUA2 | NA  NA    | 27  TPUB14| 27  TPUB14| 31  TPUA7 |
8   | 36  TPUA3 | 36a TPUA3 | NA  NA    | 44  TPUB15| 44  TPUB15| 30  TPUA8 |
9   | 35  TPUA4 | 35a TPUA4 | NA  NA    | 26  TPUA5 | 26  TPUA5 | 29  TPUA9 |
10  | 20  TPUB3 | 20a TPUB3 | NA  NA    | 16  PWM18 | 16  TPUA12| 28  TPUB9 |
11  |  5  TPUB2 |  5a TPUB2 | NA  NA    | 13  PWM19 | 13  TPUA13| 27  TPUB11|
12  | NA  NA    | 21b TPUB8 | NA  NA    | 45  DASM28| 45  TPUB0 | 26  TPUB0 |
13  | NA  NA    | 19b TPUB9 | NA  NA    | NA  TPUB10| NA  TPUB10| 25  TPUB1 |
14  | NA  NA    | 17b TPUB10| NA  NA    | NA  TPUB11| NA  TPUB11| NA  TPUC0 |
15  | NA  NA    | 15b TPUB11| NA  NA    | 28  MPIO6 | 28  TPUB1 | NA  TPUC1 |
16  | NA  NA    | 12b BAN0  | NA  NA    | 16  BAN48 | 16  AN68  | 30  AN68  |
17  | NA  NA    | 11b BAN1  | NA  NA    | 13  BAN49 | 13  AN69  | 29  AN69  |
18  | NA  NA    | 10b BAN2  | NA  NA    | 45  BAN50 | 45  AN70  | 28  AN66  |
19  | NA  NA    |  9b BAN3  | NA  NA    | 28  BAN51 | 28  AN71  | 27  AN67  |*/

/*! Legacy names and ussage of payload IO pins.  Previous Payload and Servo indices
 *  were different.
Idx | Piccolo       | Piccolo II    | Piccolo LT    | Piccolo SL    | Piccolo SL 565
    | Srv Pin Sig   | Srv Pin Sig   | Srv Pin Sig   | Srv Pin Sig   | Srv Pin Sig   |
0   | 5   39  TPUA0 | 5   39a TPUA0 | 0   19  TPUA0 | 0   21  TPUA0 | 0   21  TPUA0 |
1   | 6   38  TPUA1 | 6   38a TPUA1 | 1   16  TPUA1 | 1   22  TPUA1 | 1   22  TPUA1 |
2   | 7   37  TPUA2 | 7   37a TPUA2 | 2   13  TPUA2 | 2   23  TPUA2 | 2   23  TPUA2 |
3   | 8   36  TPUA3 | 8   36a TPUA3 | 3   37  TPUA3 | 3   24  TPUA3 | 3   24  TPUA3 |
4   | 9   35  TPUA4 | 9   35a TPUA4 | 4   34  TPUA4 | 4   25  TPUA4 | 4   25  TPUA4 |
5   | 11   5  TPUB2 | 11   5a TPUB2 | 6    4  TPUB2 | 6   30  TPUB2 | 6   30  TPUB2 |
6   | 10  20  TPUB3 | 10  20a TPUB3 | 5   22  TPUB3 | 5   47  TPUB3 | 5   47  TPUB3 |
7   | 0   44  PWM0  | 0   44a PWM0  | NA  NA  NA    | 7   27  TPUB14| 7   27  TPUB14|
8   | 1   43  PWM1  | 1   43a PWM1  | NA  NA  NA    | 8   44  TPUB15| 8   44  TPUB15|
9   | 2   42  PWM2  | 2   42a PWM2  | NA  NA  NA    | 9   26  TPUA5 | 9   26  TPUA5 |
10  | 3   41  PWM3  | 3   41a PWM3  | NA  NA  NA    | 10  16  PWM18 | 10  16  TPUA12|
11  | 4   40  PWM16 | 4   44a PWM16 | NA  NA  NA    | 11  13  PWM19 | 11  13  TPUA13|
12  | NA  NA  NA    | 12  21b TPUB8 | NA  NA  NA    | 12  45  DASM28| 12  45  TPUB0 |
13  | NA  NA  NA    | 13  19b TPUB9 | NA  NA  NA    | 13  NA  TPUB10| 13  NA  TPUB10|
14  | NA  NA  NA    | 14  17b TPUB10| NA  NA  NA    | 14  NA  TPUB11| 14  NA  TPUB11|
15  | NA  NA  NA    | 15  15b TPUB11| NA  NA  NA    | NA  28  MPIO6 | 15  28  TPUB1 |
16  | NA  NA  NA    | NA  12b BAN0  | NA  NA  NA    | NA  16  BAN48 | NA  16  AN68  |
17  | NA  NA  NA    | NA  11b BAN1  | NA  NA  NA    | NA  13  BAN49 | NA  13  AN69  |
18  | NA  NA  NA    | NA  10b BAN2  | NA  NA  NA    | NA  45  BAN50 | NA  45  AN70  |
19  | NA  NA  NA    | NA   9b BAN3  | NA  NA  NA    | NA  28  BAN51 | NA  28  AN71  | */

enum ExternalIOPinsAlt
{
    PAYLOAD0,
    PAYLOAD1,
    PAYLOAD2,
    PAYLOAD3,
    PAYLOAD4,
    PAYLOAD5,
    PAYLOAD6,
    PAYLOAD7,
    PAYLOAD8,
    PAYLOAD9,
    PAYLOAD10,
    PAYLOAD11,
    PAYLOAD12,
    PAYLOAD13,
    PAYLOAD14,
    PAYLOAD15,
    PAYLOAD16,
    PAYLOAD17,
    PAYLOAD18,
    PAYLOAD19,
    NUM_EXTERNAL_IO_PINS
};


//! External analog input modes
enum ExternalAnalogModes
{
    // DEFAULT mode zero index already declared in the digital IO modes
    RAW_INPUT = 1,              //!< Raw voltage measurement
    SCALED_INPUT,               //!< Input scaled by paramters
    OAT_INPUT,                  //!< Interpreted as an OAT sensor
    WEIGHT_ON_WHEEL_INPUT,      //!< Weight on wheel measurement
    FUEL_LEVEL_ANALOG_INPUT,    //!< Analog fuel level measurement
    SONAR_ANALOG_INPUT,         //!< Analog ranging with a sonar sensor
    MOTOR_BATT_VOLTAGE_INPUT,   //!< 0-5V input from a voltage divider circuit connected to motor battery
    NUM_EXTERNAL_ADC_MODES
};

/*! External IO control */
typedef struct
{
    UInt8 Mode;         //!< Mode of operation for this external IO line
    UInt8 Reserved1;
    UInt16 Reserved2;
    SInt32 Parameter1;  //!< First parameter for external IO control
    SInt32 Parameter2;  //!< Second parameter for external IO control
}ExternalIO_t;


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Definitions for windfinding

/*! Wind finding data */
typedef struct
{
    float WindSouth;    //!< Calculated wind from the South to the North [m/s]
    float WindWest;     //!< Calculated wind from the West to the East [m/s]
    float WindError;    //!< Final error in the wind solution [m/s]
    float Reserved;     //!< Reserved
    UInt8 Iterations;   //!< Number of iterations to converge
    UInt8 NumPoints;    //!< Number of points in solution
    UInt8 hours;        //!< Hour of day when solution completed
    UInt8 minutes;      //!< Minute of hour when solution completed
}Wind_t;


/*! Track data structure */
typedef struct
{
    double X; //!< Distance along-track [m]
    double Y; //!< Distance cross-track [m]
    float Vx; //!< Groundspeed along-track [m/s]
    float Vy; //!< Groundspeed cross-track [m/s]
    float ETA;  //!< Seconds to waypoint
    double Dist; //!< Distance to waypoint [m]
}TrackData_t;


/*! Structure to pass a simple landing pattern from the operator interface to the avionics */
typedef struct
{
    Waypoint_t WpT;     //!< the location of the touchdown waypoint
    float Heading;      //!< the final approach heading [rad]
    SInt8 Dir;          //!< the landing pattern direction (-1=clockwise or 1=counter-clockwise)
    UInt16 Start;       //!< the index of the starting waypoint.
    // (By definition, the starting waypoint will be the go-around)
} SimpleLandingPattern_t;


//! List of ECU vendors understood by Piccolo
enum ECUVendor
{
    AEROMECH_ECU,       //!< ECU from Aeromech
    HIRTH_ECU,          //!< ECU from Hirth
    CURRAWONG_ECU,      //!< ECU from Currawong
    ULTRA_ECU,          //!< ECU from Ultra electronics
    LATITUDE_EMU,       //!< EMU from latitude
    NUM_ECU_VENDORS
};


/*! Aggregate data type for holding bit fields describing ECU data.*/
typedef struct
{
    UInt32 CHT_A_included : 1;      //!< Set if cylinder head temperature for cylinder A is included
    UInt32 CHT_B_included : 1;      //!< Set if cylinder head temperature for cylinder B is included
    UInt32 EGT_A_included : 1;      //!< Set if exhaust temperature for cylinder A is included
    UInt32 EGT_B_included : 1;      //!< Set if exhaust temperature for cylinder B is included
    UInt32 IAT_included : 1;        //!< Set if inlet air temperature is included
    UInt32 ECU_volt_included : 1;   //!< Set if ECU input voltage is included
    UInt32 MAP_included : 1;        //!< Set if manifold air pressure is included
    UInt32 InjectTime_included : 1; //!< Set if injection time is included

    UInt32 InjectAngle_included : 1;//!< Set if injection angle is included
    UInt32 TPS_included : 1;        //!< Set if throttle position sensor is included
    UInt32 FuelPress_included : 1;  //!< Set if fuel pressure is included
    UInt32 AFRandComp_included : 1; //!< Set if AFR and AFR comp are included
    UInt32 Vendor : 4;              //!< ECU Vendor enumeration: Aeromech, Hirth, Currawong

    UInt32 EngineTime_included : 1; //!< Set if engine time is included
    UInt32 ErrorBits_A_included : 1;//!< Set if the first 16-bit error flag is included
    UInt32 ErrorBits_B_included : 1;//!< Set if the second 16-bit error flag is included
    UInt32 SerialNumber_included :1;//!< Set if the 16-bit CAN serial number is included
	UInt32 IsRight : 1;				//!< Set if this ECU is the "right" engine, else "left"
	UInt32 Reserved : 3;            //!< Reserved bit flags
	
    UInt32 Mode : 8;                //!< ECU mode byte

}ECUFlags_t;


/*! Aggregate data type for holding special information from an engine control computer.*/
typedef struct
{
    ECUFlags_t Flags;   //!< Flags for included data
    float CHT_A;        //!< Cylinder A head temperature in degrees Celcius
    float CHT_B;        //!< Cylinder B head temperature in degrees Celcius
    float EGT_A;        //!< Cylinder A exhaust gas temperature in degrees Celcius
    float EGT_B;        //!< Cylinder B exhaust gas temperature in degrees Celcius
    float IAT;          //!< Inlet air temperature in degrees Celcius
    float Volts;        //!< ECU input voltage
    float MAP;          //!< Manifold pressure in Pascals
    float InjectTime;   //!< Injection time in microseconds
    float InjectAngle;  //!< Injection angle in degrees
    float TPS;          //!< Throttle position sensor from 0.0 to 100.0
    float FuelPress;    //!< Fuel pressure in Pascals
    float AFR;          //!< Measured air fuel ratio
    float AFRcomp;      //!< AFR compensation value from closed loop oxygen sensor
    UInt32 EngineTime;  //!< Engine time in seconds
    UInt16 ErrorBits[2];//!< Bit fields for unit specific error flags
	UInt16 SerialNum;	//!< CAN bus serial number of this ECU, 0xFFFF if unknown
}ECU_t;


//! The ECU commands
typedef struct
{
    UInt16 ResetFuel        : 1;    //!< If set, reset the fuel burn estimate
    UInt16 PrimeEngine      : 1;    //!< If set, prime the engine
    UInt16 SetEngineMode    : 1;    //!< If set, update the engine mode
    UInt16 ClearErrorBitsA  : 1;    //!< If set, clear vendor-specific error bits A
    UInt16 ClearErrorBitsB  : 1;    //!< If set, clear vendor-specific error bits B
    UInt16 ReservedFlags    : 9;    //!< Reserved, set these to zero
    UInt16 Destination      : 2;    //!< Which ECU this command goes to: 0=both, 1=left only, 2=right only
    UInt16 EngineMode;              //!< Engine operating mode, only used if SetEngineMode bit is set
    UInt16 ErrorBitsA;              //!< Mask to clear error bits A, only used if ClearErrorBitsA bit is set
    UInt16 ErrorBitsB;              //!< Mask to clear error bits B, only used if ClearErrorBitsB bit is set
} ECUCommand_t;


/** @name Bit description for the navigation filter health bits */
//@{
#define NAV_HEALTH_READY    0x80    //!< Bit 0 set to indicate that the nav filter has initialized
#define NAV_HEALTH_MODE     0x40    //!< Bit 1 set to indicate that the nav filter is operating in normal mode
#define NAV_HEALTH_POS      0x20    //!< Bit 2 set to indicate that position variance is within bounds
#define NAV_HEALTH_VEL      0x10    //!< Bit 3 set to indicate that velocity variance is within bounds
#define NAV_HEALTH_ATT      0x08    //!< Bit 4 set to indicate that attitude variance is within bounds
#define NAV_HEALTH_BIAS     0x04    //!< Bit 5 set to indicate that gyro bias variance is within bounds
#define NAV_HEALTH_GRAV     0x02    //!< Bit 6 set to indicate that gravity variance is within bounds
#define NAV_HEALTH_WIND     0x01    //!< Bit 7 set to indicate that wind speed variance is within bounds
//@}

/*! Data type for communicating sensor errors, which was becoming unwieldy as individual fields*/
typedef struct
{
    float    TempRise;              //!< Board temperature rise from ambient in Celcius
  	float    TASFilterWindow;       //!< Window of acceptable TAS, as a fraction of TAS
	float    TASCutoff;             //!< Filter cutoff frequency for complimentary TAS
    float    AGLOffset;             //!< Error in AGL sensor to be subtracted from the reading, in meters
    float    PositionCorr;          //!< Static pressure position correction information
    UInt32   ReservedBits : 23;     //!< Reserved data bits
	UInt32	 ClearTASFault: 1;		//!< Set to clear an air data fault.
	UInt32	 AllowControllerTAS :1; //!< Set to allow the use of a controller estimated airspeed.
    UInt32   DisableAlign : 1;      //!< Set to disable consumption of Novatel Align heading data
    UInt32   OATfromStandardAtmo :1;//!< Set to compute OAT from standard atmosphere
    UInt32   MagOnlyInAHRS : 1;     //!< Set if you only want to use magnetometer in AHRS mode
    UInt32   DisableAlt : 1;		//!< Set to disable uBlox GPS altitude
    UInt32   DisableInternalMag : 1;//!< Set to disable the internal magnetometer
    UInt32   DisableVDown : 1;      //!< Set to disable uBlox Vdown output
    UInt32   DisableSBAS : 1;       //!< Set to disable SBAS	
}SensorErrorData_t;


//! Transponder status bits
typedef struct
{
    float Altitude;             //!< Transponder altitude in m   

    UInt16 SquawkCode;          //!< Squawk code
    
    UInt8 PowerUp       : 1;    //!< Power up state (0=OFF+squawk1200, 1=mode&squawk in NV memory)
    UInt8 Ident         : 1;    //!< Set if ident is active
    UInt8 Reserved      : 4;
    UInt8 Type          : 2;    //!< Transponder type
    
    UInt8 Error         : 1;    //!< Set if there's an error
    UInt8 AltSource     : 1;    //!< Reports altitude source (0=internal, 1=external)
    UInt8 GPSError      : 1;    //!< For mode S transponder, if it doesn't get GPS data
    UInt8 ICAOError     : 1;    //!< Set if ICAO address is not set
    UInt8 OverTemp      : 1;    //!< Set if over temperature condition
    UInt8 SquitterFail  : 1;    //!< Set if an extended squitter failure was detected
    UInt8 Mode          : 2;    //!< Mode of operation
    
} XPDRStatus_t;

//! Transponder types
enum XPDRTypes
{
    MODEC_XPDR_TYPE,        //!< Mode C transponder
    MODES_OUT_XPDR_TYPE,    //!< Mode S transponder with ADS-B Out
    MODES_BOTH_XPDR_TYPE,   //!< Mode S transponder with ADS-B In/Out
    NUM_XPDR_TYPES
};

//! Transponder modes
enum XPDRModes
{
    OFF_XPDR_MODE,          //!< Transponder RF stage is powered off
    STDBY_XPDR_MODE,        //!< Transponder in standby mode
    ON_XPDR_MODE,           //!< Transponder is on in mode A (no altitude)
    ALT_XPDR_MODE,          //!< Transponder is on in mode C (with altitude reporting)
    NUM_XPDR_MODES
};


//! Servo telemetry data
typedef struct
{
    UInt8 Channel   : 4;    //!< Channel number
    UInt8 Mode      : 4;    //!< Mode (0=invalid, 1=position, 2=pos w/rate, 3=rate, 4=current)
    UInt8 Volts;            //!< Voltage in 0.1 V units
    UInt16 AvgCurrent;      //!< Average current draw in mA
    UInt16 MaxCurrent;      //!< Maximum current draw in mA
    SInt16 Temperature;     //!< Temperature in 0.1 degC units
    UInt16 Position;        //!< Position in us   
    SInt16 PosError;        //!< Position error in us
    UInt16 ErrorBits;       //!< Error/alarm bits
    
} ServoTelemetry_t;


#endif //!_PICCOLO_DEFINITIONS_H

