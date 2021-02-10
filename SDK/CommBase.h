/*! \file
    \brief The CCT communication library base module.
    This module implements the communication interface to the Piccolo autopilot, groundstation, and family of CCT stabilized
    camera gimbals.
*/

#ifndef COMM_BASE_H
#define COMM_BASE_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#ifdef WIN32
    #ifndef AF_IPX
        #include <winsock2.h>
    #endif
    #include <io.h>
#else
	#include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
	#include <sys/socket.h>
    #include <netinet/ip.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <fcntl.h>
    #include <pthread.h>
    #include <errno.h>
	#include <string.h>

    // Preprocessor changes to make unix code look a littler closer to Win32
    #define SOCKET_ERROR -1
    #define closesocket close
#endif

#include "StreamPackets.h"
#include "GimbalPacket.h"
#include "GSPacket.h"
#include "Network.h"
#include "Definitions.h"
#include "AutopilotPacketGnd.h"
#include "Timer.h"


#define MAX_CLIENTS  10 //!< 10 possible client sockets
#define MAX_NETS 100    //!< 100 possible simultaneous avionics

#define MAX_BUFFER_SIZE 1000  //!< Number of bytes to buffer for UDP data.

/*! Data required for a client socket */
typedef struct
{
    int Socket;     //!< socket descriptor
    StreamPkt_t Pkt;//!< receive packet in progress
}Client_t;


/*! New network callback function pointer type */
typedef void (*NewNetworkCallBackfptr)(UInt16 NetworkID, void* Parameter);


//! The communication interface data structure
typedef struct
{
    char Error[256];                //!< Last error code
    char ReplayFileName[256];       //!< Replay file name
    char ServerAddress[256];        //!< Server address

    Network_t Net[MAX_NETS];        //!< Storage for all possible networks
    UInt32 NumNets;                 //!< The number of networks

    Serial_t Port;                  //!< Our serial communications channel
    UInt32 Baud;                    //!< The current baud rate

    FILE* StreamFile;               //!< Binary log or replay file
    char LogFilePath[256];          //!< Log file path to use.
    UInt32 StreamBytes;             //!< Number of bytes written or read
    BOOL ReplayMode;                //!< TRUE for replay mode
    UInt32 ReplayRate;              //!< Bytes per second for replay
    UInt64 OldReplayTime;           //!< Time of last replay data
    BOOL ReplayPaused;              //!< TRUE if replay paused
    BOOL ListenOnlyMode;            //!< TRUE for listen only mode (no uplink data)
    Timer_t CommSocketTimer;        //!< Timer for client socket reconnection
    Timer_t KeepAliveTimer;         //!< Timer for keep alive
    UInt16 BroadcastKeepAliveSeq;

    StreamPkt_t Pkt;                //!< Receive packet in progress

    NewNetworkCallBackfptr pNewNetworkCallBack;
    void*                  pNewNetworkCallBackParameter;

    BOOL   RunServerThread;
    int    ServeSocket;             //!< A socket used for listening for new connections
    int    ListenPort;              //!< The port the server socket is listening on

    //! Array of client information, the socket and the packet in progress
    Client_t Clients[MAX_CLIENTS];
    UInt32 NumClients;

    UInt8  Buffer[MAX_BUFFER_SIZE]; //!< UDP datagram buffer
    BOOL   IsSocketUdp;             //!< TRUE if connected to a UDP socket
    struct sockaddr_in TxAddress;   //!< Transmit address/port structure
    int    CommSocket;              //!< A socket used for sending and receiving data from an operator interface server

    BOOL Lock;                      //!< Critical section lock key

    Queue_t* pUserLogQ;             //!< Pointer to the queue used for logging custom user data

} CommBase_t;

