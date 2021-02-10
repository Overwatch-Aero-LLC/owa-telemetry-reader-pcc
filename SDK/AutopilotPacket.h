/*! \file
    \brief Autopilot stream packet utilities

    This module provides functions for encoding and decoding packets from the
    autopilot stream to and from network representation as documented in
    "Piccolo Communications".
*/
// AutopilotPacket.h
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

#ifndef _AUTOPILOT_PACKET_H
#define _AUTOPILOT_PACKET_H


#include "StreamPackets.h"
#include "PiccoloDefinitions.h"

#define AUTOPILOT_PKT_OVERHEAD SIMPLE_PKT_OVERHEAD

/*! Autopilot stream message types */
enum AutopilotPacketTypes
{
    // 0x00
    USER_WARNING,           //!< User warning
    CONFIG_UNLOCK,          //!< Configuration unlock
    BANDWIDTH_MODE,         //!< Bandwidth mode request
    FEATURE_CODE,           //!< Feature code information
    CONFIG_LOCK,            //!< Configuration lockout
    ACTUATOR_TABLE,         //!< Control surface calibration
    PAYLOAD_MASS,           //!< Set or request the payload mass value
    ELEVATION_DATA,         //!< Ground elevation information
    LEGACY_WAYPOINT,        //!< Deprecated waypoint list
    LEGACY_WAYPOINT_LIST,   //!< Deprecated list of waypoint indices
    TRACK,                  //!< Tracker command
    THROTTLE_MIN,           //!< Min throttle setting
    MISSION_SETTINGS,       //!< Mission settings
    ECU_STATUS,             //!< ECU status information
    SENSOR_ORIENTATION,     //!< Avionics installation orientation
    ALTIMETER_SETTING,      //!< Altimeter base pressure setting

    // 0x10
    SYSTEM_RESET,           //!< System reset command
    SYSTEM_VERSION,         //!< System hardware and firmware version
    RESET_REPORT,           //!< Reset report packet
    ACTUATOR_TEST,          //!< Control actuator test
    MISSION_LIMITS,         //!< Mission Limits
    BOUNDARY,               //!< Airspace boundary
    USER_ACTION,            //!< User action command
    CONFIG_UNLOCK_QUIET,    //!< Configuration unlock without respone
    AUTOPILOT_LOOP,         //!< Individual autopilot loop control
    AUTO_FLARE_DISABLE,     //!< Disable/Enable automatic flare
    DESCRIPTION,            //!< Description field in EEPROM
    USER_SPACE,             //!< User data space in EEPROM
    MANUAL_FLARE_COMMAND,   //!< Issue a manual flare command
    WAYPOINT_LIST_RANGE,    //!< Waypoint list range packet
    MAG_CALIBRATION,        //!< Magnetometer calibration
    ENGINE_KILL,            //!< Engine kill command

    // 0x20
    RADIO_TRANSMIT_DISABLE, //!< Transmit disable for radio systems
    RADIO_SETTINGS,         //!< Radio settings
    EXTERNAL_SERIAL_SETUP,  //!< External serial port setup
    EXTERNAL_IO_SETUP,      //!< External IO setup
    EXTERNAL_IO_SAMPLE,     //!< External IO input sample
    WAYPOINTS,              //!< Multiple waypoints
    EXTERNAL_ADC_SAMPLE,    //!< External analog input sample
    LAND_NOW,               //!< The command to autoland the aircraft using the default landing waypoint
    EDC_EMU_SENSORS,        //!< Engine data converter / Engine monitor unit
    ECU_COMMAND,            //!< ECU command packet
    CLEAR_PDYN_FAULT,        //!< Clear air data fault cmd.
    CREATE_SIMPLE_LANDING,  //!< Directs the navigator to create a simple landing pattern flightplan
    GPS_HEALTH,             //!< GPS health data
    AIR_DATA_ZERO,          //!< Zero just the air data sensors
    PRE_LAUNCH_MODE,        //!< Go to prelaunch mode
    NEW_MANUAL_ASSIST_MODE, //!< Manual assist mode command

