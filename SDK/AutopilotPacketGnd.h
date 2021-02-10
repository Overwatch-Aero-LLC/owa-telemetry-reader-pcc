/*! \file
    \brief Autopilot packet utilities used on the ground

    This module provides packet creation and decoding functions for autopilot
    stream packets that are specific to ground systems as documented in
    "Piccolo Communications".
*/
// AutopilotPacketGnd.h
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

#ifndef _AUTOPILOT_PACKET_GND_H
#define _AUTOPILOT_PACKET_GND_H

#include "AutopilotPacket.h"

/*! Aggregate data type for holding typical telemetry information */
typedef struct
{
        // This data comes from the system status packet
    SystemDiagnostic_t  System;     //!< System diagnostic data

        // This data comes from the system status packet
    NavDiagnostic_t     Nav;        //!< Navigation diagnostic data

        // This data comes from the system status packet
    AutopilotStatus_t   AP;         //!< Autopilot status data

        // This data comes from the telemetry packet
    UInt32          SystemTime;     //!< ms since system turned on

        // GPS data (Flags.GPS)
    GPSUser_t       GPS;            //!< GPS data

        // Calculated data (Flags.CalSensor)
    float           Euler[NAXIS];   //!< Euler angles [rad]
    float           BaroAlt;        //!< Barometric altitude [m]
    float           TAS;            //!< True air speed [m/s]
    float           Density;        //!< Atmospheric density [kg/m^3]
    float           WindSouth;      //!< Wind from the south [m/s]
    float           WindWest;       //!< Wind from the west [m/s]
    UInt16          LeftRPM;        //!< Left engine RPM
    UInt16          RightRPM;       //!< Right engine RPM
    float           WeightOnWheel;  //!< Weight on wheel from 0.0 to 1.0, -1.0 if N/A
    BOOL            GroundContact;  //!< TRUE if ground contact sensed

        // Raw sensor data (Flags.RawSensor)
    float           IAS;            //!< Indicated air speed [m/s]
    float           Pdynamic;       //!< Dynamic pressure [Pa]
    float           OAT;            //!< Outside air temperature [°C]
    float           Pstatic;        //!< Static pressure [Pa]
    float           Gyro[NAXIS];    //!< Gyro data [rad/s]
    float           Accel[N3D];     //!< Accelerometer data [m/s/s]

        // Magnetometer data (Flags.Magnetometer)
    float           MagField[N3D];  //!< Magnetic field in body frame [mGauss]
    float           MagHeading;     //!< Magnetic heading
    float           AGL;            //!< Altitude above ground, filtered

        // Actuator data (Flags.NumActuators)
    float           Actuators[NUM_ACTUATORS];   //!< Actauator data

        // Fuel data
    float           Fuel;           //!< Fuel remaining in kilograms or Watt-Hours
    float           FuelFlow;       //!< Rate of fuel consumption in grams per hour or Watts
    BOOL            Electric;       //!< TRUE if the engine is electric, else fuel based

        // Novatel Align data (Flags.AlignHeading)
    float           AlignHeading;       //!< Align GPS heading angle, in rad
    float           AlignHeadingSigma;  //!< Align GPS heading standard deviation, in rad
    UInt8           AlignSolnType;      //!< Align GPS solution type
    UInt8           AlignVisibleSats;   //!< Align GPS number of visible sats
    UInt8           AlignTrackedSats;   //!< Align GPS number of tracked sats

		// GPS residuals (Flags.GPSResidual)
	float			PRes[N3D];			//!< Position difference between GPS and INS in meters North, East, Down
	float			VRes[N3D];			//!< Velocity difference between GPS and INS in m/s North, East, Down

        // Altitude used by the controller (Flags.AltUsed)
    float           AltUsed;            //!< Altitude used by the controller, m
	
        // Meta data about telemetry
    AvailableData_t AvailableData;  //!< Bit field indicating data in telemetry packet
    TelemLimits_t   Limits;         //!< Bit field indicating items outside limits

}UserData_t;


