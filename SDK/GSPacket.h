/*! \file
    \brief Ground station packet utilities

    This module provides functions for encoding and decoding ground station
    packets to and from network representation as documented in "Piccolo Communications"
*/

// GSPacket.h
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

#ifndef _GS_PACKET_H
#define _GS_PACKET_H

#include "StreamPackets.h"
#include "GSDefinitions.h"

#ifdef _GROUND_STATION_PC
#include "GroundStationPC.h"
#endif


/*! Ground station message types */
enum GSPacketTypes
{
    ADD_ADDRESS,        //!< Add an address to the polling list.
    DEL_ADDRESS,        //!< Delete an address from the polling list.
    PILOT_ADDRESS,      //!< Add/change the pilot address.
    DGPS_SETTINGS,      //!< Differential corrections base station setup.
    GS_VERSION,         //!< Firmware version of the ground station.
    REQUEST_CONFIG,     //!< Request complete configuration data.
    TRANSMITTER_CON,    //!< Reserved.
    GS_OLD_TELEMETRY,   //!< Old format ground station telemetry.
    GROUP_ADDRESS,      //!< Gives the entire network configuration.
    GS_RADIO_SETTINGS,  //!< Radio settings.
    GS_SPECTRUM,        //!< Spectrum analyzer function and resulting data for up to 127 channels
    DYNAMIC_ENUMERATION,//!< Dynamic enumeration for the network
    GS_SPECTRUM2,       //!< Spectrum analyzer function for coherent MHX radios
    GS_USER_WARNING,    //!< Warning about user mistakes
    GS_RESET_REPORT,    //!< Reset report for the groundstation
    REQ_GPSHOTSTART,    //!< Request GPS hotstart
    DUAL_ANT_STATUS,    //!< Status report for dual antenna moving reference RTK system
    GS_CONFIGURATION,   //!< Change or request ground station configuration settting
    GS_NAME,            //!< Groundstation name
    GS_NEW_TELEMETRY,   //!< New format ground station telemetry message
    GS_DEVICE_DETAILS,  //!< Device details
	GS_GPS_HEALTH,		//!< Health information for ground station GPSs
	GS_SIMULATION_CONTROL, //!< Control packet for simulated ground station. 
    NUM_GS_MSGS         //!< Number of ground station packet types.
};

/*! Ground station configuration enumerations */
enum GSConfigurations
{
    GS_CONFIG_NORMAL,                   //!< Normal (single internal radio) ground station configuration
    GS_CONFIG_BAI,                      //!< BAI configured groundstation (secondary 9600 link, split on two ports)
    GS_CONFIG_FREE_WAVE_FULL_DUPLEX,    //!< Notional free wave configuration (secondary full duplex 57600 link)
    GS_CONFIG_TELEDESIGN_HALF_DUPLEX,   //!< Teledesign half duplex configuration (secondary half duplex 19200 link with specific timing)
    GS_CONFIG_MHX_EXTERNAL_HALF_DUPLEX, //!< Notional MHX external radio (secondary half duplex 57600 link)
    GS_CONFIG_AGIG_C2,                  //!< AGIG C2 link
    NUM_GS_CONFIG
};


typedef struct
{
    #define NUM_GROUP_ADDRESS 100

        // Data received from telemetry packet
    GPSUser_t GPS;                  //!< GPS information
    SystemDiagnostic_t System;      //!< Diagnostic and BIT data
    float PilotSamplePercentage;    //!< Pilot sample percentage
    float PilotUplinkRate;          //!< Pilot uplink rate
    UInt32 SystemTime;              //!< Time in milliseconds since reset

        // Data received from group address packet
    UInt16 Addresses[NUM_GROUP_ADDRESS];    //!< List of addresses in the ground station
    UInt8 AckRatio[NUM_GROUP_ADDRESS];      //!< Ack ratio for each address
    UInt8 NumAddresses;                     //!< Number of addresses in the list
    UInt16 PilotAddress;                    //!< Address of the manual pilot
    GSNetworkFlags_t Flags;                 //!< Network flags

}GSUserData_t;

/*! Amount of overhead in a ground station packet */
#define GS_PKT_OVERHEAD SIMPLE_PKT_OVERHEAD