    // 0x30
    LIGHTS_CMD,             //!< Lights command
    BRAKES_CMD,             //!< Brakes command
    PARACHUTE_CMD,          //!< Parachute command
    DROP_CMD,               //!< Drop command
    THROTTLE_TRIM,          //!< Set throttle trim
    DEVICE_DETAILS,         //!< Device detail information
    AVAILABLE_ID_36,        //!< Obsolete packet ID
    SERVO_TELEMETRY,        //!< Servo telemetry data
    TRANSPONDER_INSTALLATION,//!< Transponder Mode S installation message
    TRANSPONDER_STATUS,     //!< Transponder status information
    SYSTEM_STATUS,          //!< System status information
    TRANSPONDER_CONTROL,    //!< Transponder control
    TRANSPONDER_PREFLIGHT,  //!< Transponder Mode S preflight message
    RAW_VIBRATION_DATA,     //!< Raw vibration data, 1024 samples at 1khz, 6 sensros
    FREQ_SWEEP_CMD,         //!< Command for initiating a frequency sweep maneuver
    LINK_BRIDGE,            //!< Bridging data from link to link

    // 0x40
    DOUBLET_CMD,            //!< Command for initiating a doublet maneuver
    DOUBLET_AP_DATA,        //!< Returns autopilot data for a doublet
    DOUBLET_SENSOR_DATA,    //!< Returns high speed sensor data for a doublet
    CONTROLLER_DATA_REQUEST,//!< Request controller data
    CONTROLLER_DATA,        //!< Controller data
    TELEMETRY_HI_RES,       //!< High resolution telemetry
    TELEMETRY_LO_RES,       //!< Low resolution telemetry
    LEGACY_SYSTEM_STATUS_HI_RES,   //!< Legacy high resolution status
    LEGACY_SYSTEM_STATUS_LO_RES,   //!< Legacy low resolution status
    LAUNCH_NOW,             //!< Launch now command
    CONTROLLER_DATA_DEFAULT,//!< Default controller data
    SET_FUEL_LEVEL,         //!< Packet used to set the fuel level
    SENSOR_ERROR,           //!< Set the sensor errors
    CONTROLLER_ABORT,       //!< Issue an abort command to the controller
    MOVING_BASELINE_STATUS, //!< Status of the moving baseline landing system
    SET_FUEL_IFLOCKED,      //!< Packet used to set the fuel level if the system is locked

    // 0x50
    LATITUDE_RESERVED_0,    //!< Packet reserved for Latitude
    LATITUDE_RESERVED_1,    //!< Packet reserved for Latitude
    LATITUDE_RESERVED_2,    //!< Packet reserved for Latitude
    LATITUDE_RESERVED_3,    //!< Packet reserved for Latitude
    LATITUDE_RESERVED_4,    //!< Packet reserved for Latitude
    LATITUDE_RESERVED_5,    //!< Packet reserved for Latitude
    LATITUDE_RESERVED_6,    //!< Packet reserved for Latitude
    LATITUDE_RESERVED_7,    //!< Packet reserved for Latitude
    LATITUDE_RESERVED_8,    //!< Packet reserved for Latitude
    LATITUDE_RESERVED_9,    //!< Packet reserved for Latitude
    LATITUDE_RESERVED_A,    //!< Packet reserved for Latitude
    LATITUDE_RESERVED_B,    //!< Packet reserved for Latitude
    LATITUDE_RESERVED_C,    //!< Packet reserved for Latitude
    LATITUDE_RESERVED_D,    //!< Packet reserved for Latitude
    LATITUDE_RESERVED_E,    //!< Packet reserved for Latitude
    LATITUDE_RESERVED_F,    //!< Packet reserved for Latitude

    NUM_AP_PACKETS          //!< The number of autopilot stream packets
};


/*! Controller telemetry packet types, spanning from 220 to 239 */
enum ControllerPacketTypes
{
    CONTROLLER_PACKET_1 = 220,
    CONTROLLER_PACKET_2,
    CONTROLLER_PACKET_3,
    CONTROLLER_PACKET_4,
    CONTROLLER_PACKET_5,
    CONTROLLER_PACKET_6,
    CONTROLLER_PACKET_7,
    CONTROLLER_PACKET_8,
    CONTROLLER_PACKET_9,
    CONTROLLER_PACKET_10,
    CONTROLLER_PACKET_11,
    CONTROLLER_PACKET_12,
    CONTROLLER_PACKET_13,
    CONTROLLER_PACKET_14,
    CONTROLLER_PACKET_15,
    CONTROLLER_PACKET_16,
    CONTROLLER_PACKET_17,
    CONTROLLER_PACKET_18,
    CONTROLLER_PACKET_19,
    CONTROLLER_PACKET_20
};


