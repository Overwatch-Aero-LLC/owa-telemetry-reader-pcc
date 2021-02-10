/*! \file
    \brief Communications manager for PCs talking to the Piccolo system.

    The communications manager abstracts the low level details of talking to one
    or more Piccolo systems.  It can work over serial lines or over an IP network
    as either a client or a server.
*/

// CommManager.h
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


/*! \mainpage Communications Standard Developer Kit
    \section section1 Using the communications SDK
    The communications SDK is a hybrid mix of both C and C++ modules.  It is
    this way because it shares many source modules with the avionics and ground
    station software which is written entirely in C.

    \subsection subsection1_1 Instantiating the communications manager
    In order to use the SDK you first instantiate a CCommManager object, passing
    it the necessary constructor arguments to determine how the communications
    manager will function (serial, network client, network server).  For example:

    \code
    m_pComm = new CCommManager(COM1, "", 2000);
    \endcode

    Will create a CCommManager object that communicates over the first serial
    port to a ground station, and serves communications streams over port 2000.
    Alternatively you could:

    \code
    m_pComm = new CCommManager(0, "hostname:2000", 0);
    \endcode

    To create a CCommManager object which will connect to a server over port 2000.

    \subsection subsection1_2   Register the call back function
    Once instantiated you should register a call back functions.  The
    communications manager object uses this call backs to alert you when a
    new avionics is detected.  To register the call back function you would
    call:

    \code
    m_pComm->SetNewNetworkCallBack(NewNetwork, this);
    \endcode

    Where NewNetwork is the name of a statically accessible function which
    accepts two parameters according to:

    \code
    void NewNetwork(UInt16 NetworkID, void* Parameter)
    \endcode

    The first parameter is the network address of the new avionics.  The second
    parameter is an echo of the second parameter passed to the SetNewNetworkCallBack()
    function.  The communications manager keeps an array of Network_t data
    structures, one for each avionics and ground station in the network.  If a
    packet from a new avionics arrives the manager will create a new network
    structure and then call the NewNetwork() call back function.

    \subsection subsection1_3 Run the network
    Once instantiated and setup the communications manager needs a periodic
    function call in order to service the communications endpoints.  On a
    roughly periodic schedule you should call.

    \code
    m_pComm->RunNetwork();
    \endcode

    The thread which calls the RunNetwork() function is the same thread which
    will receive the call back functions.  A typical period for calling this
    function is 10ms.

    \subsection subsection1_4   Receiving data from the avionics
    Data coming from the avionics are contained in streams, which are contained
    in queues.  As an example, to receive data from the autopilot stream you
    would first get a pointer to the queue:

    \code
    Queue_t* pQ = m_pComm->GetStreamRxBuffer(ID, AUTOPILOT_STREAM);
    \endcode

    Where ID is the network address, and AUTOPILOT_STREAM is the identifier for
    the autopilot stream endpoint.  The queue pointer which is returned will
    persist as long as the Network_t structure exists in the communications
    manager.  Hence you only need call this function once (typically in the
    NewNetwork call back).

    You can now get at the raw data from the stream by using the primitives
    defined for a Queue_t.  Typically you’ll be looking at data from the
    autopilot stream.  In that case you can use the functions defined in the
    module AutopilotPacket:

    \code
    void ExampleFunctionWhichLooksForData(Queue_t* pQ)
    {
        static AutopilotPkt_t Pkt;  // An autopilot packet that will persist between calls

        while(LookForAutopilotPacket(pQ, &Pkt))
            ProcessAPPacket(&Pkt);

    }// ExampleFunctionWhichLooksForData
    \endcode

    Note that the packet structure passed to LookForAutopilotPacket must be
    statically allocated.  This is because LookForAutopilotPacket will only
    return TRUE when a complete packet is available, but that doesn’t mean it
    isn’t storing data in the packet structure while waiting for the rest of
    the packet to arrive.

    Once you have an autopilot packet you can determine its type from the
    PktType member.  This will be one of the defined types in AutopilotPacket.h.
    Knowing the type of the packet you can decode it using the relevant
    function defined in AutopilotPacket.h.

    \subsection subsection1_5 Sending data

    Although the communications manager maintains a separate receive queue for
    each stream on each network it does not maintain transmit queues.  Hence
    each time you send something to an avionics or ground station you must send
    complete packets.  This is by design since it prevents packet collision
    when multiple applications are trying to send data to the ground station
    or avionics.

    To send a raw stream packet you call the function SendStreamPacket(),
    passing it a complete stream packet, including CRC.  However in the case
    of the avionics you will normally send data destined for a specific stream.
    The communications manager includes functions for sending and requesting
    autopilot stream packets.  For example to send a autopilot gains packet:

    \code
    void ExampleSendAutopilotGainsFunction(CCommManager* pComm, UInt16 NetworkID, const Gains_t* pGains)
    {
        pComm->SendGainsPacket(NetworkID, pGains);

    }// ExampleSendAutopilotGainsFunction
    \endcode

    To request a autopilot gains packet:

    \code
    void ExampleRequestAutopilotGainsFunction(CCommManager* pComm, UInt16 NetworkID)
    {
        pComm->RequestGainsPacket(NetworkID);

    }// ExampleRequestAutopilotGainsFunction
    \endcode

    To send data destined for other streams, like the payload stream you would:

    \code
    void ExampleSendPayloadDataFunction(CCommManager* pComm, UInt16 NetworkID, const UInt8* pData, UInt32 Size)
    {
        // Send the payload stream packet as the payload of a generic stream packet
        pComm->SendBlock(NetworkID, PAYLOAD_STREAM, pData, Size);

    }// ExampleSendPayloadDataFunction
    \endcode

 */