/*! Data type for holding a single sample of low speed doublet data */
typedef struct
{
    UInt16  LeftRPM;        //!< Left engine RPM
    UInt16  RightRPM;       //!< Right engine RPM
    float   Euler[NAXIS];   //!< Euler angles [rad]
    float   Controls[NUM_CONTROL_AXIS]; //!< Autopilot control outputs
}LowSpeedDoubletData_t;


/*! Data type for holding a single sample of high speed doublet data */
typedef struct
{
    float   Pdynamic;       //!< Dynamic pressure [Pa]
    float   Pstatic;        //!< Static pressure [Pa]
    float   Gyro[NAXIS];    //!< Gyro data [rad/s]
    float   Accel[N3D];     //!< Accelerometer data [m/s/s]
    UInt16  ADC[NUM_EXTERNAL_ADC_PINS];
}HighSpeedDoubletData_t;

// Prototypes

#ifdef __cplusplus
extern "C" {
#endif

//! Decode GPS health information for a single GPS. 
BOOL DecodeGPSHealth(const AutopilotPkt_t* pPkt, GPSUser_t* pGPS, UInt8 *pIndex, UInt8* pNum);

//! Build a sensor error packet
void FormSensorErrorPacket(AutopilotPkt_t* pPkt, const SensorErrorData_t* pSensorError);

//! Decode a sensor error packet
BOOL DecodeSensorErrorPacket(const AutopilotPkt_t* pPkt, SensorErrorData_t* pSensorError);

//! Decode device details packet
const char* DecodeDeviceDetails(AutopilotPkt_t* pPkt, UInt8* pIndex, UInt8* pDevice, BOOL* pComplete);

//! Form the user action command
void FormUserActionPacket(AutopilotPkt_t* pPkt, UInt8 ActionIndex, BOOL On);

//! Decode an ECU status packet
BOOL DecodeECUStatusPacket(const AutopilotPkt_t* pPkt, ECU_t* pECU);

//! Decode a high resolution telemetry packet
BOOL DecodeTelemetryHiResPacket(const AutopilotPkt_t* pPkt, UserData_t* pData);

//! Decode a low resolution telemetry packet
BOOL DecodeTelemetryLoResPacket(const AutopilotPkt_t* pPkt, UserData_t* pData);

//! Decode a system status packet
BOOL DecodeSystemStatusPacket(const AutopilotPkt_t* pPkt, SystemDiagnostic_t* pSystem, NavDiagnostic_t* pNav, AutopilotStatus_t* pStatus, BOOL* pLong);

//! Decode a legacy high resolution system status packet
BOOL DecodeLegacySystemStatusHiResPacket(const AutopilotPkt_t* pPkt, SystemDiagnostic_t* pSystem, NavDiagnostic_t* pNav, AutopilotStatus_t* pStatus, BOOL* pLong);

//! Decode a legacy low resolution system status packet
BOOL DecodeLegacySystemStatusLoResPacket(const AutopilotPkt_t* pPkt, SystemDiagnostic_t* pSystem, NavDiagnostic_t* pNav, AutopilotStatus_t* pStatus, BOOL* pLong);

//! Form a controller data request packet
void FormControllerDataRequestPacket(AutopilotPkt_t* pPkt, UInt8 Controller, UInt8 Version, UInt8 Category, UInt8 DataID, UInt8 NumData);

//! Form a controller data default packet
void FormControllerDataDefaultPacket(AutopilotPkt_t* pPkt, UInt8 Controller, UInt8 Version, UInt8 Category, UInt8 DataID, UInt8 NumData);

//! Decode a low speed doublet data packet
UInt32 DecodeLowSpeedDoubletDataPacket(const AutopilotPkt_t* pPkt, UInt16* pSampleIndex, UInt16* pSampleRate, UInt16* pTotalSamples, LowSpeedDoubletData_t Data[10]);

//! Decode a high speed doublet data packet
UInt32 DecodeHighSpeedDoubletDataPacket(const AutopilotPkt_t* pPkt, UInt16* pSampleIndex, UInt16* pSampleRate, UInt16* pTotalSamples, HighSpeedDoubletData_t Data[10]);

//! Form a packet commanding a doublet manuever
void FormDoubletCommandPacket(AutopilotPkt_t* pPkt, const DoubletCmd_t* pCmd);

//! Form a packet commanding a frequency sweep manuever
void FormFrequencySweepCommandPacket(AutopilotPkt_t* pPkt, const FrequencySweepCmd_t* pCmd);

//! Decode a raw vibration packet
UInt8 DecodeRawVibrationPacket(const AutopilotPkt_t* pPkt, UInt16* pSampleIndex, UInt16 Data[64], UInt16* pOffset, float* pGain);

//! Extract the data from an external IO sample packet
BOOL DecodeExternalIOSamplePacket(const AutopilotPkt_t* pPkt, UInt32 Sample[NUM_EXTERNAL_IO_PINS]);

//! Extract the data from a payload analog input samples packet
BOOL DecodeExternalADCSamplePacket(const AutopilotPkt_t* pPkt, float Sample[NUM_EXTERNAL_ADC_PINS]);

//! Extract the data from a reset report packet
BOOL DecodeResetReportPacket(const AutopilotPkt_t* pPkt, UInt16* pRSR, UInt16* pVec, UInt32* pPC, UInt32* pMSR, UInt32* pLink, UInt32* pFPSCR);

//! Extract the data from a system version packet
BOOL DecodeSystemVersionPacket(const AutopilotPkt_t* pPkt, Version_t* pVersion, Board_t* pBoard);

//! Form an actuator test command packet
void FormActuatorTestPacket(AutopilotPkt_t* pPkt, float Angle, float Width, float Freq, float Dev, UInt8 Index);

//! Form a set fuel level packet
void FormSetFuelLevelPacket(AutopilotPkt_t* pPkt, float Fuel, BOOL Electric);

//! Form a packet to set the fuel level even if the autopilot is locked.
void FormSetFuelIfLockedPacket(AutopilotPkt_t* pPkt, float Fuel, BOOL Electric);

//! Extract the data from a moving base status packet
BOOL DecodeMovingBaseStatusPacket(const AutopilotPkt_t* pPkt, MovingBaseStatus_t* pStatus);

//! Extract the data from a user warning packet
const char* DecodeUserWarningPacket(AutopilotPkt_t* pPkt, UInt8* pType, UInt8* pCode);

//! Form feature code packet
void FormFeatureCodePacket(AutopilotPkt_t* pPkt, const UInt8 Data[8]);

//! Elevation data for uplink to the avionics
void FormTerrainElevationDataPacket(AutopilotPkt_t* pPkt, UInt32 Time, const float Elevations[6]);

//! Form the mission settings packet
void FormMissionSettingsPacket(AutopilotPkt_t* pPkt, SInt16 MinAltitude, SInt16 MaxAltitude, UInt32 FlightTimeout);

//! Form the airspace boundary packet
void FormBoundaryPacket(AutopilotPkt_t* pPkt, const Boundary_t* pBnd);

//! Decode the airspace boundary packet
BOOL DecodeBoundaryPacket(const AutopilotPkt_t* pPkt, Boundary_t* pBnd);

//! Form a configuration lock packet
void FormConfigurationLockPacket(AutopilotPkt_t* pPkt, const char Password[32]);

//! Form a configuration unlock packet
void FormConfigurationUnlockPacket(AutopilotPkt_t* pPkt, const char Password[32], BOOL Silent);

//! Decode the transponder status packet
BOOL DecodeTransponderStatusPacket(const AutopilotPkt_t* pPkt, XPDRStatus_t* pStatus);

//! Decode the servo telemetry packet
UInt32 DecodeServoTelemetryPacket(const AutopilotPkt_t* pPkt, ServoTelemetry_t ServoTel[NUM_ACTUATORS]);

#ifdef __cplusplus
}
#endif

#endif // _AUTOPILOT_PACKET_GND_H