/*! Components of data available in the telemetry message */
typedef struct
{
    UInt16 GPS : 1;         //!< GPS data included in packet
    UInt16 CalSensor : 1;   //!< Calculated data included in packet
    UInt16 RawSensor : 1;   //!< Raw sensor data included in packet
    UInt16 Magnetometer : 1;//!< Magnetometer data included in packet
    UInt16 AGL : 1;         //!< AGL data included in packet
    UInt16 Fuel : 1;        //!< Fuel data included in packet
    UInt16 AGLfromsensor: 1;//!< AGL data comes from installed sensor
    UInt16 AltUsed: 1;      //!< Altitude used by the controller
    UInt16 AlignHeading: 1; //!< Set if Novatel Align heading in packet
    UInt16 GPSResiduals : 1;//!< Set if position and velocity residuals in packet
    UInt16 Reserved: 1;
    UInt16 NumActuators: 5; //!< Number of actuators in the packet
}AvailableData_t;


/** @name Defines used with the waypoint list packet */
//@{
#define GET_WAYPOINT_LIST 0 //!< Request waypoints (if uplink packet)
#define CURRENT_WAYPOINTS 0 //!< Currently active list (if downlink packet)
#define DELETE_WAYPOINTS  1 //!< Delete waypoints (if uplink packet)
#define REQUEST_WAYPOINTS 2 //!< Request waypoints (if uplink packet)
#define WAYPOINT_BLOCK    3 //!< Prepare to receive waypoints (both ways)
#define WAYPOINT_ACK      4 //!< Acknowledge receive of waypoints (both ways)
//@}

#define MAX_WAYPOINTS_IN_PACKET 14  //!< Maximum number of waypoints you can fit in one packet
#define MAX_WAYPOINTS_IN_FLIGHT (3*MAX_WAYPOINTS_IN_PACKET) //!< Maximum amount of unacknowledged waypoint traffic

//! Predefined user warning types
enum UserWarningTypes
{
    HARDWARE_WARNING = 250,     //!< Warn on CPU and board problems
    SENSOR_WARNING,             //!< Warn on abnormal sensor data
    NAV_WARNING,                //!< Warn on nav filter and sensor fusion issues
    COMM_WARNING,               //!< Warn on communications and radio problems
    MISSION_WARNING,            //!< Warn on mission limits violations
    CONTROLLER_WARNING          //!< Warn on controller issues
};


// Prototypes