#ifndef _COMM_MANAGER_H
#define _COMM_MANAGER_H

#include <string>
#include "CommBase.h"

#ifdef DIGICONNECTME
    #ifdef HAVE_CONFIG_H
    #include <config.h>
    #endif
    // Try to resolve compile problems in updated BSP (min,max 2 arguments vs. 1, in #include <limits>)
    #undef min
    #undef max
#endif


/*! Communications manager class */
class CCommManager
{
// Construction
public:
    CCommManager(const char* ReplayFile, int ListenPort = -1);
    CCommManager(int chan,              const char* ServerAddress = "", int Port = -1, BOOL EnableLogging = TRUE, const char * pIridiumNumber = NULL, const char * pLogPath = NULL, BOOL ListenOnly = FALSE );
    CCommManager(int chan, UInt32 baud, const char* ServerAddress = "", int Port = -1, BOOL EnableLogging = TRUE, const char * pIridiumNumber = NULL, const char * pLogPath = NULL, BOOL ListenOnly = FALSE );
    ~CCommManager(void);

    //! Change, restart, or stop the log file.
    void ChangeLogFile(BOOL EnableLogging = TRUE, const char * pLogPath = NULL);

    //! Change the serial port configuration
    BOOL NewSerialPortConfiguration(int chan, UInt32 baud, const char* pIridiumNumber = NULL, BOOL EnableLogging = TRUE, const char * pLogPath = NULL, BOOL ListenOnly = FALSE);

    //! Change the network client configuration
    BOOL NewNetworkClientConfiguration(const char* ServerAddress, BOOL EnableLogging = TRUE, const char * pLogPath = NULL, BOOL ListenOnly = FALSE);

    //! Change the replay file configuration
    BOOL NewReplayFileConfiguration(const char* ReplayFile);

    //! Change the network server configuration
    BOOL NewNetworkServerConfiguration(int ListenPort);

    //! Return the current serial port parameters
    BOOL GetSerialPortConfiguration(UInt8* chan, UInt32* baud);

    //! Return the current Iridium configuration
    BOOL GetSatCommConfiguration(std::string& Number);

    //! Return the network client parametes
    BOOL GetNetworkClientConfiguration(std::string& ServerAddress);

    //! Return the replay file name
    BOOL GetReplayFileConfiguration(std::string& ReplayFile);

    //! Return the network server configuration
    int  GetNetworkServerConfiguration(void);

    //! Get the version of the communications manager software
    void GetVersionInformation(Version_t* pVersion);

    //! Register the call back function for new networks
    void SetNewNetworkCallBack(void(*NewNetworkCallBack)(UInt16 NetworkID, void* Parameter), void* Parameter);