#ifdef __cplusplus
extern "C" {
#endif

//! Decode GPS health information for a single GPS. 
BOOL DecodeGSGPSHealth(const GroundStationPkt_t* pPkt, GPSUser_t* pGPS, UInt8 *pIndex, UInt8* pNum);

//! Form a packet that describes the GPS health information, including satellite data.
void FormGSGPSHealthPacket(GroundStationPkt_t* pPkt, const GPS_t* pGPS, UInt8 Index, UInt8 Num);

//! Decode device details packet
const char* DecodeGSDeviceDetails(GroundStationPkt_t* pPkt, UInt8* pIndex, UInt8* pDevice, BOOL* pComplete);

//! Form the radio details packet
const char* FormGSDeviceDetailsPacket(GroundStationPkt_t* pPkt, const char* pDetails, UInt8 IndexThis, UInt8 Device, BOOL FinalDevice);

//! Decode the ground station group address packet in a GSUserData_t structure
BOOL DecodeGroupAddressPacketNew(const GroundStationPkt_t* pPkt, GSUserData_t* pData);

//! Form a ground station radio settings packet
void FormGSRadioSettingsPacket(GroundStationPkt_t* pPkt, const RadioSettings_t* pSettings);

//! Deocde a ground station radio settings packet
BOOL DecodeGSRadioSettingsPacket(const GroundStationPkt_t* pPkt, RadioSettings_t* pSettings);

//! Form a ground station telemetry packet.
void FormGSTelemetryPacket(GroundStationPkt_t* pPkt, UInt32 SystemTime, const GPS_t* pGPS, const SystemDiagnostic_t* pSystem, float PilotSamplePercentage, float PilotUplinkRate);

//! Decode a ground station telemetry packet, either the old packet or the new, in a GSUserData_t structure
BOOL DecodeGSTelemetryPacket(const GroundStationPkt_t* pPkt, GSUserData_t* pData);

//! Form a DGPS base station packet
void FormDGPSBasePacket(GroundStationPkt_t* pPkt, const DGPSParameter_t* pDGPS);

//! Decode a DGPS base station packet
BOOL DecodeDGPSBasePacket(const GroundStationPkt_t* pPkt, DGPSParameter_t* pData);

//! Form a ground station version packet
void FormGSVersionPacket(GroundStationPkt_t* pPkt, const Version_t* pVersion, const Board_t* pBoard);

//! Decode a ground station version packet
BOOL DecodeGSVersionPacket(const GroundStationPkt_t* pPkt, Version_t* pVersion, Board_t* pBoard);

//! Form a ground station group address packet
void FormGroupAddressPacket(GroundStationPkt_t* pPkt, const UInt16 Addresses[], const UInt8 AckRatio[], UInt8 NumAddresses, UInt16 PilotAddress, GSNetworkFlags_t Flags);

//! Decode a ground station group address packet
UInt8 DecodeGroupAddressPacket(const GroundStationPkt_t* pPkt, UInt16 Addresses[], UInt8 AckRatio[], UInt8 MaxAddresses, UInt16* pPilotAddress, GSNetworkFlags_t* Flags);

//! Form a ground station spectrun sweep packet which contains the results of spectrum analysis run on a coherent MHX radio
void FormGSSpectrumSweepPacket(GroundStationPkt_t* pPkt, const SpectrumSweep_t* pSpec);

//! From a ground station specturm sweep command packet
UInt32 FormGSSpectrumSweepDetails(GroundStationPkt_t* pPkt, float StartFreq, float Spacing, UInt16 Dwell, UInt8 NumMeasurements);

//! Decode a ground station radio spectrum sweep report packet
BOOL DecodeGSSpectrumSweepPacket(const GroundStationPkt_t* pPkt, SpectrumSweep_t* pSpec);

//! Decode a ground station spectrum sweep command packet
BOOL DecodeGSSpectrumSweepDetails(const GroundStationPkt_t* pPkt, float* pStartFreq, float* pSpacing, UInt16* pDwell, UInt8* pNumMeasurements);

//! Form a ground station radio spectrum packet
void FormGSSpectrumPacket(GroundStationPkt_t* pPkt, const SpectrumAnalyzer_t* pSpec, UInt32* pIdx);

//! Decode a ground station radio spectrum packet
BOOL DecodeGSSpectrumPacket(const GroundStationPkt_t* pPkt, SpectrumAnalyzer_t* pSpec, UInt32* pSeqNum);

//! Form a ground station user warning packet
const GroundStationPkt_t* FormGSUserWarningPacket(GroundStationPkt_t* pPkt, UInt8 Type, UInt8 Code, const char* pWarning);

//! Form a ground station user warning packet in place.
const GroundStationPkt_t* FormGSUserWarningPacketEx(GroundStationPkt_t* pPkt, UInt8 Type, UInt8 Code);

//! Decode the ground station user warning packet
char* DecodeGSUserWarningPacket(GroundStationPkt_t* pPkt, UInt8* pType, UInt8* pCode);

//! Form the ground station reset report packet
void FormGSResetReportPacket(GroundStationPkt_t* pPkt, UInt16 RSR, UInt16 Vec, UInt32 PC, UInt32 MSR, UInt32 Link, UInt32 FPSCR);

//! Decode the ground station reset report packet
BOOL DecodeGSResetReportPacket(const GroundStationPkt_t* pPkt, UInt16* pRSR, UInt16* pVec, UInt32* pPC, UInt32* pMSR, UInt32* pLink, UInt32* pFPSCR);

//! Form the dual antenna status packet
void FormDualAntStatusPacket(GroundStationPkt_t* pPkt, const DualAntStatus_t* pData);

//! Decode the dual antenna status packet
BOOL DecodeDualAntStatusPacket(const GroundStationPkt_t* pPkt, DualAntStatus_t* pData);

//! Form the ground station configuration packet
void FormGroundStationConfigurationPacket(GroundStationPkt_t* pPkt, UInt8 ConfigEnum, UInt8 SBASDisable, UInt8 EnableGD, UInt32 PayloadBaud, UInt8 EnableMHXRelay);

//! Decode the ground station configuration packet
BOOL DecodeGroundStationConfigurationPacket(const GroundStationPkt_t* pPkt, UInt8* pConfigEnum, UInt8* pSBASDisable, UInt8* pEnableGD, UInt32* pPayloadBaud, UInt8* pEnableMHXRelay);

//! Form a groundstation name packet
void FormGSNamePacket(GroundStationPkt_t* pPkt, const char *pName);

//! Decode a groundstation name packet
char *DecodeGSNamePacket(GroundStationPkt_t* pPkt);

void DecodeGroundStationSimControlPacket(const GroundStationPkt_t* pPkt);

#ifdef __cplusplus
}
#endif

#endif // _GS_PACKET_H