#ifdef __cplusplus
extern "C" {
#endif

//! Extract the controller data from an autopilot packet
void* DecodeControllerDataPacket(AutopilotPkt_t* pPkt, UInt8* pController, UInt8* pVersion, UInt8* pCategory, UInt8* pDataID, UInt8* pNumData);

//! Form a controller data packet which contains 32-bit data words
void FormControllerDataPacket(AutopilotPkt_t* pPkt, UInt8 Controller, UInt8 Version, UInt8 Category, UInt8 DataID, UInt8 NumData, const void* pData);

//! Extract the controller data request from an autopilot packet.
BOOL DecodeControllerDataRequestPacket(const AutopilotPkt_t* pPkt, UInt8* pController, UInt8* pVersion, UInt8* pCategory, UInt8* pDataID, UInt8* pNumData);

//! Extract the data from a transponder control packet
BOOL DecodeTransponderControlPacket(const AutopilotPkt_t* pPkt, UInt16* pCode, BOOL* pIdent, UInt8* pMode);

//! Form a tranponder control packet
void FormTransponderControlPacket(AutopilotPkt_t* pPkt, UInt16 Code, BOOL Ident, UInt8 Mode);

//! Form the transponder preflight packet
void FormTransponderPreflightPacket(AutopilotPkt_t* pPkt, const char FlightID[9]);

//! Decode the transponder preflight packet
BOOL DecodeTransponderPreflightPacket(const AutopilotPkt_t* pPkt, char FlightID[9]);

//! Form the transponder installation packet
void FormTransponderInstallationPacket(AutopilotPkt_t* pPkt, UInt32 ICAOAddress, const char Registration[9]);

//! Decode the transponder installation packet
BOOL DecodeTransponderInstallationPacket(const AutopilotPkt_t* pPkt, UInt32* pICAOAddress, char Registration[9]);

//! Form a radio settings packet
void FormRadioSettingsPacket(AutopilotPkt_t* pPkt, const RadioSettings_t* pSettings);

//! Extract the data from a radio settings packet
BOOL DecodeRadioSettingsPacket(const AutopilotPkt_t* pPkt, RadioSettings_t* pSettings);

//! Form an autopilot loop control packet
void FormAutopilotLoopControlPacket(AutopilotPkt_t* pPkt, const AutopilotLoopCmd_t* pCmd);

//! Form an autopilot loop control packet
void FormAutopilotLoopControlPacketEx(AutopilotPkt_t* pPkt, const AutopilotLoopCmd_t* pCmd, BOOL UpdateEEPROM);

//! Decode an autopilot loop control packet
BOOL DecodeAutopilotLoopControlPacket(const AutopilotPkt_t* pPkt, AutopilotLoopCmd_t* pCmd);

//! Decode an autopilot loop control packet
BOOL DecodeAutopilotLoopControlPacketEx(const AutopilotPkt_t* pPkt, AutopilotLoopCmd_t* pCmd, BOOL* pUpdateEEPROM);

//! Form a autopilot actuator table calibration packet
void FormActuatorTablePacket(AutopilotPkt_t* pPkt, const ActuatorTable_t* pTable, UInt8 Actuator, UInt8 Type, UInt8 Virtual, UInt8 Fast);

//! Decode a autopilot actator table calibration packet
BOOL DecodeActuatorTablePacket(const AutopilotPkt_t* pPkt, ActuatorTable_t* pTable, UInt8* pActuator, UInt8* pType, UInt8* pVirtual, UInt8* pFast);

//! Form an autopilot orientation packet
void FormOrientationPacket(AutopilotPkt_t* pPkt, const float Euler[NAXIS], const float IMUtoAnt[N3D], const float IMUtoAnt2[N3D]);

//! Decode an autopilot orientation packet
BOOL DecodeOrientationPacket(const AutopilotPkt_t* pPkt, float Euler[NAXIS], float IMUtoAnt[N3D], float IMUtoAnt2[N3D]);

//! Decode a autopilot waypoints packet.
UInt16 DecodeWaypointsPacket(const AutopilotPkt_t* pPkt, FPPoint_t Points[MAX_WAYPOINTS_IN_PACKET], BOOL* pAck);

//! Form a autopilot waypoints packet including the packet header and crc.
void FormWaypointsPacket(AutopilotPkt_t* pPkt, const FPPoint_t Points[MAX_WAYPOINTS_IN_PACKET], UInt16 Num, BOOL Ack);

//! Extract the data from a waypoint list packet
UInt8 DecodeWaypointListPacket(const AutopilotPkt_t* pPkt, FPMask_t Mask);

//! Form a waypoint list packet
void FormWaypointListPacket(AutopilotPkt_t* pPkt, const FPMask_t Mask, UInt8 Flag);

//! Form a autopilot waypoint list packet with a contiguous range of asserted
void FormWaypointListRangePacket(AutopilotPkt_t* pPkt, UInt16 Start, UInt16 End, UInt8 Flag);

//! Form a legacy autopilot waypoint packet (before 2.2.x)*/
void FormLegacyWaypointPacket(AutopilotPkt_t* pPkt, const FPPoint_t* pPoint, UInt8 Index);

//! Decode a autopilot legacy waypoint packet (prior to version 2.2.x.
UInt16 DecodeLegacyWaypointPacket(const AutopilotPkt_t* pPkt, FPPoint_t* pPoint);

//! Decode a legacy autopilot waypoint list packet (before software 2.2.x).
UInt8 DecodeLegacyWaypointListPacket(const AutopilotPkt_t* pPkt, FPMask_t Mask);

//! Form a legacy autopilot waypoint list packet (before software 2.2.x)
void FormLegacyWaypointListPacket(AutopilotPkt_t* pPkt, const FPMask_t Mask, UInt8 Flag);

//! Decode mission limits packet
BOOL DecodeMissionLimitsPacket(const AutopilotPkt_t* pPkt, MissionLimits_t* pMission);

//! Form a mission limits packet
void FormMissionLimitsPacket(AutopilotPkt_t* pPkt, const  MissionLimits_t* pMission);

//! Form a magnetometer calibration packet
void FormMagCalPacket(AutopilotPkt_t* pPkt, const float HardIronErr[N3D], const float ScaleFactErr[N3D]);

//! Decode a magnetometer calibration packet
BOOL DecodeMagCalPacket(const AutopilotPkt_t* pPkt, float HardIronErr[N3D], float ScaleFactErr[N3D]);

//! Form a serial port setup packet
void FormExternalSerialSetupPacket(AutopilotPkt_t* pPkt, const ExternalSerial_t* pSerial);

//! Decode a serial port setup packet
BOOL DecodeExternalSerialSetupPacket(const AutopilotPkt_t* pPkt, ExternalSerial_t* pSerial);

//! Create an external IO setup packet
void FormExternalIOSetupPacket(AutopilotPkt_t* pPkt, const ExternalIO_t* pExternalIO, UInt8 pin, UInt32 SampleTime);

//! Create an external IO setup packet
void FormExternalIOSetupPacketEx(AutopilotPkt_t* pPkt, const ExternalIO_t* pExternalIO, UInt8 pin, UInt32 SampleTime, BOOL UpdateEEPROM);

//! Extract the data from an external IO setup packet
UInt8 DecodeExternalIOSetupPacket(const AutopilotPkt_t* pPkt, ExternalIO_t* pExternalIO, UInt32* pSampleTime);

//! Extract the data from an external IO setup packet
UInt8 DecodeExternalIOSetupPacketEx(const AutopilotPkt_t* pPkt, ExternalIO_t* pExternalIO, UInt32* pSampleTime, BOOL* pUpdateEEPROM);

//! Create a simple landing packet
void FormSimpleLandingPacket(AutopilotPkt_t* pPkt, const SimpleLandingPattern_t* pLand);

//! Extract the data from a simple landing packet
BOOL DecodeSimpleLandingPacket(const AutopilotPkt_t* pPkt, SimpleLandingPattern_t* pLand);

//! Create an altimeter setting packet
void FormAltimeterSettingPacket(AutopilotPkt_t* pPkt, UInt32 Altimeter, BOOL ControlToGPS);

//! Extract the data from a altimeter setting packet
BOOL DecodeAltimeterSettingPacket(const AutopilotPkt_t* pPkt, UInt32* pAltimeter, BOOL* pControlToGPS);

//! Build a bandwidth mode packet
void FormBandwidthModePacket(AutopilotPkt_t* pPkt, BandwidthMode_t Mode);

//! Decode a bandwidth mode packet
BOOL DecodeBandwidthModePacket(const AutopilotPkt_t* pPkt, BandwidthMode_t* pMode);

//! Form the payload mass packet including the packet header and crc.
void FormPayloadMassPacket(AutopilotPkt_t* pPkt, float PayloadMass);

//! Decode the payload mass packet
BOOL DecodePayloadMassPacket(const AutopilotPkt_t* pPkt, float* pPayloadMass);

//! Form the throttle min packet, including the header and crc
void FormThrottleMinPacket(AutopilotPkt_t* pPkt, float Throttle);

//! Decode the throttle min packet
BOOL DecodeThrottleMinPacket(const AutopilotPkt_t* pPkt, float* pThrottle);

//! Form the ECU command packet
void FormECUCommandPacket(AutopilotPkt_t* pPkt, const ECUCommand_t* pCmd);

//! Decode the ECU command packet
BOOL DecodeECUCommandPacket(const AutopilotPkt_t* pPkt, ECUCommand_t* pCmd);


#ifdef __cplusplus
}
#endif

#endif // _AUTOPILOT_PACKET_H