    //! Get stream receive pointer
    Queue_t* GetStreamRxBuffer(UInt16 NetworkID, UInt8 Stream);

    //! Get stream transmit pointer
    Queue_t* GetStreamTxBuffer(UInt16 NetworkID, UInt8 Stream);

    //! Get user log buffer pointer
    Queue_t* GetUserLogBuffer(void);

    //! Call periodically to service the communications
    void RunNetwork(void);

    //! Send a autopilot packet
    void SendAutopilotPacket(UInt16 NetworkID, const AutopilotPkt_t* pPkt);

    //! Send a zero length autopilot packet
    void SendZeroLengthAutopilotPacket(UInt16 NetworkID, UInt8 PktType);

    //! Send an groundstation packet
    void SendGroundStationPacket(const GroundStationPkt_t* pPkt);

    //! Send a zero length ground station packet
    void SendZeroLengthGroundStationPacket(UInt8 PktType);

    //! Send a gimbal packet on the gimbal stream to the autopilot
    void SendGimbalPacket(UInt16 NetworkID, const GimbalPkt_t* pPkt);

    //! Send a block of data in a packet
    void SendBlock(UInt16 NetworkID, UInt8 Stream, UInt8* pData, UInt32 Size);

    //! Send a low level packet of data
    void SendStreamPacket(StreamPkt_t* pPkt);

    //! Write a block of custom user data to the telemetry file
    void LogUserData(UInt8* pData, UInt32 Size);

    //! Creates a network
    SInt32 CreateNetwork(UInt16 NetworkID);

    //! Destroys a network
    void DestroyNetwork(UInt16 NetworkID);

    //! Destroys all networks
    void DestroyAllNetworks(void);

    //! Get the number of networks that exist
    UInt32 GetNumNets(void);

    //! Get the network IDs that are stored by the comm manager
    SInt32 GetIDFromIndex(UInt32 index);

    //! Get the index into a network from ID
    SInt32 GetIndexFromID(UInt16 NetworkID);

    //! Disable the replay
    void PauseReplay(void);

    //! Enable the replay
    void ResumeReplay(void);

    //! Return the state of the replay mode flag
    BOOL IsInReplay(void);

    //! Return the state of listen only mode flag
    BOOL IsInListenOnly(void);

    //! Get the number of bytes read from the groundstation or from the relplay
    UInt32 GetNumStreamBytes(void);

    //! Get the size of the log or replay file
    UInt32 GetFileSize(void);

    //! Get the location of the file position
    UInt32 GetReplayFileLocation(void);

    //! Set the location of the file position
    void SetReplayFileLocation(UInt32 location);

    //! Set the current replay rate
    void SetReplayRate(UInt32 ReplayRate);

    //! Get the replay rate
    UInt32 GetReplayRate(void);

    //! Get the last error information
    const char* GetLastError(void);

    //! Clear the error string
    void ClearLastError(void);

    //! Compute the downlink percentage for a specific network.*/
    float ComputeDownlinkPercent(UInt16 NetworkID);

    //Satcomm related
    size_t GetSatcommStatusString(char* pString, size_t count);      //!< Get satcomm dialer status string
	size_t GetSatcommErrorString(char* pString, size_t count);      //!< Get satcomm dialer error string
    void HangUpSatcommRedial(void);     //!< Hang up satcomm and redial
    void HangUpSatcommIdle(void);       //!< Hang up satcomm and go to an idle state
    void StartDialingSatcomm(void);     //!< Start the dialing sequence
    void SetSatcommPhoneNumber(const char* pPhoneNum);  //!< Set/change the satcomm phone number

    ///////////////////////////////////////////////////////////////////////////
    // PACKETS FOR A PICCOLO AUTOPILOT
    ///////////////////////////////////////////////////////////////////////////

    //! Create and send a user action command packet
    void SendUserActionPacket(UInt16 NetworkID, UInt8 ActionIndex, BOOL On);

    //! Create and send a configuration lock packet
    void SendConfigurationLockPacket(UInt16 NetworkID, const char Password[32] = NULL);

    //! Create and send a configuration unlock packet
    void SendConfigurationUnlockPacket(UInt16 NetworkID, const char Password[32] = NULL, BOOL Silent = FALSE);

