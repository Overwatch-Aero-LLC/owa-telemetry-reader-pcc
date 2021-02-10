/*! \file
    \brief Gimbal packet encode and decode functionality
*/

/*! \mainpage Gimbal Standard Developers Kit

    The Gimbal SDK is a set of C source and header files which make it easier to write
    software applications to interface with a Cloud Cap TASE camera gimbal.  An example
    application has been provided which gives a simple command line interface to basic
    gimbal functionality; see TASEExample.cpp and TASEExampleSDK.sln. The complete 
    communications specification for the gimbal is given in Gimbal Communications.pdf.
*/

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

#ifndef _GIMBAL_PACKET_USER_H
#define _GIMBAL_PACKET_USER_H

#include "GimbalPacketGeneric.h"

#ifdef __cplusplus
extern "C" {
#endif

//! Interpret a gimbal system version packet
BOOL DecodeGimbalVersionPacket(const GimbalPkt_t* pPkt, Version_t* pVersion, Board_t* pBoard, UInt8 LegacyTypes[NUM_CAMERAS], UInt8 ProtocolTypes[NUM_CAMERAS], float MaxHFOV[NUM_CAMERAS], float MinHFOV[NUM_CAMERAS], Version_t* pLensVersion, LensCtrlType_t* pLensType );

//! Interpret a newer gimbal system version packet
BOOL DecodeGimbalVersion2Packet(const GimbalPkt_t* pPkt, Version_t* pVersion, Board_t* pBoard, UInt8 LegacyTypes[NUM_CAMERAS], UInt8 ProtocolTypes[NUM_CAMERAS], float MaxHFOV[NUM_CAMERAS], float MinHFOV[NUM_CAMERAS] );

//! Interpret a gimbal lens control version packet
BOOL DecodeLensControlVersionPacket(const GimbalPkt_t* pPkt, Version_t* pLensVersion, LensCtrlType_t* pLensType);

//! Create a gimbal command packet containing pan, tilt, zoom, and mode.
void FormGimbalCmdPacket(GimbalPkt_t* pPkt, float Pan, float Tilt, SInt8 ZoomRate, float CamTimeout, UInt8 Flags, float ImpulseTime, SInt8 FocusRate, UInt8 Flags1);

//! Interpret a gimbal angle packet containing the pan, tilt, and roll positions.
BOOL DecodeGimbalAnglePacket(const GimbalPkt_t* pPkt, float* pPan, float* pTilt, float* pRoll);

//! Interpret a gimbal rate packet containing the position rate and inertial data
BOOL DecodeGimbalRatePacket(const GimbalPkt_t* pPkt, float AxisRates[NGIMBAL], float MountRates[NGIMBAL], UInt32* pTime, UInt8* pMode);

//! Create a gimbal host sensor packet which contains gimbal mount body rates from the host
void FormGimbalHostSensorPacket(GimbalPkt_t* pPkt, const float Rates[NAXIS], const float Accel[NAXIS], float TAS, UInt32 Time, UInt8 SampleRate, UInt8 Flags);

//! Create a gimbal host attitude packets which contains the euler angles of the gimbal mount
void FormGimbalHostAttitudePacket(GimbalPkt_t* pPkt, const float Attitude[NAXIS], UInt32 Time, UInt8 SampleRate, UInt8 Flags, float Baro);

//! Create a gimbal host gps packet which contains GPS information from the host
void FormGimbalHostGPSPacket(GimbalPkt_t* pPkt, const GPSData_t* pGPS, UInt32 Time, UInt8 Valid);

//! Interpret a gimbal geo reference packet which contains the GPS and camera data from the gimbal
BOOL DecodeGimbalGeoreferencePacket(const GimbalPkt_t* pPkt, GimbalTelemetryData_t* pData);

//! Interpret a gimbal telemetry packet which contains the GPS and camera data from the gimbal
BOOL DecodeGimbalTelemetryPacket(const GimbalPkt_t* pPkt, GimbalTelemetryData_t* pData);

//! Decode the vibration data packet
BOOL DecodeGimbalRawVibrationPacket(const GimbalPkt_t* pPkt, UInt8* pSensor, UInt16* pSampleIndex, UInt16 Data[64], UInt16* pOffset, float* pGain);

//! Decode the video ground station antenna control packet
void FormVGSAntennaCommand(GimbalPkt_t* pPkt, float Pan, float Tilt, UInt8 Mode);

//! Decode a telemetry packet for the video ground station
BOOL DecodeVGSTelemetry(const GimbalPkt_t* pPkt, VGSTelemetryData_t* pData);

//! Decode special stabilizer telemetry packet
BOOL DecodeGimbalStabDevTelemetry(const GimbalPkt_t* pPkt, UInt32* uTime, float* Ref, float EncoderAngles[2], float CmdAngles[2], float EncoderRates[2], float CmdRates[2], float GyroRates[2], float Currents[2], float CmdCurrents[2]);

//! Create the initial heading packet
void FormGimbalInitialHeadingPacket(GimbalPkt_t* pPkt, float Heading);

//! Create a packet containing current debugging info
BOOL DecodeGimbalCurrentPacket(const GimbalPkt_t *pPkt, UInt32 *pTimeMs, float *pPanCur, float *pTiltCur, float *pPanCurCmd, float *pTiltCurCmd);

//! Decode the gimbal user warning packet
const char* DecodeGimbalUserWarningPacket(GimbalPkt_t* pPkt, UInt8* pGroup, UInt8* pType, UInt8* pSeverity);

//! Extract the data from a reset report packet
BOOL DecodeGimbalResetReportPacket(const GimbalPkt_t* pPkt, UInt16* pRSR, UInt16* pVec, UInt32* pPC, UInt32* pMSR, UInt32* pLink, UInt32* pFPSCR);

//! Extract the data from a gimbal diagnostics packet
BOOL DecodeGimbalDiagnosticsPacket(const GimbalPkt_t* pPkt, GimbalDiagnostics_t* pData);

//! Decode the VPS user warning packet
const char* DecodeVps3UserWarningPacket(const GimbalPkt_t* pPkt, UInt8* pGroup, UInt8* pType, UInt8* pSeverity, UInt16* pWarnId);

//! Extract the data from a reset report packet
BOOL DecodeVps3ResetReportPacket(const GimbalPkt_t* pPkt, Vps3Reset_t* pType, UInt32* pAddress, UInt32* pInfo);

//! Extract the diagnostics data from the packet
BOOL DecodeVps3DiagnosticsPacket(const GimbalPkt_t* pPkt, Vps3Diagnostics_t* pVpsDiagnostics );

//! Decode the VPS3 version packet
BOOL DecodeVps3VersionPacket(const GimbalPkt_t* pPkt, Version_t* pVersion );

//! Decode a MB user warning packet
const char* DecodeMbUserWarningPacket(const GimbalPkt_t* pPkt, UInt8* pGroup, UInt8* pType, UInt8* pSeverity, UInt16* pWarnId);

//! Decode a MB reset report packet
BOOL DecodeMbResetReportPacket(const GimbalPkt_t* pPkt, MbReset_t* pType, UInt32* pAddress, UInt32* pInfo);

//! Decode a MB app version packet
BOOL DecodeMbAppVersionPacket(const GimbalPkt_t* pPkt, Version_t* pVersion);

//! Decode an FPGA bit version packet
BOOL DecodeFpgaBitVersionPacket(const GimbalPkt_t* pPkt, Version_t* pVersion);

//! Decode the motion control version packet
BOOL DecodeMotionCtrlVersionPacket(const GimbalPkt_t* pPkt, Version_t* pVersion);

//! Extract the diagnostics data from the packet
BOOL DecodeMotionCtrlDiagnosticsPacket(const GimbalPkt_t* pPkt, MotionCtrlDiagnostics_t* pDiagnostics);

//! Extract the diagnostics data from the packet
BOOL DecodeMicroblazeDiagnosticsPacket(const GimbalPkt_t* pPkt, MicroblazeDiagnostics_t* pDiagnostics);

#ifdef __cplusplus
}
#endif

#endif // _GIMBAL_PACKET_USER_H