#ifdef __cplusplus
extern "C" {
#endif

//! Open communication interface for file replay
void CommBase_OpenReplay(CommBase_t* pComm, const char* ReplayFile, int ListenPort);

//! Open communication interface on serial port
void CommBase_OpenSerial(CommBase_t* pComm, int Chan, UInt32 Baud, const char *IridiumNumber, int ListenPort, BOOL EnableLogging, const char* LogPath, BOOL ListenOnly);

//! Open communication interface on device
void CommBase_OpenDevice(CommBase_t* pComm, const char* sDevice, UInt32 Baud, const char *IridiumNumber, int ListenPort, BOOL EnableLogging, const char* LogPath, BOOL ListenOnly);

//! Open communication interface on network socket
void CommBase_OpenSocket(CommBase_t* pComm, const char* ServerAddress, BOOL EnableLogging, const char* LogPath, BOOL ListenOnly);

//! Close communication interface
void CommBase_Close(CommBase_t* pComm);


//! Change, restart, or stop the log file.
void CommBase_ChangeLogFile(CommBase_t* pComm, BOOL EnableLogging, const char *LogPath);

//! Change the serial port configuration
BOOL CommBase_NewSerialPortConfiguration(CommBase_t* pComm, int Chan, UInt32 Baud, const char* IridiumNumber, BOOL EnableLogging, const char* LogPath, BOOL ListenOnly);

//! Change the network client configuration
BOOL CommBase_NewNetworkClientConfiguration(CommBase_t* pComm, const char* ServerAddress, BOOL EnableLogging, const char* LogPath, BOOL ListenOnly);

//! Change the replay file configuration
BOOL CommBase_NewReplayFileConfiguration(CommBase_t* pComm, const char* ReplayFile);

//! Change the network server configuration
BOOL CommBase_NewNetworkServerConfiguration(CommBase_t* pComm, int ListenPort);


//! Return the current serial port parameters
BOOL CommBase_GetSerialPortConfiguration(CommBase_t* pComm, UInt8* Chan, UInt32* Baud);

//! Return the current Iridium configuration
BOOL CommBase_GetSatCommConfiguration(const CommBase_t* pComm, char* IridiumNumber);

//! Return the network client parametes
BOOL CommBase_GetNetworkClientConfiguration(const CommBase_t* pComm, char* ServerAddress);

//! Return the replay file name
BOOL CommBase_GetReplayFileConfiguration(const CommBase_t* pComm, char* ReplayFile);

//! Return the network server configuration
int CommBase_GetNetworkServerConfiguration(const CommBase_t* pComm);

//! Get the version of the communications manager software
void CommBase_GetVersionInformation(Version_t* pVersion);

//! Get stream receive pointer
Queue_t* CommBase_GetStreamRxBuffer(const CommBase_t* pComm, UInt16 NetworkID, UInt8 Stream);

//! Get stream transmit pointer
Queue_t* CommBase_GetStreamTxBuffer(const CommBase_t* pComm, UInt16 NetworkID, UInt8 Stream);

//! Get user log buffer pointer
Queue_t* CommBase_GetUserLogBuffer(const CommBase_t* pComm);


//! Call periodically to service the communications
void CommBase_RunNetwork(CommBase_t* pComm);

//! Register the call back function for new networks
void CommBase_SetNewNetworkCallBack(CommBase_t* pComm,
                                    void(*NewNetworkCallBack)(UInt16 NetworkID, void* Parameter),
                                    void* Parameter);


//! Send a block of data in a packet
void CommBase_SendBlock(CommBase_t* pComm, UInt16 NetworkID, UInt8 Stream, UInt8* pData, UInt32 Size);

//! Send a low level packet of data
void CommBase_SendStreamPacket(CommBase_t* pComm, StreamPkt_t* pPkt);

//! Write a block of custom user data to the telemetry file
void CommBase_LogUserData(CommBase_t* pComm, UInt8* pData, UInt32 Size);

//! Creates a network
SInt32 CommBase_CreateNetwork(CommBase_t* pComm, UInt16 NetworkID);

//! Destroys a network
void CommBase_DestroyNetwork(CommBase_t* pComm, UInt16 NetworkID);

//! Destroys all networks
void CommBase_DestroyAllNetworks(CommBase_t* pComm);

//! Get the number of networks that exist
UInt32 CommBase_GetNumNets(const CommBase_t* pComm);

//! Get the network IDs that are stored by the comm manager
SInt32 CommBase_GetIDFromIndex(const CommBase_t* pComm, UInt32 index);

//! Get the index into a network from ID
SInt32 CommBase_GetIndexFromID(const CommBase_t* pComm, UInt16 NetworkID);

//! Disable the replay
void CommBase_PauseReplay(CommBase_t* pComm);

//! Enable the replay
void CommBase_ResumeReplay(CommBase_t* pComm);

//! Return the state of the replay mode flag
BOOL CommBase_IsInReplay(const CommBase_t* pComm);

//! Return the state of the listen only mode flag
BOOL CommBase_IsInListenOnly(const CommBase_t* pComm);

//! Get the number of bytes read from the groundstation or from the relplay
UInt32 CommBase_GetNumStreamBytes(const CommBase_t* pComm);

//! Get the size of the log or replay file
UInt32 CommBase_GetFileSize(const CommBase_t* pComm);

//! Get the location of the file position
UInt32 CommBase_GetReplayFileLocation(const CommBase_t* pComm);

//! Set the location of the file position
void CommBase_SetReplayFileLocation(CommBase_t* pComm, UInt32 location);

//! Set the current replay rate
void CommBase_SetReplayRate(CommBase_t* pComm, UInt32 ReplayRate);

//! Get the replay rate
UInt32 CommBase_GetReplayRate(const CommBase_t* pComm);

//! Get the last error information
const char* CommBase_GetLastError(const CommBase_t* pComm);

//! Clear the error string
void CommBase_ClearLastError(CommBase_t* pComm);

//! Compute the downlink percentage for a specific network.*/
float CommBase_ComputeDownlinkPercent(CommBase_t* pComm, UInt16 NetworkID);


//! Get satcomm dialer status string
size_t CommBase_GetSatcommStatusString(char* String, size_t count);
size_t CommBase_GetSatcommErrorString(char* String, size_t count);

//! Hang up satcomm and redial
void CommBase_HangUpSatcommRedial(void);

//! Hang up satcomm and go to an idle state
void CommBase_HangUpSatcommIdle(void);

//! Start the dialing sequence
void CommBase_StartDialingSatcomm(CommBase_t* pComm);

//! Set/change the satcomm phone number
void CommBase_SetSatcommPhoneNumber(const char* PhoneNum);


///////////////////////////////////////////////////////////////////////////
// PACKETS FOR A PICCOLO AUTOPILOT
///////////////////////////////////////////////////////////////////////////

//! Send a autopilot packet
void CommBase_SendAutopilotPacket(CommBase_t* pComm, UInt16 NetworkID, const AutopilotPkt_t* pPkt);

//! Send a zero length autopilot packet
void CommBase_SendZeroLengthAutopilotPacket(CommBase_t* pComm, UInt16 NetworkID, UInt8 PktType);

//! Create and send a user action command packet
void CommBase_SendUserActionPacket(CommBase_t* pComm, UInt16 NetworkID, UInt8 ActionIndex, BOOL On);

//! Create and send a configuration lock packet
void CommBase_SendConfigurationLockPacket(CommBase_t* pComm, UInt16 NetworkID, const char Password[32]);

//! Create and send a configuration unlock packet
void CommBase_SendConfigurationUnlockPacket(CommBase_t* pComm, UInt16 NetworkID, const char Password[32], BOOL Silent);

//! Create and send a controller packet
void CommBase_SendControllerDataPacket(CommBase_t* pComm, UInt16 NetworkID, UInt8 Controller, UInt8 Version, UInt8 Category, UInt8 DataID, UInt8 NumData, const void* pData);

//! Create and send a controller packet to default data
void CommBase_SendControllerDataDefaultPacket(CommBase_t* pComm, UInt16 NetworkID, UInt8 Controller, UInt8 Version, UInt8 Category, UInt8 DataID, UInt8 NumData);

//! Create and send a transponder control packet
void CommBase_SendTransponderControlPacket(CommBase_t* pComm, UInt16 NetworkID, UInt16 Code, BOOL Ident, UInt8 Mode);

//! Create and send a transponder preflight packet
void CommBase_SendTransponderPreflightPacket(CommBase_t* pComm, UInt16 NetworkID, const char FlightID[9]);

//! Create and send a transponder installation packet
void CommBase_SendTransponderInstallationPacket(CommBase_t* pComm, UInt16 NetworkID, UInt32 ICAOAddress, const char Registration[9]);

//! Create and send a radio settings packet
void CommBase_SendRadioSettingsPacket(CommBase_t* pComm, UInt16 NetworkID, const RadioSettings_t* pSettings);

//! Create and send a autopilot loop control packet
void CommBase_SendAutopilotLoopControlPacket(CommBase_t* pComm, UInt16 NetworkID, const AutopilotLoopCmd_t* pCmd, BOOL UpdateEEPROM);

//! Create and send a control actuator calibration packet
void CommBase_SendActuatorTablePacket(CommBase_t* pComm, UInt16 NetworkID, const ActuatorTable_t* pTable, 
                                      UInt8 Sur, UInt8 Type, UInt8 Virtual, UInt8 Fast);

//! Create and send a packet to change actuator type
void CommBase_SendActuatorTypePacket(CommBase_t* pComm, UInt16 NetworkID, UInt8 Sur, UInt8 Type);

//! Create and send a sensor orientatin packet
void CommBase_SendOrientationPacket(CommBase_t* pComm, UInt16 NetworkID, const float Euler[NAXIS], const float IMUtoAnt[N3D], const float IMUtoAnt2[N3D]);

//! Create and send waypoints packet
void CommBase_SendWaypointsPacket(CommBase_t* pComm, UInt16 NetworkID, const FPPoint_t Points[MAX_WAYPOINTS_IN_PACKET], UInt16 Num, BOOL Ack);

//! Create and send a waypoint list packet
void CommBase_SendWaypointListPacket(CommBase_t* pComm, UInt16 NetworkID, const FPMask_t Mask, UInt8 Flag);

//! Create and send a waypoint list packet using a range of points
void CommBase_SendWaypointListRangePacket(CommBase_t* pComm, UInt16 NetworkID, UInt16 Start, UInt16 End, UInt8 Flag);

//! Create and send a legacy waypoint packet
void CommBase_SendLegacyWaypointPacket(CommBase_t* pComm, UInt16 NetworkID, const FPPoint_t* pPoint, UInt8 Index);

//! Create and send a legacy waypoint list packet
void CommBase_SendLegacyWaypointListPacket(CommBase_t* pComm, UInt16 NetworkID, const FPMask_t Mask, UInt8 Flag);

//! Create and send a surface test packet
void CommBase_SendSurfaceTestPacket(CommBase_t* pComm, UInt16 NetworkID, float Angle, float Width, float Freq, float Dev, UInt8 Index);

//! Create and send a mission limits packet
void CommBase_SendMissionLimitsPacket(CommBase_t* pComm, UInt16 NetworkID, const  MissionLimits_t* pMission);

//! Create and a send a mission settings packet
void CommBase_SendMissionSettingsPacket(CommBase_t* pComm, UInt16 NetworkID, SInt16 MinAltitude, SInt16 MaxAltitude, UInt32 FlightTimeout);

//! Create and send a airspace boundary packet
void CommBase_SendBoundaryPacket(CommBase_t* pComm, UInt16 NetworkID, const Boundary_t* pBnd);

//! Create and send a magnetic sensor calibration packet
void CommBase_SendMagCalPacket(CommBase_t* pComm, UInt16 NetworkID, const float HardIronErr[N3D], const float ScaleFactErr[N3D]);

//! Create and send a sensor error
void CommBase_SendSensorErrorPacket(CommBase_t* pComm, UInt16 NetworkID, const SensorErrorData_t* pSensorError);

//! Create and send a external serial setup packet
void CommBase_SendExternalSerialSetupPacket(CommBase_t* pComm, UInt16 NetworkID, const ExternalSerial_t* pSerial);

//! Create and send a external IO setup packet
void CommBase_SendExternalIOSetupPacket(CommBase_t* pComm, UInt16 NetworkID, const ExternalIO_t* pExternalIO, UInt8 pin, UInt32 SampleTime, BOOL UpdateEEPROM);

//! Create and send a simple landing packet
void CommBase_SendSimpleLandingPacket(CommBase_t* pComm, UInt16 NetworkID, const SimpleLandingPattern_t* pLand);

//! Create and send an altimeter setting packet
void CommBase_SendAltimeterSettingPacket(CommBase_t* pComm, UInt16 NetworkID, UInt32 Altimeter, BOOL ControlToGPS);

//! Create and send a bandwidth mode packet
void CommBase_SendBandwidthModePacket(CommBase_t* pComm, UInt16 NetworkID, BandwidthMode_t Bandwidth);

//! Create and send the engine kill packet
void CommBase_SendEngineKillPacket(CommBase_t* pComm, UInt16 NetworkID, BOOL EngineKill);

//! Create and send the land now command packet.
void CommBase_SendLandNowPacket(CommBase_t* pComm, UInt16 NetworkID);

//! Create and send the launch now command packet.
void CommBase_SendLaunchNowPacket(CommBase_t* pComm, UInt16 NetworkID);

//! Create and send the go to prelaunch command packet.
void CommBase_SendPrelaunchPacket(CommBase_t* pComm, UInt16 NetworkID);

//! Create and send the zero air data command packet.
void CommBase_SendAirDataZeroPacket(CommBase_t* pComm, UInt16 NetworkID, float ActualAltitude);

//! Create and send the manual assist mode command packet.
void CommBase_SendManualAssistModePacket(CommBase_t* pComm, UInt16 NetworkID, UInt8 Mode);

//! Create and send the lights command packet.
void CommBase_SendLightsCommandPacket(CommBase_t* pComm, UInt16 NetworkID, BOOL LightsOn);

//! Create and send the drop command packet.
void CommBase_SendDropCommandPacket(CommBase_t* pComm, UInt16 NetworkID, BOOL DropOn);

//! Create and send the parachute command packet.
void CommBase_SendParachuteCommandPacket(CommBase_t* pComm, UInt16 NetworkID, BOOL ParachuteDeploy);

//! Create and send the brakes command packet.
void CommBase_SendBrakesCommandPacket(CommBase_t* pComm, UInt16 NetworkID, float Brakes);

//! Create and send the throttle trim command packet.
void CommBase_SendThrottleTrimPacket(CommBase_t* pComm, UInt16 NetworkID, float Throttle);

//! Create and send a track command packet.
void CommBase_SendTrackCommandPacket(CommBase_t* pComm, UInt16 NetworkID, UInt16 Waypoint, BOOL GoTo);

//! Create and send a parameter description packet.
void CommBase_SendDescriptionPacket(CommBase_t* pComm, UInt16 NetworkID, const char Description[64]);

//! Create and send a user space packet.
void CommBase_SendUserSpacePacket(CommBase_t* pComm, UInt16 NetworkID, const UInt8 User[64]);

//! Create and send a doublet command packet
void CommBase_SendDoubletCommandPacket(CommBase_t* pComm, UInt16 NetworkID, const DoubletCmd_t* pDoublet);

//! Create and send a frequency sweep command packet
void CommBase_SendFrequencySweepCommandPacket(CommBase_t* pComm, UInt16 NetworkID, const FrequencySweepCmd_t* pFreqSwp);

//! Create and send a set fuel level packet.
void CommBase_SendSetFuelLevelPacket(CommBase_t* pComm, UInt16 NetworkID, float Fuel, BOOL Electric);

//! Create and send a set fuel level packet for fuel update while locked.
void CommBase_SendSetFuelIfLockedPacket(CommBase_t* pComm, UInt16 NetworkID, float Fuel, BOOL Electric);

//! Create and send a payload mass packet
void CommBase_SendPayloadMassPacket(CommBase_t* pComm, UInt16 NetworkID, float PayloadMass);

//! Create and send a throttle min packet
void CommBase_SendThrottleMinPacket(CommBase_t* pComm, UInt16 NetworkID, float Throttle);

//! Create and send a feature code packet.
void CommBase_SendFeatureCodePacket(CommBase_t* pComm, UInt16 NetworkID, const UInt8 Data[8]);

//! Create and send terrain elevation information.
void CommBase_SendTerrainElevationDataPacket(CommBase_t* pComm, UInt16 NetworkID, UInt32 Time, float Elevations[6]);


//! Request some controller data
void CommBase_RequestControllerDataPacket(CommBase_t* pComm, UInt16 NetworkID, UInt8 Controller, UInt8 Version, UInt8 Category, UInt8 DataID, UInt8 NumData);

//! Request the transponder control packet
void CommBase_RequestTransponderControlPacket(CommBase_t* pComm, UInt16 NetworkID);

//! Request the transponder preflight packet
void CommBase_RequestTransponderPreflightPacket(CommBase_t* pComm, UInt16 NetworkID);

//! Request the transponder installation packet
void CommBase_RequestTransponderInstallationPacket(CommBase_t* pComm, UInt16 NetworkID);

//! Request the radio settings packet
void CommBase_RequestRadioSettingsPacket(CommBase_t* pComm, UInt16 NetworkID);

//! Request the autopilot surface table calibration packet.
void CommBase_RequestActuatorTablePacket(CommBase_t* pComm, UInt16 NetworkID, UInt8 Sur);

//! Request the autopilot orientation packet.
void CommBase_RequestOrientationPacket(CommBase_t* pComm, UInt16 NetworkID);

//! Request the autopilot mission limits packet.
void CommBase_RequestMissionLimitsPacket(CommBase_t* pComm, UInt16 NetworkID);

//! Request the packet with the magnetometer calibration data.
void CommBase_RequestMagCalPacket(CommBase_t* pComm, UInt16 NetworkID);

//! Request the packet with the sensor error data
void CommBase_RequestSensorErrorPacket(CommBase_t* pComm, UInt16 NetworkID);

//! Request the autopilot packet with the external serial setup parameters.
void CommBase_RequestExternalSerialSetupPacket(CommBase_t* pComm, UInt16 NetworkID);

//! Request then autopilot packet with the setup parameters for a single external IO pin.
void CommBase_RequestExternalIOSetupPacket(CommBase_t* pComm, UInt16 NetworkID, UInt8 pin);

//! Request the parameter description packet.
void CommBase_RequestDescriptionPacket(CommBase_t* pComm, UInt16 NetworkID);

//! Request the user space packet.
void CommBase_RequestUserSpacePacket(CommBase_t* pComm, UInt16 NetworkID);

//! Request the altimeter setting packet.
void CommBase_RequestAltimeterSettingPacket(CommBase_t* pComm, UInt16 NetworkID);

//! Request the bandwidth mode packet
void CommBase_RequestBandwidthModePacket(CommBase_t* pComm, UInt16 NetworkID, UInt8 NetID);

//! Request system version packet
void CommBase_RequestSystemVersionPacket(CommBase_t* pComm, UInt16 NetworkID);

//! Request the raw vibration sensor data
void CommBase_RequestRawVibrationDataPacket(CommBase_t* pComm, UInt16 NetworkID);

//! Request the payload mass packet
void CommBase_RequestPayloadMassPacket(CommBase_t* pComm, UInt16 NetworkID);

//! Request the throttle min packet
void CommBase_RequestThrottleMinPacket(CommBase_t* pComm, UInt16 NetworkID);


//! Write the header for the log file
BOOL CommBase_WriteTextLogHeader(FILE* pFile);

//! Write a single line of data for the log file
BOOL CommBase_WriteTextLogLine(FILE* pFile, const UserData_t* pData, UInt8 AckRatio, const double TrackPosition[N3D],
                               const float TrackVelocity[N3D], const float ADCSample[NUM_EXTERNAL_ADC_PINS],
							   const ECU_t* pLeftECU, const ECU_t* pRightECU, const MovingBaseStatus_t* pMBStatus,
							   const GPSUser_t* pGSGPS, float GSRSSI, float GSPilotPercentage, float GSPilotRate);


///////////////////////////////////////////////////////////////////////////
// PACKETS FOR A PICCOLO GROUNDSTATION
///////////////////////////////////////////////////////////////////////////

//! Send an groundstation packet
void CommBase_SendGroundStationPacket(CommBase_t* pComm, const GroundStationPkt_t* pPkt);

//! Send a zero length ground station packet
void CommBase_SendZeroLengthGroundStationPacket(CommBase_t* pComm, UInt8 PktType);

//! Create and send a ground station add address packet
void CommBase_SendGSAddAddressPacket(CommBase_t* pComm, UInt16 Address);

//! Create and send a ground station add pilot address packet
void CommBase_SendGSAddPilotAddressPacket(CommBase_t* pComm, UInt16 Pilot);

//! Create and send a ground station delete address packet
void CommBase_SendGSDeleteAddressPacket(CommBase_t* pComm, UInt16 DelAddress, UInt16 KeepAddress);

//! Create and a ground station radio settings packet
void CommBase_SendGSRadioSettingsPacket(CommBase_t* pComm, const RadioSettings_t* pSettings);

//! Create and send a ground station differential corrections packet
void CommBase_SendDGPSBasePacket(CommBase_t* pComm, const DGPSParameter_t* pDGPS);

//! Create and send a dynamic enumeration on/off packet
void CommBase_SendGSDynamicEnumerationPacket(CommBase_t* pComm, BOOL On);

//! Create and send a ground station configuration packet
void CommBase_SendGSConfiguration(CommBase_t* pComm, UInt8 ConfigEnum, UInt8 SBASDisable, UInt8 EnableGD, UInt32 Baud, UInt8 EnableMHXRelay);


//! Request ground station radio settings
void CommBase_RequestGSRadioSettingsPacket(CommBase_t* pComm);

//! Request ground station spectrum sweep, coherent frequency radio
void CommBase_RequestGSCoherentSpectrumSweepPacket(CommBase_t* pComm, float StartFreq, float Spacing, UInt16 Dwell, UInt8 NumMeasurements);

//! Request ground station spectrum sweep, hopping radio
void CommBase_RequestGSHoppingSpectrumSweepPacket(CommBase_t* pComm);

//! Request the software and hardware configuration settings from the ground station
void CommBase_RequestGSFirmwarePacket(CommBase_t* pComm);

//! Request the packet with DGPS base settings
void CommBase_RequestDGPSBasePacket(CommBase_t* pComm);


///////////////////////////////////////////////////////////////////////////
// PACKETS FOR A TASE GIMBAL CONNECTED TO A PICCOLO AUTOPILOT
///////////////////////////////////////////////////////////////////////////

//! Send a gimbal packet on the gimbal stream to the autopilot
void CommBase_SendGimbalPacket(CommBase_t* pComm, UInt16 NetworkID, const GimbalPkt_t* pPkt);

//! Create and send a gimbal command packet containing pan, tilt, zoom and focus rate, and mode.
void CommBase_SendGimbalCmdPacket(CommBase_t* pComm, UInt16 NetworkID, float Pan, float Tilt, SInt8 ZoomRate, float CamTimeout, UInt8 Flags, float ImpulseTime, SInt8 FocusRate, UInt8 Flags1);

//! Create and send a gimbal sensor point of interest packet containing the position of the SPOI
void CommBase_SendGimbalSPOICmdPacket(CommBase_t* pComm, UInt16 NetworkID, const double Pos[NPOS], const float Vel[NDIR], UInt8 Flags);

//! Create and send a camera command packet
void CommBase_SendGimbalCameraCmdPacket(CommBase_t* pComm, UInt16 NetworkID, UInt8 Camera, float HFOV, UInt16 Focus, UInt8 Mode, UInt8 Shutter, UInt8 Bright, UInt8 Flags);

//! Create and send a gimbal axis settings packet
void CommBase_SendGimbalAxisSettingsPacket(CommBase_t* pComm, UInt16 NetworkID, const float NoGoCenter[NGIMBAL_SHORT], const float NoGoRange[NGIMBAL_SHORT], const float MaxRate[NGIMBAL_SHORT], const float MaxAccel[NGIMBAL_SHORT], const float PosGain[NGIMBAL_SHORT], const float VelGain[NGIMBAL_SHORT], const float DerGain[NGIMBAL_SHORT], BOOL Temporary, BOOL Factory);

//! Create and send gimbal packet rate packet
void CommBase_SendGimbalSerialInterfacePacket(CommBase_t* pComm, UInt16 NetworkID, GimbalSerial_t Serial[6], BOOL Temporary);

//! Create and send gimbal camera alignment packet
void CommBase_SendGimbalCameraAlignPacket(CommBase_t* pComm, UInt16 NetworkID, float Zoom1, const float Error1[NGIMBAL], float Zoom2, const float Error2[NGIMBAL], UInt8 Camera, BOOL Factory);

//! Create and send gimbal camera definitions packet
void CommBase_SendGimbalCameraTypesPacket(CommBase_t* pComm, UInt16 NetworkID, const CameraDefinition_t Cam[NUM_CAMERAS], const BOOL Send[NUM_CAMERAS], BOOL Factory);

//! Create and send gimbal laser setup packet
void CommBase_SendGimbalLaserSetupPacket(CommBase_t* pComm, UInt16 NetworkID, LaserSetup_t Lasers[NUM_LASERS], BOOL Factory);

//! Create and send gimbal laser arm packet
void CommBase_SendGimbalLaserArmPacket(CommBase_t* pComm, UInt16 NetworkID, UInt8 LaserIndex, BOOL Arm);

//! Create and send gimbal laser fire packet
void CommBase_SendGimbalLaserFirePacket(CommBase_t* pComm, UInt16 NetworkID, UInt8 LaserIndex, UInt8 Power);

//! Create and send gimbal retract/deploy command
void CommBase_SendGimbalRetractDeployCmdPacket(CommBase_t* pComm, UInt16 NetworkID, UInt32 Deploy);

//! Create and send gimbal track command packet
void CommBase_SendGimbalTrackCmdPacket(CommBase_t* pComm, UInt16 NetworkID, float InitialX, float InitialY, UInt8 Algorithm, UInt8 Size, UInt8 Stamp);

//! Request a sensor point of interest command packet
void CommBase_RequestGimbalSPOICmdPacket(CommBase_t* pComm, UInt16 NetworkID);

//! Request a camera command packet
void CommBase_RequestGimbalCameraCmdPacket(CommBase_t* pComm, UInt16 NetworkID, UInt8 Camera);

//! Request a gimbal axis settings packet
void CommBase_RequestGimbalAxisSettingsPacket(CommBase_t* pComm, UInt16 NetworkID, BOOL Factory);

//! Request camera alignment information
void CommBase_RequestGimbalCameraAlignPacket(CommBase_t* pComm, UInt16 NetworkID, UInt8 Camera, BOOL Factory);

//! Request gimbal camera types
void CommBase_RequestGimbalCameraTypesPacket(CommBase_t* pComm, UInt16 NetworkID, BOOL Factory);

//! Request gimbal laser setup
void CommBase_RequestGimbalLaserSetupPacket(CommBase_t* pComm, UInt16 NetworkID, BOOL Factory);

#ifdef __cplusplus
}
#endif

#endif