    //! Create and send a controller packet
    void SendControllerDataPacket(UInt16 NetworkID, UInt8 Controller, UInt8 Version, UInt8 Category, UInt8 DataID, UInt8 NumData, const void* pData);

    //! Create and send a controller packet to default data
    void SendControllerDataDefaultPacket(UInt16 NetworkID, UInt8 Controller, UInt8 Version, UInt8 Category, UInt8 DataID, UInt8 NumData);

    //! Create and send a transponder control packet
    void SendTransponderControlPacket(UInt16 NetworkID, UInt16 Code, BOOL Ident, UInt8 Mode);

	//! Create and send a transponder preflight packet
	void SendTransponderPreflightPacket(UInt16 NetworkID, const char FlightID[9]);

	//! Create and send a transponder installation packet
	void SendTransponderInstallationPacket(UInt16 NetworkID, UInt32 ICAOAddress, const char Registration[9]);

    //! Create and send a radio settings packet
    void SendRadioSettingsPacket(UInt16 NetworkID, const RadioSettings_t* pSettings);

    //! Create and send a autopilot loop control packet
    void SendAutopilotLoopControlPacket(UInt16 NetworkID, const AutopilotLoopCmd_t* pCmd, BOOL UpdateEEPROM = TRUE);

    //! Create and send a control actuator calibration packet
    void SendActuatorTablePacket(UInt16 NetworkID, const ActuatorTable_t* pTable, 
                                 UInt8 Sur, UInt8 Type, UInt8 Virtual, UInt8 Fast);

    //! Create and send a packet to change actuator type
    void SendActuatorTypePacket(UInt16 NetworkID, UInt8 Sur, UInt8 Type);

    //! Create and send a sensor orientatin packet
    void SendOrientationPacket(UInt16 NetworkID, const float Euler[NAXIS], const float IMUtoAnt[N3D], const float IMUtoAnt2[N3D]);

    //! Create and send waypoints packet
    void SendWaypointsPacket(UInt16 NetworkID, const FPPoint_t Points[MAX_WAYPOINTS_IN_PACKET], UInt16 Num, BOOL Ack);

    //! Create and send a waypoint list packet
    void SendWaypointListPacket(UInt16 NetworkID, const FPMask_t Mask, UInt8 Flag);

    //! Create and send a waypoint list packet using a range of points
    void SendWaypointListRangePacket(UInt16 NetworkID, UInt16 Start, UInt16 End, UInt8 Flag);

    //! Create and send a legacy waypoint packet
    void SendLegacyWaypointPacket(UInt16 NetworkID, const FPPoint_t* pPoint, UInt8 Index);

    //! Create and send a legacy waypoint list packet
    void SendLegacyWaypointListPacket(UInt16 NetworkID, const FPMask_t Mask, UInt8 Flag);

    //! Create and send a surface test packet
    void SendSurfaceTestPacket(UInt16 NetworkID, float Angle, float Width, float Freq, float Dev, UInt8 Index);

    //! Create and send a mission limits packet
    void SendMissionLimitsPacket(UInt16 NetworkID, const  MissionLimits_t* pMission);

    //! Create and a send a mission settings packet
    void SendMissionSettingsPacket(UInt16 NetworkID, SInt16 MinAltitude, SInt16 MaxAltitude, UInt32 FlightTimeout);

    //! Create and send a airspace boundary packet
    void SendBoundaryPacket(UInt16 NetworkID, const Boundary_t* pBnd);

    //! Create and send a magnetic sensor calibration packet
    void SendMagCalPacket(UInt16 NetworkID, const float HardIronErr[N3D], const float ScaleFactErr[N3D]);

    //! Create and send a sensor error
    void SendSensorErrorPacket(UInt16 NetworkID, const SensorErrorData_t* pSensorError);

    //! Create and send a external serial setup packet
    void SendExternalSerialSetupPacket(UInt16 NetworkID, const ExternalSerial_t* pSerial);

    //! Create and send a external IO setup packet
    void SendExternalIOSetupPacket(UInt16 NetworkID, const ExternalIO_t* pExternalIO, UInt8 pin, UInt32 SampleTime, BOOL UpdateEEPROM = TRUE);

    //! Create and send a simple landing packet
    void SendSimpleLandingPacket(UInt16 NetworkID, const SimpleLandingPattern_t* pLand);

    //! Create and send an altimeter setting packet
    void SendAltimeterSettingPacket(UInt16 NetworkID, UInt32 Altimeter, BOOL ControlToGPS);

    //! Create and send a bandwidth mode packet
    void SendBandwidthModePacket(UInt16 NetworkID, BandwidthMode_t Bandwidth);

    //! Create and send the engine kill packet
    void SendEngineKillPacket(UInt16 NetworkID, BOOL EngineKill);

    //! Create and send the land now command packet.
    void SendLandNowPacket(UInt16 NetworkID);

    //! Create and send the launch now command packet.
    void SendLaunchNowPacket(UInt16 NetworkID);

    //! Create and send the go to prelaunch command packet.
    void SendPrelaunchPacket(UInt16 NetworkID);

    //! Create and send the zero air data command packet.
    void SendAirDataZeroPacket(UInt16 NetworkID, float ActualAltitude);

    //! Create and send the manual assist mode command packet.
    void SendManualAssistModePacket(UInt16 NetworkID, UInt8 Mode);

    //! Create and send the lights command packet.
    void SendLightsCommandPacket(UInt16 NetworkID, BOOL LightsOn);

    //! Create and send the drop command packet.
    void SendDropCommandPacket(UInt16 NetworkID, BOOL DropOn);

    //! Create and send the parachute command packet.
    void SendParachuteCommandPacket(UInt16 NetworkID, BOOL ParachuteDeploy);

    //! Create and send the brakes command packet.
    void SendBrakesCommandPacket(UInt16 NetworkID, float Brakes);

    //! Create and send the throttle trim command packet.
    void SendThrottleTrimPacket(UInt16 NetworkID, float Throttle);

    //! Create and send a track command packet.
    void SendTrackCommandPacket(UInt16 NetworkID, UInt16 Waypoint, BOOL GoTo);

    //! Create and send a parameter description packet.
    void SendDescriptionPacket(UInt16 NetworkID, const char Description[64]);

    //! Create and send a user space packet.
    void SendUserSpacePacket(UInt16 NetworkID, const UInt8 User[64]);

    //! Create and send a doublet command packet
    void SendDoubletCommandPacket(UInt16 NetworkID, const DoubletCmd_t* pDoublet);

    //! Create and send a frequency sweep command packet
    void SendFrequencySweepCommandPacket(UInt16 NetworkID, const FrequencySweepCmd_t* pFreqSwp);

    //! Create and send a set fuel level packet.
    void SendSetFuelLevelPacket(UInt16 NetworkID, float Fuel, BOOL Electric);

    //! Create and send a set fuel level packet for fuel update while locked.
    void SendSetFuelIfLockedPacket(UInt16 NetworkID, float Fuel, BOOL Electric);

    //! Create and send a payload mass packet
    void SendPayloadMassPacket(UInt16 NetworkID, float PayloadMass);

    //! Create and send a throttle min packet
    void SendThrottleMinPacket(UInt16 NetworkID, float Throttle);

    //! Create and send a feature code packet.
    void SendFeatureCodePacket(UInt16 NetworkID, const UInt8 Data[8]);

    //! Create and send terrain elevation information.
    void SendTerrainElevationDataPacket(UInt16 NetworkID, UInt32 Time, float Elevations[6]);

    //! Request some controller data
    void RequestControllerDataPacket(UInt16 NetworkID, UInt8 Controller, UInt8 Version, UInt8 Category, UInt8 DataID, UInt8 NumData);

    //! Request the transponder control packet
    void RequestTransponderControlPacket(UInt16 NetworkID);

	//! Request the transponder preflight packet
    void RequestTransponderPreflightPacket(UInt16 NetworkID);

	//! Request the transponder installation packet
	void RequestTransponderInstallationPacket(UInt16 NetworkID);

    //! Request the radio settings packet
    void RequestRadioSettingsPacket(UInt16 NetworkID);

    //! Request the autopilot surface table calibration packet.
    void RequestActuatorTablePacket(UInt16 NetworkID, UInt8 Sur);

    //! Request the autopilot orientation packet.
    void RequestOrientationPacket(UInt16 NetworkID);

    //! Request the autopilot mission limits packet.
    void RequestMissionLimitsPacket(UInt16 NetworkID);

    //! Request the packet with the magnetometer calibration data.
    void RequestMagCalPacket(UInt16 NetworkID);

    //! Request the packet with the sensor error data
    void RequestSensorErrorPacket(UInt16 NetworkID);

    //! Request the autopilot packet with the external serial setup parameters.
    void RequestExternalSerialSetupPacket(UInt16 NetworkID);

    //! Request then autopilot packet with the setup parameters for a single external IO pin.
    void RequestExternalIOSetupPacket(UInt16 NetworkID, UInt8 pin);

    //! Request the parameter description packet.
    void RequestDescriptionPacket(UInt16 NetworkID);

    //! Request the user space packet.
    void RequestUserSpacePacket(UInt16 NetworkID);

    //! Request the altimeter setting packet.
    void RequestAltimeterSettingPacket(UInt16 NetworkID);

    //! Request the bandwidth mode packet
    void RequestBandwidthModePacket(UInt16 NetworkID, UInt8 NetID);

    //! Request system version packet
    void RequestSystemVersionPacket(UInt16 NetworkID);

    //! Request the raw vibration sensor data
    void RequestRawVibrationDataPacket(UInt16 NetworkID);

    //! Request the payload mass packet
    void RequestPayloadMassPacket(UInt16 NetworkID);

    //! Request the throttle min packet
    void RequestThrottleMinPacket(UInt16 NetworkID);

    ///////////////////////////////////////////////////////////////////////////
    // PACKETS FOR A PICCOLO GROUNDSTATION
    ///////////////////////////////////////////////////////////////////////////

    //! Create and send a ground station add address packet
    void SendGSAddAddressPacket(UInt16 Address);

    //! Create and send a ground station add pilot address packet
    void SendGSAddPilotAddressPacket(UInt16 Pilot);

    //! Create and send a ground station delete address packet
    void SendGSDeleteAddressPacket(UInt16 DelAddress, UInt16 KeepAddress = 0);

    //! Create and a ground station radio settings packet
    void SendGSRadioSettingsPacket(const RadioSettings_t* pSettings);

    //! Create and send a ground station differential corrections packet
    void SendDGPSBasePacket(const DGPSParameter_t* pDGPS);

    //! Create and send a dynamic enumeration on/off packet
    void SendGSDynamicEnumerationPacket(BOOL On);

    //! Create and send a ground station configuration packet
    void SendGSConfiguration(UInt8 ConfigEnum, UInt8 SBASDisable, UInt8 EnableGD, UInt32 Baud, UInt8 EnableMHXRelay);

    //! Request ground station radio settings
    void RequestGSRadioSettingsPacket(void);

    //! Request ground station spectrum sweep, coherent frequency radio
    void RequestGSSpectrumSweepPacket(float StartFreq, float Spacing, UInt16 Dwell, UInt8 NumMeasurements);

    //! Request ground station spectrum sweep, hopping radio
    void RequestGSSpectrumSweepPacket(void);

    //! Request the software and hardware configuration settings from the ground station
    void RequestGSFirmwarePacket(void);

    //! Request the packet with DGPS base settings
    void RequestDGPSBasePacket(void);

    //! Request a sensor point of interest command packet
    void RequestGimbalSPOICmdPacket(UInt16 NetworkID);

    ///////////////////////////////////////////////////////////////////////////
    // PACKETS FOR A TASE GIMBAL CONNECTED TO A PICCOLO AUTOPILOT
    ///////////////////////////////////////////////////////////////////////////

    //! Create and send a gimbal command packet containing pan, tilt, zoom and focus rate, and mode.
    void SendGimbalCmdPacket(UInt16 NetworkID, float Pan, float Tilt, SInt8 ZoomRate, float CamTimeout, UInt8 Flags, float ImpulseTime, SInt8 FocusRate = 0, UInt8 Flags1 = 0);

    //! Create and send a gimbal sensor point of interest packet containing the position of the SPOI
    void SendGimbalSPOICmdPacket(UInt16 NetworkID, const double Pos[NPOS], const float Vel[NDIR], UInt8 Flags);

    //! Create and send a camera command packet
    void SendGimbalCameraCmdPacket(UInt16 NetworkID, UInt8 Camera, float HFOV, UInt16 Focus, UInt8 Mode, UInt8 Shutter, UInt8 Bright, UInt8 Flags);

    //! Create and send a gimbal axis settings packet
    void SendGimbalAxisSettingsPacket(UInt16 NetworkID, const float NoGoCenter[NGIMBAL_SHORT], const float NoGoRange[NGIMBAL_SHORT], const float MaxRate[NGIMBAL_SHORT], const float MaxAccel[NGIMBAL_SHORT], const float PosGain[NGIMBAL_SHORT], const float VelGain[NGIMBAL_SHORT], const float DerGain[NGIMBAL_SHORT], BOOL Temporary = FALSE, BOOL Factory = FALSE);

    //! Create and send gimbal packet rate packet
    void SendGimbalSerialInterfacePacket(UInt16 NetworkID, GimbalSerial_t Serial[6], BOOL Temporary);

    //! Create and send gimbal camera alignment packet
    void SendGimbalCameraAlignPacket(UInt16 NetworkID, float Zoom1, const float Error1[NGIMBAL], float Zoom2, const float Error2[NGIMBAL], UInt8 Camera, BOOL Factory = FALSE);

    //! Create and send gimbal camera definitions packet
    void SendGimbalCameraTypesPacket(UInt16 NetworkID, const CameraDefinition_t Cam[NUM_CAMERAS], const BOOL Send[NUM_CAMERAS], BOOL Factory = FALSE);

    //! Create and send gimbal laser setup packet
    void SendGimbalLaserSetupPacket(UInt16 NetworkID, LaserSetup_t Lasers[NUM_LASERS], BOOL Factory = FALSE);

    //! Create and send gimbal laser arm packet
    void SendGimbalLaserArmPacket(UInt16 NetworkID, UInt8 LaserIndex, BOOL Arm);

    //! Create and send gimbal laser fire packet
    void SendGimbalLaserFirePacket(UInt16 NetworkID, UInt8 LaserIndex, UInt8 Power);

    //! Create and send gimbal retract/deploy command
    void SendGimbalRetractDeployCmdPacket(UInt16 NetworkID, UInt32 Deploy);

    //! Create and send gimbal track command packet
    void SendGimbalTrackCmdPacket(UInt16 NetworkID, float InitialX, float InitialY, UInt8 Algorithm, UInt8 Size, UInt8 Stamp);

    //! Request a camera command packet
    void RequestGimbalCameraCmdPacket(UInt16 NetworkID, UInt8 Camera);

    //! Request a gimbal axis settings packet
    void RequestGimbalAxisSettingsPacket(UInt16 NetworkID, BOOL Factory = FALSE);

    //! Request camera alignment information
    void RequestGimbalCameraAlignPacket(UInt16 NetworkID, UInt8 Camera, BOOL Factory = FALSE);

    //! Request gimbal camera types
    void RequestGimbalCameraTypesPacket(UInt16 NetworkID, BOOL Factory = FALSE);

    //! Request gimbal laser setup
    void RequestGimbalLaserSetupPacket(UInt16 NetworkID, BOOL Factory = FALSE);

    //! Write the header for the log file
    BOOL WriteTextLogHeader(FILE* pFile);

    //! Write a single line of data for the log file
    BOOL WriteTextLogLine(FILE* pFile, const UserData_t* pData, UInt8 AckRatio, const double TrackPosition[N3D] = NULL,
                          const float TrackVelocity[N3D] = NULL, const float ADCSample[NUM_EXTERNAL_ADC_PINS] = NULL,
						  const ECU_t* pLeftECU = NULL, const ECU_t* pRightECU = NULL, const MovingBaseStatus_t* pMBStatus = NULL, 
						  const GPSUser_t* pGSGPS = NULL, float GSRSSI = 0.0f, float GSPilotPercentage = 0.0f, float GSPilotRate = 0.0f);

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
protected:
    CommBase_t m_CommBase; //!< The communication interface data
#endif

};

#endif // _COMM_MANAGER_H
