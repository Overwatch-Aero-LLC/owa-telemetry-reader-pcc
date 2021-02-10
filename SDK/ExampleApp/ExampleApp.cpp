
// Example application used for demonstration of comms library
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

#ifdef WIN32
	#include <conio.h>
#else
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <termios.h>
	#include <unistd.h>
	#include <fcntl.h>
#endif

#include <math.h>

#include "CommManager.h"
#include "GreatCircle.h"
#include "MathUtil.h"
#include "FlightPlan.h"
#include "GSPacket.h"
#include "FixedWingCommonEnum.h"

#ifndef WIN32
# include <sys/select.h>
# include <sys/param.h>

static char _ch = 0;
static int _kbhit(void)
{
    struct termios before, after;
    struct timeval tv;
    fd_set read_fd;
    int result, retval;

    // Clear the stdin fd settings structure
    tv.tv_sec = tv.tv_usec = 0;
    FD_ZERO(&read_fd);
    FD_SET(fileno(stdin), &read_fd);

    // Set the terminal to raw mode
    tcgetattr(fileno(stdin), &before);
    after = before;
    after.c_lflag &= ~(ICANON|ECHO);
    tcsetattr(fileno(stdin), TCSANOW, &after);

    // Get the state of stdin
    result = select(fileno(stdin) + 1, &read_fd, NULL, NULL, &tv);

    // If there is keyboard input, read it in
    if (FD_ISSET(fileno(stdin), &read_fd) && (result != -1))
        retval = read(fileno(stdin), &_ch, 1);
    else
        retval = 0;

    // Reset the terminal to its original state
    tcsetattr(fileno(stdin), TCSANOW, &before);

    // Return 1 if there was data to be read, otherwise 0
    return retval;

}// _kbhit

static char _getch(void)
{
    return _ch;
}

# ifndef getch
#  define getch _getch
#endif
# ifndef kbhit
#  define kbhit _kbhit
# endif
#endif

// Local functions
static void RunCommunications(CCommManager* pComm, FILE* pTextLog);
static void RunGroundCommunications(CCommManager* pComm);
static BOOL ProcessKeyboard(CCommManager* pComm);
static void RegularService(CCommManager* pComm);
static BOOL ProcessAutopilotPacket(AutopilotPkt_t* pPkt, UserData_t* pData, ECU_t* pLeftECU, ECU_t* pRightECU);
static void ProcessGroundStationPacket(GroundStationPkt_t* pPkt, GSUserData_t* pData);
static BOOL ProcessGSGPSHealth(const GroundStationPkt_t* pPkt);
static BOOL ProcessGPSHealth(const AutopilotPkt_t* pPkt);
static BOOL ProcessTelemetry(const AutopilotPkt_t* pPkt, UserData_t* pData);
static void ProcessECUStatus(const AutopilotPkt_t* pPkt, ECU_t* pLeftECU, ECU_t* pRightECU);
static void ProcessSystemStatus(const AutopilotPkt_t* pPkt, UserData_t* pData);
static BOOL CalculateTrackParameters(const UserData_t* pData, double TrackPosition[N3D], float TrackVelocity[N3D]);
static void ProcessExternalIOSample(const AutopilotPkt_t* pPkt);
static void ProcessExternalADCSample(const AutopilotPkt_t* pPkt);
static void ProcessWaypoint(const AutopilotPkt_t* pPkt);
static void ProcessWaypointList(const AutopilotPkt_t* pPkt);
static void ProcessRadioSettings(const AutopilotPkt_t* pPkt);
static void ProcessBandwidthMode(const AutopilotPkt_t* pPkt);
static void ProcessAutopilotLoop(const AutopilotPkt_t* pPkt);
static void ProcessTransponderControl(const AutopilotPkt_t* pPkt);
static void ProcessThrottleTrim(const AutopilotPkt_t* pPkt);
static void ProcessMissionLimits(const AutopilotPkt_t* pPkt);
static void ProcessActuatorTable(const AutopilotPkt_t* pPkt);
static void ProcessSensorOrientation(const AutopilotPkt_t* pPkt);
static void ProcessAltimeterSetting(const AutopilotPkt_t* pPkt);
static void ProcessMagnetometerCalibration(const AutopilotPkt_t* pPkt);
static void ProcessExternalSerialSetup(const AutopilotPkt_t* pPkt);
static void ProcessExternalIOSetup(const AutopilotPkt_t* pPkt);
static void ProcessControllerData(const AutopilotPkt_t* pPkt);
static void ProcessSensorError(const AutopilotPkt_t* pPkt);
static void ProcessResetReport(const AutopilotPkt_t* pPkt);
static void ProcessSystemVersion(const AutopilotPkt_t* pPkt);
static void ProcessDoubletData(const AutopilotPkt_t* pPkt);
static void ProcessRawVibratinoData(const AutopilotPkt_t* pPkt);
static void ProcessLinkBridge(const AutopilotPkt_t* pPkt);
static void ProcessNotImplementedPacket(const AutopilotPkt_t* pPkt);
static void NewNetwork(UInt16 NetworkID, void* Parameter);


// Local data
static UInt16 SerialNumber = 0;

int main(int argc, char* argv[])
{
	CCommManager* pComm = NULL;
	char Server[128] = "localhost:2000";
	char ReplayFile[512] = "";
	FILE* pTextLog = NULL;
	BOOL Replay = FALSE;
	int ListenPort = -1;
	UInt8 Port;
	UInt32 Baud = 57600;
	UInt32 ReplayRate = 2000;

	int i;
	char* param;

	printf("\n\rExample application for piccolo communications developers\n\r\n\r");
	printf("Switches [-P=x] [-B=x] [-SERVER=server:port] [-SN=serialnumber] [-LISTEN=port]\n\r");
	printf("-P=x  the serial port number, default of 1\n\r");
	printf("-B=x  the serial port baud rate, default of 57600\n\r");
	printf("-SERVER=server:port  the piccolo communications server to\n\r");
	printf("                     connect to, defaults to localhost:2000\n\r");
	printf("-LISTEN=port  The IP port to open as a piccolo communciations server\n\r");
	printf("-REPLAY=filename  A binary replay file to use as data source\n\r");
	printf("-REPLAY_RATE=speed  The replay rate in bytes/s.  <-/-> keys change rate\n\r");
	printf("-TEXTLOG=filename  A file to write for text logging of data\n\r");
	printf("-SN=serialnumber  The serial number of the avionics to connect to\n\r");
	printf("                  Defaults to the first avionics detected\n\n\r");

	// parse the command line parameters
	for(i = 1; i < argc; i++)
	{
		param = strstr(argv[i], "-P=");
		if(param)
		{
			param += 3;						// Move forward past parameter name
			Port = (UInt8)atoi(param)-1;	// Get the port number in the parameter; subtract 1 because COM1 is actually 0.
			strcpy(Server,"");				// Disable network communications
			continue;						// Move to next parameter
		}// If found a port number parameter

		param = strstr(argv[i], "-B=");
		if(param)
		{
			param += 3;					// Move forward past parameter name
			Baud = (UInt32)atoi(param);	// Get the baud rate in the parameter
			continue;					// Move to next parameter
		}// If found a baud rate parameter

		param = strstr(argv[i], "-SERVER=");
		if(param)
		{
			param += 8;				// Move forward past parameter name
			strcpy(Server, param);	// Get the server name in the parameter
			continue;				// Move to next parameter
		}// If found a server parameters

		param = strstr(argv[i], "-LISTEN=");
		if(param)
		{
			param += 8;					// Move forward past parameter name
			ListenPort = atoi(param);	// Get the port number in the parameter
			continue;					// Move to next parameter
		}// If found a port number parameter

		param = strstr(argv[i], "-SN=");
		if(param)
		{
			param += 4;							// Move forward past parameter name
			SerialNumber = (UInt16)atoi(param);	// Get the serial number in the parameter
			continue;							// Move to next parameter
		}// If found a port number parameter

		param = strstr(argv[i], "-REPLAY=");
		if(param)
		{
			param += 8;							// Move forward past parameter name
			strcpy(ReplayFile, param);			// Get the replay file name in the parameter
			Replay = TRUE;
			continue;							// Move to next parameter
		}// If found a port number parameter

		param = strstr(argv[i], "-TEXTLOG=");
		if(param)
		{
			param += 9;							// Move forward past parameter name
			pTextLog = fopen(param, "w");		// Open the file
			continue;							// Move to next parameter
		}// If found a port number parameter

		
		param = strstr(argv[i], "-REPLAY_RATE=");
		if(param)
		{
			param += 13;				// Move forward past parameter name
			ReplayRate = atoi(param);	// Get the replay rate in the parameter
			continue;					// Move to next parameter
		}// If found a port number parameter

	}// for all parameters

	// Instantiate the communications manager, logging is on
	if(Replay)
		pComm = new CCommManager(ReplayFile, ListenPort);
	else
		pComm = new CCommManager(Port, Baud, Server, ListenPort);
	
	// If object construction was successful
	if(pComm)
	{
		if(pComm->GetLastError())
			printf("%s", pComm->GetLastError());
		else
		{
			// If we are doing a replay then tell the speed
			if(Replay)
				pComm->SetReplayRate(ReplayRate);

			// Register new avionics callback function
			pComm->SetNewNetworkCallBack(NewNetwork, 0);

			// Run the main communications function
			RunCommunications(pComm, pTextLog);
		
		}// If no errors in the comm manager

		delete pComm;

	}// If created the communications manager
	else
		printf("Failed to create comm manager!\r\n");

	return 0;
}// main


/*! This is the main function that runs indefinitely, servicing communications.
 *  \param pComm points to a correctly instantiated communications manager.
 *  \param pTextLog points to the text logging file to use, if NULL no logging
 *         will be done*/
void RunCommunications(CCommManager* pComm, FILE* pTextLog)
{
	// Pointer to receive buffers
	Queue_t* pRxQ = NULL;
	Queue_t* pGSRxQ = NULL;
	
	// Place holder for incoming AUTOPILOT_STREAM data.  This memory storage
	//   location must persiste between service calls.
	AutopilotPkt_t Pkt;
	UserData_t Data;
	ECU_t LeftECU, RightECU;

	// Place holder for incoming ground station data.  This memory storage 
	//   location must persist between service calls
	GroundStationPkt_t GSPkt;
	GSUserData_t GSData;	

	// Initialize all fields to zero
	memset(&Pkt, 0, sizeof(Pkt));
	memset(&Data, 0, sizeof(Data));
	memset(&LeftECU, 0, sizeof(ECU_t));
	memset(&RightECU, 0, sizeof(ECU_t));
	memset(&GSPkt, 0, sizeof(GSPkt));
	memset(&GSData, 0, sizeof(GSData));

	// The SerialNumber variable is global, it will change when the
	//   NewNetwork callback function fires.  That is how we will know
	//   that we have detected an avionics.
	if(SerialNumber == 0)
	{
		printf("Waiting to detect first avionics\n\r");
		printf("Press Q to quit\n\r");

		while(SerialNumber == 0)
		{
			// Serivce comms and remainder of OS
			RegularService(pComm);

			// see if the user wants to quit
			if(_kbhit())
				return;

		}// while still waiting to detect a network

		printf("Connected to %u\r\n", SerialNumber);

	}// If user didn't specify an explicit serial number
	else
	{
		pComm->CreateNetwork(SerialNumber);
		printf("Press Q to quit\n\r");
		printf("Forced connection to %u\n\r", SerialNumber);

	}// If user did specify a serial number

	printf("Press P to pause\n\n\r");

	if(pTextLog)
		pComm->WriteTextLogHeader(pTextLog);

	// If we get here then the comm manager is up and running and all the
	//    network structures are built, start moving data
	pRxQ = pComm->GetStreamRxBuffer(SerialNumber, AUTOPILOT_STREAM);
	while(1)
	{
		// Serivce comms and remainder of OS
		RegularService(pComm);

		if(!pRxQ)
			pRxQ = pComm->GetStreamRxBuffer(SerialNumber, AUTOPILOT_STREAM);
		else
		{
			// Look for autopilot packets and process them
			while(LookForAutopilotPacket(pRxQ, &Pkt))
			{
				if(ProcessAutopilotPacket(&Pkt, &Data, &LeftECU, &RightECU))
				{
					// Get the ack ratio
					UInt8 AckRatio = 0;
					UInt32 i;
					for(i = 0; i < GSData.NumAddresses; i++)
					{
						if(GSData.Addresses[i] == SerialNumber)
						{
							AckRatio = GSData.AckRatio[i];
							break;
						}
                    }

					if(pTextLog)
						pComm->WriteTextLogLine(pTextLog, &Data, AckRatio, NULL, NULL, NULL, &LeftECU, &RightECU, NULL, NULL, 
							GSData.System.RSSI, GSData.PilotSamplePercentage, GSData.PilotUplinkRate);

				}// If decoded telemetry packet

			}// if found autopilot packet

		}// else if we have receive queue buffer

		if(!pGSRxQ)
			pGSRxQ = pComm->GetStreamRxBuffer(GS_ADDRESS, GS_STREAM);
		else
		{
			// Look for ground station packets and process them
			while(LookForGroundStationPacket(pGSRxQ, &GSPkt))
				ProcessGroundStationPacket(&GSPkt, &GSData);	
		}


		// Take care of any user input
		if(ProcessKeyboard(pComm))
			return;

	}// while

}// RunCommunications


/*! This function encapsulates the regular procesing that must be doen to
 *  service low level communciations and to service the host operating
 *  system.  It will sleep the system for 10 ms on each call.
 *  \param pComm points to a correctly instantiated communications manager.*/
void RegularService(CCommManager* pComm)
{
	// Sleep to let the rest of the system run
	#ifdef WIN32
	Sleep(10);
	#else
	usleep(10000);
	#endif

	// Run the network, servicing the low leve of the communications
	//   subsystem.  This must be called at a reasonable rate to prevent
	//   buffer build up
	pComm->RunNetwork();

}// RegularService


/*! This function handles keyboard input from the user.
 *  \param pComm points to a correctly instantiated communications manager.
 *  \return TRUE if the application should exit (user pushed 'q')*/
BOOL ProcessKeyboard(CCommManager* pComm)
{
	while(_kbhit())
	{
		int key = _getch();
		switch(key)
		{
		case 'Q':
		case 'q':
			return TRUE;
			break;

		case 'P':
		case 'p':
			printf("Press any key to continue...");
			while(!_kbhit())
			{
				// Just sit here spinning wheels until the next keyboard hit
				//    Keep calling RunNetwork() so that comms layer is serviced
				if(!pComm->IsInReplay())
					RegularService(pComm);
			}
			// clear the character used to get us out of the loop
			_getch();
			printf("\n\r");
			break;

		case 224:	// arrow keys
			{
				UInt32 Rate = pComm->GetReplayRate();
				
				switch(_getch())
				{
				case 77: // right arrow
					Rate *= 2; // double rate
					break;

				case 75: // left arrow

					// Half rate, but not to zero
					Rate = Rate>>1;
					if(Rate == 0)
						Rate = 1;

					break;

				}// switch on arrow keys

				// Set the new rate
				pComm->SetReplayRate(Rate);

				// inform the user
				printf("\nReplay rate chagned to %u\n\r", Rate);
			}
			break;

		}// switch

	}// if the keyboard was hit

	return FALSE;

}// ProcessKeyboard


/*! Decode a ground station packet.
*  \param pPkt points to valid autopilot packet received from an avionics.
*  \param pData points to space to store information coming from the packets.*/
void ProcessGroundStationPacket(GroundStationPkt_t* pPkt, GSUserData_t* pData)
{
	switch(pPkt->PktType)
	{
	case GROUP_ADDRESS:	// Regular packet indicating list of addresses
		DecodeGroupAddressPacketNew(pPkt, pData);
		break;

	case GS_OLD_TELEMETRY:	// Ground station telemetry
    case GS_NEW_TELEMETRY:	// Ground station telemetry
		DecodeGSTelemetryPacket(pPkt, pData);
		break;

	case GS_GPS_HEALTH:
		ProcessGSGPSHealth(pPkt);
        break;
	}// switch

}// ProcessGroundStationPacket


BOOL ProcessGSGPSHealth(const GroundStationPkt_t* pPkt)
{
    GPSUser_t GPS;
    UInt8 Index, Num;
    BOOL Return = DecodeGSGPSHealth(pPkt, &GPS, &Index, &Num);
    int i;

    if(Return)
    {
        printf("Groundstation ");

        if(GPS.UsedInINS)
            printf("Primary GPS ");

        if(GPS.GPSType == UBLOX_GPSTYPE)
            printf("uBlox");
        else if(GPS.GPSType == NOV_GPSTYPE)
            printf("Novatel");
        else if(GPS.GPSType == GSSIP_GPSTYPE)
            printf("GSSIP");
        else if(GPS.GPSType == M12_GPSTYPE)
            printf("M12");
        else
            printf("Unknown ");

        printf(" GPS Health packet %d of %d\n\r", (int)(Index+1), (int)Num);
        
        // GPS information
		printf("Pos: %0.6lf %0.6lf %0.2lf Sats:%d PDOP:%0.1f", GPS.Latitude*180.0/PI,	// radians to degrees
															   GPS.Longitude*180.0/PI,	// radians to degrees
															   GPS.Altitude,			// meters
															   (int)GPS.TrackedSats,
															   GPS.PDOP);

        // Satellite data
        // Print the space vehicle IDs
        printf("\n\n\rSVID ");
        for(i = 0; i < NUM_SATS; i++)
        {
            if(GPS.Sat[i].SVID > 0)
                printf("%3d ", (int)GPS.Sat[i].SVID);
        }

        printf("\n\rL1CNo");
        for(i = 0; i < NUM_SATS; i++)
        {
            if(GPS.Sat[i].SVID > 0)
                printf("%3d ", (int)GPS.Sat[i].L1CNo);
        }

        printf("\n\rL2CNo");
        for(i = 0; i < NUM_SATS; i++)
        {
            if(GPS.Sat[i].SVID > 0)
                printf("%3d ", (int)GPS.Sat[i].L2CNo);
        }

        printf("\n\rAz   ");
        for(i = 0; i < NUM_SATS; i++)
        {
            if(GPS.Sat[i].SVID > 0)
            {
            	if(GPS.Sat[i].Geometry)
	                printf("%3d ", (int)GPS.Sat[i].Azimuth);
	            else
	                printf(" NA ");
	        }
        }
        
        printf("\n\rEl   ");
        for(i = 0; i < NUM_SATS; i++)
        {
            if(GPS.Sat[i].SVID > 0)
            {
            	if(GPS.Sat[i].Geometry)
	                printf("%3d ", (int)GPS.Sat[i].Elevation);
	            else
	                printf(" NA ");
	        }
	    }        
		
        printf("\n\rUsed ");
        for(i = 0; i < NUM_SATS; i++)
        {
            if(GPS.Sat[i].SVID > 0)
            {
                if(GPS.Sat[i].Used)
                    printf("Yes ");
                else
                    printf(" No ");
            }
        }

        printf("\n\n\r");
    }

    return Return;

}// ProcessGSGPSHealth


/*! This is the primary workhorse function that determines what a received
 *  autopilot packet is and calls the appropriate helper function to
 *  interpret it.
 *  \param pPkt points to valid autopilot packet received from an avionics.
 *  \param pData points to space to store information coming from the main
 *         telemetry packets, TELEMETRY and SYSTEM_STATUS.
 *  \return TRUE if a telemetry packet was decoded correctly.*/
BOOL ProcessAutopilotPacket(AutopilotPkt_t* pPkt, UserData_t* pData, ECU_t* pLeftECU, ECU_t* pRightECU)
{
	switch(pPkt->PktType)
	{
    default:
        ProcessNotImplementedPacket(pPkt);
        break;

		///////////////////////////////////////////////////////////////////////
		// Regular telemetry packets
		///////////////////////////////////////////////////////////////////////

    case GPS_HEALTH:
        return ProcessGPSHealth(pPkt);
        break;

	case TELEMETRY_HI_RES:		//!< High resolution telemetry
	case TELEMETRY_LO_RES:		//!< Low resolution telemetry
		return ProcessTelemetry(pPkt, pData);
		break;

	case SYSTEM_STATUS:
		ProcessSystemStatus(pPkt, pData);
		break;

	case EXTERNAL_IO_SAMPLE:	//!< External IO input sample
		ProcessExternalIOSample(pPkt);
		break;

	case EXTERNAL_ADC_SAMPLE:	//!< External analog input sample
		ProcessExternalADCSample(pPkt);
		break;

	case ECU_STATUS:
		ProcessECUStatus(pPkt, pLeftECU, pRightECU);
		break;

		///////////////////////////////////////////////////////////////////////
		// Commands
		///////////////////////////////////////////////////////////////////////

	case WAYPOINTS:			//!< Waypoint data
		ProcessWaypoint(pPkt);
		break;

	case WAYPOINT_LIST_RANGE://!< List of waypoint indices
		ProcessWaypointList(pPkt);
		break;

	case TRACK:				//!< Tracker command
		printf("Tracker command acknowledge, waypoint:%d\n\r", (int)pPkt->Data[0]);
		break;

	case RADIO_SETTINGS:	//!< Radio settings
		ProcessRadioSettings(pPkt);
		break;

	case BANDWIDTH_MODE:	//!< Bandwidth mode request
		ProcessBandwidthMode(pPkt);
		break;

	case AUTOPILOT_LOOP:	//!< Individual autopilot loop control
		ProcessAutopilotLoop(pPkt);
		break;

	case TRANSPONDER_CONTROL://!< Transponder control
		ProcessTransponderControl(pPkt);
		break;

	case THROTTLE_TRIM:		//!< Set throttle trim
		ProcessThrottleTrim(pPkt);
		break;

		///////////////////////////////////////////////////////////////////////
		// Setup
		///////////////////////////////////////////////////////////////////////

	case MISSION_LIMITS:	//!< Mission Limits
		ProcessMissionLimits(pPkt);
		break;

	case ACTUATOR_TABLE:	//!< Control surface calibration
		ProcessActuatorTable(pPkt);
		break;

	case SENSOR_ORIENTATION://!< Avionics installation orientation
		ProcessSensorOrientation(pPkt);
		break;

	case ALTIMETER_SETTING:	//!< Altimeter base pressure setting
		ProcessAltimeterSetting(pPkt);
		break;

	case MAG_CALIBRATION:	//!< Magnetometer calibration
		ProcessMagnetometerCalibration(pPkt);
		break;

	case EXTERNAL_SERIAL_SETUP:	//!< External serial port setup
		ProcessExternalSerialSetup(pPkt);
		break;

	case EXTERNAL_IO_SETUP:	//!< External IO setup
		ProcessExternalIOSetup(pPkt);
		break;

	case CONTROLLER_DATA:	//!< Controller data
		ProcessControllerData(pPkt);
		break;

	case SENSOR_ERROR:		//!< Set the sensor errors
		ProcessSensorError(pPkt);
		break;

		///////////////////////////////////////////////////////////////////////
		// Miscellanious
		///////////////////////////////////////////////////////////////////////

	case RESET_REPORT:		//!< Reset report packet
		ProcessResetReport(pPkt);
		break;

	case SYSTEM_VERSION:	//!< System hardware and firmware version
		ProcessSystemVersion(pPkt);
		break;

	case DOUBLET_AP_DATA:	//!< Returns autopilot data for a doublet
		ProcessDoubletData(pPkt);
		break;

	case DOUBLET_SENSOR_DATA://!< Returns high speed sensor data for a doublet
		ProcessDoubletData(pPkt);
		break;

	case RAW_VIBRATION_DATA://!< Raw vibration data, 1024 samples at 1khz, 6 sensros
		ProcessRawVibratinoData(pPkt);
		break;

	case LINK_BRIDGE:		//!< Bridging data from link to link
		ProcessLinkBridge(pPkt);
		break;

	case DESCRIPTION:		//!< Description field in EEPROM
		if(pPkt->Size == 0)
			printf("Invalid downlink packet, DESCRIPTION request\n\r");
		else
		{
			// Make sure the string terminates
			pPkt->Data[64] = 0;

			// Print out the description
			printf("DESCRIPTION packet contents: %s\n\r", pPkt->Data);
		}
		break;

	case USER_SPACE:		//!< User data space in EEPROM
		if(pPkt->Size == 0)
			printf("Invalid downlink packet, USER_SPACE request\n\r");
		else
			printf("USER_SPACE packet, size %d\n\r", (int)pPkt->Size);
		break;

		///////////////////////////////////////////////////////////////////////
		// Packets that should not appear on the downlink (uplink only)
		///////////////////////////////////////////////////////////////////////

	case SYSTEM_RESET:		//!< System reset command
		printf("Invalid downlink packet, SYSTEM_RESET\n\r");
		break;

	case ACTUATOR_TEST:		//!< Control actuator test
		printf("Invalid downlink packet, ACTUATOR_TEST\n\r");
		break;

	case ENGINE_KILL:		//!< Engine kill command
		printf("Invalid downlink packet, ENGINE_KILL\n\r");
		break;

	case RADIO_TRANSMIT_DISABLE://!< Transmit disable for radio systems
		printf("Invalid downlink packet, RADIO_TRANSMIT_ENABLE\n\r");
		break;

	case CREATE_SIMPLE_LANDING:	//!< Directs the navigator to create a simple landing pattern flightplan
		printf("Invalid downlink packet, CREATE_SIMPLE_LANDING\n\r");
		break;

	case AIR_DATA_ZERO:		//!< Zero just the air data sensors
		printf("Invalid downlink packet, AIR_DATA_ZERO\n\r");
		break;

	case DOUBLET_CMD:		//!< Command for initiating a doublet maneuver
		printf("Invalid downlink packet, DOUBLET_CMD\n\r");
		break;

	case LAND_NOW:			//!< The command to autoland the aircraft using the default landing waypoint
		printf("Invalid downlink packet, LAND_NOW\n\r");
		break;

	case LAUNCH_NOW:		//!< Launch now command
		printf("Invalid downlink packet, LAUNCH_NOW\n\r");
		break;

	case PRE_LAUNCH_MODE:	//!< Go to prelaunch mode
		printf("Invalid downlink packet, PRE_LAUNCH_MODE\n\r");
		break;

	case NEW_MANUAL_ASSIST_MODE://!< Manual assist mode command
		printf("Invalid downlink packet, NEW_MANUAL_ASSIST_MODE\n\r");
		break;

	case LIGHTS_CMD:		//!< Lights command
		printf("Invalid downlink packet, LIGHTS_CMD\n\r");
		break;

	case BRAKES_CMD:		//!< Brakes command
		printf("Invalid downlink packet, BRAKES_CMD\n\r");
		break;

	case PARACHUTE_CMD:		//!< Parachute command
		printf("Invalid downlink packet, PARACHUTE_CMD\n\r");
		break;

	case DROP_CMD:			//!< Drop command
		printf("Invalid downlink packet, DROP_CMD\n\r");
		break;

	case CONTROLLER_DATA_REQUEST://!< Request controller data
		printf("Invalid downlink packet, CONTROLLER_DATA_REQUEST\n\r");
		break;

	case CONTROLLER_DATA_DEFAULT://!< Default controller data
		printf("Invalid downlink packet, CONTROLLER_DATA_DEFAULT\n\r");
		break;

	case SET_FUEL_LEVEL:	//!< Packet used to set the fuel level
		printf("Invalid downlink packet, SET_FUEL_LEVEL\n\r");
		break;

	case CONTROLLER_ABORT:	//!< Issue an abort command to the controller
		printf("Invalid downlink packet, CONTROLLER_ABORT\n\r");
		break;

	}// switch on packet type

	return FALSE;

}// ProcessAutopilotPacket


BOOL ProcessGPSHealth(const AutopilotPkt_t* pPkt)
{
    GPSUser_t GPS;
    UInt8 Index, Num;
    BOOL Return = DecodeGPSHealth(pPkt, &GPS, &Index, &Num);
    int i;

    if(Return)
    {
        if(GPS.UsedInINS)
            printf("Used in INS ");

        if(GPS.GPSType == UBLOX_GPSTYPE)
            printf("uBlox");
        else if(GPS.GPSType == NOV_GPSTYPE)
            printf("Novatel");
        else if(GPS.GPSType == GSSIP_GPSTYPE)
            printf("GSSIP");
        else
            printf("Unknown");

        printf(" GPS Health packet %d of %d\n\r", (int)(Index+1), (int)Num);
        
        // GPS information
		printf("Pos: %0.6lf %0.6lf %0.2lf Sats:%d PDOP:%0.1f", GPS.Latitude*180.0/PI,	// radians to degrees
															   GPS.Longitude*180.0/PI,	// radians to degrees
															   GPS.Altitude,			// meters
															   (int)GPS.TrackedSats,
															   GPS.PDOP);

        // Satellite data
        // Print the space vehicle IDs
        printf("\n\n\rSVID ");
        for(i = 0; i < NUM_SATS; i++)
        {
            if(GPS.Sat[i].SVID > 0)
                printf("%3d ", (int)GPS.Sat[i].SVID);
        }

        printf("\n\rL1CNo");
        for(i = 0; i < NUM_SATS; i++)
        {
            if(GPS.Sat[i].SVID > 0)
                printf("%3d ", (int)GPS.Sat[i].L1CNo);
        }

        printf("\n\rL2CNo");
        for(i = 0; i < NUM_SATS; i++)
        {
            if(GPS.Sat[i].SVID > 0)
                printf("%3d ", (int)GPS.Sat[i].L2CNo);
        }

        printf("\n\rAz   ");
        for(i = 0; i < NUM_SATS; i++)
        {
            if(GPS.Sat[i].SVID > 0)
            {
            	if(GPS.Sat[i].Geometry)
	                printf("%3d ", (int)GPS.Sat[i].Azimuth);
	            else
	                printf(" NA ");
	        }
        }
        
        printf("\n\rEl   ");
        for(i = 0; i < NUM_SATS; i++)
        {
            if(GPS.Sat[i].SVID > 0)
            {
            	if(GPS.Sat[i].Geometry)
	                printf("%3d ", (int)GPS.Sat[i].Elevation);
	            else
	                printf(" NA ");
	        }
	    }        
		
        printf("\n\rUsed ");
        for(i = 0; i < NUM_SATS; i++)
        {
            if(GPS.Sat[i].SVID > 0)
            {
                if(GPS.Sat[i].Used)
                    printf("Yes ");
                else
                    printf(" No ");
            }
        }

        printf("\n\n\r");
    }

    return Return;

}// ProcessGPSHealth


/*! This is the helper function that processes a TELEMETRY_HI_RES or
 *  TELEMETRY_LO_RES packet and displays information about it.  TELEMETRY
 *  packets arrive regularly from the autopilot, typically once per second
 *  but potentially faster or slower.  This packet and the SYSTEM_STATUS
 *  packet represent the entire unsolicted data set from the autopilot.
 *  \param pPkt points to valid autopilot packet received from an avionics.
 *  \param pData points to space to store information coming from the packet.
 *  \return TRUE if the packet decoded correctly.*/
BOOL ProcessTelemetry(const AutopilotPkt_t* pPkt, UserData_t* pData)
{
	BOOL Return;

	// Telemetry data can arrive in either high or low resolution forms.  The
	//   data contained is the same in either case, only the quality varies.
	if(pPkt->PktType == TELEMETRY_HI_RES)
		Return = DecodeTelemetryHiResPacket(pPkt, pData);
	else
		Return = DecodeTelemetryLoResPacket(pPkt, pData);

	if(Return)
	{
		if(pPkt->PktType == TELEMETRY_HI_RES)
			printf("High resolution TELEMETRY packet\n\r");
		else
			printf("Low resolution TELEMETRY packet\n\r");

		// There is a lot of information in this packet.  Everything except
		//   System, Nav, and autpilot status.  It would be impractical to
		//   print it all out so go look in the communications specification
		//   and the definition of UserData_t to get an idea of what is here.

		// GPS information (actual data from the GPS/INS)
		printf("Pos: %0.6lf %0.6lf %0.2lf Sats:%d PDOP:%0.1f %d:%d:%0.2f\n\r", pData->GPS.Latitude*180.0/PI,	// radians to degrees
																			   pData->GPS.Longitude*180.0/PI,	// radians to degrees
																			   pData->GPS.Altitude,				// meters
																			   (int)pData->GPS.TrackedSats,
																			   pData->GPS.PDOP,
																			   (int)pData->GPS.hours,
																			   (int)pData->GPS.minutes,
																			   pData->GPS.seconds);
		// Velocity and body acceleration
		printf("Clock:%u Vel:%0.2f %0.2f %0.2f Accel:%0.2f %0.2f %0.2f\n\r", pData->SystemTime,	// milliseconds
																	pData->GPS.VNorth,		// meters per second
																	pData->GPS.VEast,		// meters per second
																	pData->GPS.VDown,		// meters per second
																	pData->Accel[X_AXIS],	// m/s/s
																	pData->Accel[Y_AXIS],	// m/s/s
																	pData->Accel[Z_AXIS]);	// m/s/s
		// Attitude and body rate
		printf("Att:%0.2f %0.2f %0.2f Gyro:%0.2f %0.2f %0.2f\n\r", pData->Euler[ROLL_AXIS]*180.0/PI,
																	pData->Euler[PITCH_AXIS]*180.0/PI,
																	pData->Euler[YAW_AXIS]*180.0/PI,
																	pData->Gyro[ROLL_AXIS]*180.0/PI,
																	pData->Gyro[PITCH_AXIS]*180.0/PI,
																	pData->Gyro[YAW_AXIS]*180.0/PI);
		// Altitude, Airspeed, RPM.  Notice how we combine the
		//   two RPM readings into a single display
		float RPM = pData->LeftRPM;
		if((pData->LeftRPM > 100) && (pData->RightRPM > 100))
			RPM = (pData->LeftRPM + pData->RightRPM)*0.5f;
		else if(pData->RightRPM > 100)
			RPM = pData->RightRPM;
		printf("Baro:%0.2f IAS:%0.2f RPM:%0.0f\n\r", pData->BaroAlt, pData->IAS, RPM);

		// Limits information.
		if(pData->Limits.AltitudeHigh) printf("Alt High!\n\r");
		else if(pData->Limits.AltitudeLow) printf("Alt Low!\n\r");
		if(pData->Limits.IASHigh) printf("Airspeed High!\n\r");
		else if(pData->Limits.IASLow) printf("Airspeed Low!\n\r");
		if(pData->Limits.RollHigh) printf("Roll Angle High!\n\r");
		if(pData->Limits.PitchHigh) printf("Pitch Angle High!\n\r");
		if(pData->Limits.RPMHigh) printf("RPM High!\n\r");
		else if(pData->Limits.RPMLow) printf("RPM Low!\n\r");
		if(pData->Limits.WindsHigh) printf("Winds High!\n\r");
	}
	else
		printf("Invalid TELEMETRY packet\n\r");

	printf("\n\r");

	return Return;

}// ProcessTelemetry


/*! This is the helper function that processes a ECU_STATUS packet.
 *  \param pPkt points to valid autopilot packet received from an avionics.
 *  \param pData points to space to store information coming from the packet.*/
void ProcessECUStatus(const AutopilotPkt_t* pPkt, ECU_t* pLeftECU, ECU_t* pRightECU)
{
	ECU_t ECU;

	if(DecodeECUStatusPacket(pPkt, &ECU))
	{
		if(ECU.Flags.IsRight)
		{
			memcpy(pRightECU, &ECU, sizeof(ECU_t));
			printf("Right ECU status packet\n\r");
		}
		else
		{
			memcpy(pLeftECU, &ECU, sizeof(ECU_t));
			printf("Left ECU status packet\n\r");
		}
	}

}// ProcessECUStatus


/*! This is the helper function that processes a SYSTEM_STATUS_HI_RES or
 *  SYSTEM_STATUS_LO_RES packet and displays information about it.  SYSTEM_STATUS
 *  packets arrive regularly from the autopilot, typically every other second
 *  but potentially faster or slower.  SYSTEM_STATUS packets have a long and
 *  short form.  The long form typically arrives every six seconds.  This packet
 *  and the TELEMETRY packet represent the entire unsolicted data set from the
 *  autopilot.
 *  \param pPkt points to valid autopilot packet received from an avionics.
 *  \param pData points to space to store information coming from the packet.*/
void ProcessSystemStatus(const AutopilotPkt_t* pPkt, UserData_t* pData)
{
	BOOL Return, Long;

	// Telemetry data can arrive in either high or low resolution forms.  The
	//   data contained is the same in either case, only the quality varies.
	if(pPkt->PktType == SYSTEM_STATUS)
		Return = DecodeSystemStatusPacket(pPkt, &pData->System, &pData->Nav, &pData->AP, &Long);

	if(Return)
	{
		printf("SYSTEM_STATUS packet, ");
		
		// The long format of the packet ccontains the current track data,
		//   the loop targets, and the loop limits.  It does not arrive as
		//   fast because this data don't change quickly.
		if(Long)
			printf("long format\n\r");
		else
			printf("short format\n\r");

		// Print out the system status information
		printf("Vin:%0.2f Pow:%0.2f Sin:%0.2f Temp:%0.1f RSSI:%0.0f\n\r", pData->System.InputVoltage,
																		pData->System.InputVoltage*pData->System.InputCurrent,
																		pData->System.ServoVoltage,
																		pData->System.BoardTemp + CELSIUS_OFFSETf,	// degrees C
																		pData->System.RSSI);		// dbm
		// Print out any alarm bits
		if(pData->System.FirstStageBIT)  printf("Main input converter bad!\n\r");
		if(pData->System.FiveDigitalBIT) printf("5 volt digital bad!\n\r");
		if(pData->System.FiveAnalogBIT)	 printf("5 volt analog bad!\n\r");
		if(pData->System.CPUVoltageBIT)  printf("CPU voltage bad!\n\r");
		if(pData->System.GPSVoltageBIT)  printf("GPS voltage bad!\n\r");

		// Print out navigation diagnostic information.  This data come
		//   from the GPS/INS Kalman Filter
		switch(pData->Nav.NavStatus.Mode)
		{
		case FILTER_INIT:				//!< The filter is initializing
			printf("Nav:Init\n\r");
			break;
		case FILTER_BUFFERING:			//!< The filter is buffering data to start
			printf("Nav:Buffer\n\r");
			break;
		case FILTER_AHRS:				//!< The filter is in AHRS mode (no GPS, INS-only)
		case FILTER_BOUND_VEL:
			printf("Nav:AHRS\n\r");
			break;
		case FILTER_NORMAL:				//!< The filter is in GPS/INS mode
			printf("Nav:GPS/INS\n\r");
			break;
		case FILTER_ALIGN:
			printf("Nav:GPS/INS\n\r");	//!< The filter is in GPS/INS mode, aligning
		default:
			printf("Nav:UKNOWN!\n\r");
		}

		// Indicate any failure bits for the nav system
		if(!pData->Nav.NavStatus.PositionGood)	printf("Position bad\n\r");		// Set if GPS position measurement is good
		if(!pData->Nav.NavStatus.VelocityGood)	printf("Velocity bad\n\r");		// Set if GPS velocity measurement is good
		if(!pData->Nav.NavStatus.BaroGood)		printf("Barometer bad\n\r");	// Set if baro altitude measurement is good
		if(!pData->Nav.NavStatus.TASGood)		printf("Airspeed bad\n\r");		// Set if true airspeed measurement is good
		if(!pData->Nav.NavStatus.HeadingGood)	printf("Heading bad\n\r");		// Set if heading measurement is good
		if(!pData->Nav.NavStatus.AttitudeGood)	printf("Attitude bad\n\r");		// Set if attitude estimates are good
		if(!pData->Nav.NavStatus.GyroBiasGood)	printf("Gyro bias bad\n\r");	// Set if gyro bias estimates are good
		if(!pData->Nav.NavStatus.AccBiasGood)	printf("Accel bias bad\n\r");	// Set if accelerometer bias estimates are good
		if(!pData->Nav.NavStatus.MagBiasGood)	printf("Mag bias bad\n\r");		// Set if magnetic field bias estimates are good
		if(!pData->Nav.NavStatus.WindGood)		printf("Winds bad\n\r");		// Set if wind speed estimates are good

		// These two bits will always be bad unless the actual hardware is hooked up
		//if(!pData->Nav.NavStatus.AGLGood)		printf("AGL bad\n\r");			// Set if AGL measurement is good
		//if(!pData->Nav.NavStatus.MagGood)		printf("Magnetometer bad\n\r");	// Set if magnetometer measurement is good

		// Fixed wing states
		switch(pData->AP.APMode)
		{
		case AP_PRE_LAUNCH_STATE:
			printf("State: Pre launch\n\r");
			break;
		case AP_TRANSITION_STATE:
			printf("State: Transition\n\r");
			break;
		case AP_LIFTOFF_STATE:
			printf("State: Liftoff\n\r");
			break;
		case AP_CLIMBOUT_STATE:
			printf("State: Climbout\n\r");
			break;
		case AP_FLYING_STATE:
			printf("State: Flying\n\r");
			break;
		case AP_LAND_PATTERN_STATE:
			printf("State: Landing\n\r");
			break;
		case AP_FINAL_APPROACH_STATE:
			printf("State: Final approach\n\r");
			break;
		case AP_SHORT_FINAL_STATE:
			printf("State: Short final approach\n\r");
			break;
		case AP_TOUCHDOWN_STATE:
			printf("State: Touchdown\n\r");
			break;
		case AP_ROLLOUT_STATE:
			printf("State: Rollout\n\r");
			break;
		}


		if(Long)
		{
			double TrackPosition[N3D];
			float TrackVelocity[N3D];

			// Only the long format of the packet contains this data

			// We wish to calculate track relative information for display
			//   this is really just vector math, and all the math is in
			//   the library so don't freak out.
			if(CalculateTrackParameters(pData, TrackPosition, TrackVelocity))
			{
				printf("Track Waypoint:%d ", (int)pData->AP.WaypointTo);
				if(pData->AP.Orbit) printf("Orbit ON\n\r");
				else				printf("Orbit OFF\n\r");
				printf("Track Pos: %0.1lf %0.1lf %0.1lf\n\r", TrackPosition[X_AXIS], TrackPosition[Y_AXIS], TrackPosition[Z_AXIS]);
				printf("Track Vel: %0.1f %0.1f %0.1f\n\r", TrackVelocity[X_AXIS], TrackVelocity[Y_AXIS], TrackVelocity[Z_AXIS]);
			}

			// Display the current command loops and their associated telemetry.
			//   NOTE!!! this code assumes you are running a fixed wing generation
			//   two controller in the autopilot.  It does not work for other
			//   controllers.  To do this properly we should be reading the
			//   controller XML file to discover the loops
			char LoopStatus[3][10] = {"OFF ", "ON  ", "AUTO"};
			printf("IAS:  %s Cmd:%0.2f Tel%0.2f\n\r", LoopStatus[pData->AP.LoopStatus0], pData->AP.LoopTargets[0], pData->IAS);
			printf("Alt:  %s Cmd:%0.2f Tel%0.2f\n\r", LoopStatus[pData->AP.LoopStatus1], pData->AP.LoopTargets[1], pData->BaroAlt);
			printf("Bank: %s Cmd:%0.2f Tel%0.2f\n\r", LoopStatus[pData->AP.LoopStatus2], pData->AP.LoopTargets[2], pData->Euler[ROLL_AXIS]);
			printf("Flap: %s Cmd:%0.2f\n\r",          LoopStatus[pData->AP.LoopStatus3], pData->AP.LoopTargets[3]);
			printf("Head: %s Cmd:%0.2f\n\r",          LoopStatus[pData->AP.LoopStatus4], pData->AP.LoopTargets[4]);
			printf("VDown:%s Cmd:%0.2f Tel%0.2f\n\r", LoopStatus[pData->AP.LoopStatus5], pData->AP.LoopTargets[5], pData->GPS.VDown);

		}// If the long format of the system status message

	}// if packet is valid
	else
		printf("Invalid SYSTEM_STATUS packet\n\r");

	printf("\n\r");

}// ProcessSystemStatus


/*! This function encapsulates the processing required to calculate the
 *  current track relative position and velocity of the autopilot.  It
 *  accounts for great circle effets since it works in the ECEF coordinate
 *  frame (Earth-Centered-Earth-Fixed).
 *  \param pData contains information from TELEMETRY and SYSTEM_STATUS packets
 *         required to perform the track computations.
 *  \param TrackPosition receives the track relative position in meters.
 *  \param TrackVelocity receives the track relative velocity in meters per second.
 *  \return TRUE if the tracker is on and the track data were computed.*/
BOOL CalculateTrackParameters(const UserData_t* pData, double TrackPosition[N3D], float TrackVelocity[N3D])
{
	if(pData->AP.TrackerStatus)
	{
		double PosWP1[N3D];
		double PosWP2[N3D];
		double Pos[N3D];
		float Vel[N3D];

		// Destination waypoint in ECEF coordinats
		LLAtoECEF((double*)&pData->AP.Waypoint2, PosWP2);

		// Local position and velocity in ECEF coordinates
		LLAandNEDtoECEF(&pData->GPS.Latitude, Pos, &pData->GPS.VNorth, Vel);

		// Compute track relative coordinates, this computation depends
		//   on whether we are orbiting or not.  The orbit computation is
		//   much more intensive but it basically involves a bunch of vector
		//   math to figure out the tangent to the orbit
		if(pData->AP.Orbit)
		{
			double Ray[N3D];
			double Start[N3D];
			double End[N3D];
			double Radius;

			// Calculate the ray vector from origin to current location
			SubtractVectors3D(Pos, PosWP2, Ray);

			// Turn this into a unit vector
			UnitVector3D(Ray);

			// Turn this vector into radius length.  However, there may be an
			//   error introduced by the fact that our altitude does not match
			//   the orbit points altitude.  In this case the ray vector needs
			//   to be longer than the radius so that the horizontal projection
			//   of the ray vector matches the orbit radius.
			Radius = pData->AP.Waypoint2.Alt - pData->GPS.Altitude;
			Radius = sqrt(sqr(pData->AP.OrbitRadius) + sqr(Radius));
			MultiplyVectorByScaler3D(Radius, Ray, Ray);

			// Calculate starting waypoint, which is the tangent point on the circle
			AddVectors3D(PosWP2, Ray, Start);

			// Calculate tangent line vector
			CrossProduct3D(PosWP2, Ray, End);

			// Make this a unit vector, we do this so that the along track value always read 1.0
			UnitVector3D(End);

			// Calculate ending point of track vector
			AddVectors3D(Start, End, End);

			// Calculate track data relative to this tangent line, this will
			//   allow the user to see how well we are doing with respect to
			//   the circular path
			TrackCoordinateParametersECEF(Pos,	// Our current ECEF position
										  Vel,	// Our current ECEF velocity
										  NULL,	// Current ECEF wind velocity
										  Start,		// Beginning of the orbit tangent ECEF
										  End,			// End of the orbit tangent in ECEF
										  TrackPosition,// Track relative position
										  TrackVelocity,// Track relative velocity
										  NULL);		// Track relative wind

			// We have to deal with an interesting problem.  We don't necessarily know the
			//   orbit direction (theoretically we could look it up from what we know of the
			//   flight plan, but that is a pain).  Hence we do a clever guess here, by
			//   looking at the direction of travel and assuming the airplane is doing the
			//   right thing
			if(TrackVelocity[X_AXIS] < 0.0)
			{
				// We are orbiting the other way
				TrackVelocity[X_AXIS] *= -1.0;
				TrackVelocity[Y_AXIS] *= -1.0;
				TrackPosition[Y_AXIS] *= -1.0;

			}// if going backwards

		}// If we are orbiting
		else
		{
			// Origin waypoint in ECEF coordinates
			LLAtoECEF((double*)&pData->AP.Waypoint1, PosWP1);

			// Calculate the position of the vehicle in track relative coordinates
			TrackCoordinateParametersECEF(Pos,			// Current ECEF position
										  Vel,			// Current ECEF velocity
										  NULL,			// ECEF wind vector
										  PosWP1,		// ECEF position of the start of the track segment
										  PosWP2,		// ECEF position of the end of the track segment
										  TrackPosition,// Track relative poisition
										  TrackVelocity,// Track relative velocity
										  NULL);		// Track relative wind
		}// else if we are not orbiting

		return TRUE;

	}// if tracker is on
	else
	{
		// Tracker is off, null the data
		memset(&TrackPosition, 0, sizeof(double[N3D]));
		memset(&TrackVelocity, 0, sizeof(float[N3D]));

		return FALSE;

	}// else if tracker isoff

}// CalculateTrackParameters


void ProcessExternalIOSample(const AutopilotPkt_t* pPkt){ProcessNotImplementedPacket(pPkt);}
void ProcessExternalADCSample(const AutopilotPkt_t* pPkt){ProcessNotImplementedPacket(pPkt);}


/*! This is the helper function that processes a WAYPOINT packet and displays
 *  information about it.  WAYPOINT packets contain the position and metadata
 *  of a single waypoint.  WAYPOINT packets arrive from an autopilot in resposne
 *  to a request for waypoitns, or as an acknowledgement of a waypoint sent to
 *  the avionics.  Receipt of a valid WAYPOINT packet indicates that the
 *  waypoint is resident on avionics.
 *  \param pPkt points to valid autopilot packet received from an avionics. */
void ProcessWaypoint(const AutopilotPkt_t* pPkt)
{
    /*
	FPPoint_t Point;
	UInt8 Index = DecodeWaypointPacket(pPkt, &Point);

	if(Index < NUM_FP_POINTS)
	{
		printf("WAYPOINT packet, index:%d\n\r", (int)Index);

		// Print the interesting data for the packet, notice conversion from radians to degrees
		printf("Pos:%0.6lf %0.6lf %0.2lf Next:%d\n\r", Point.Pos[LAT]*180.0/PI, Point.Pos[LON]*180.0/PI, Point.Pos[ALT], (int)Point.Next);
	}
	else
		printf("Invalid WAYPOINT packet\n\r");

	printf("\n\r");
    */

}// ProcessWaypoint


/*! This is the helper function that processes a WAYPOINT_LIST packet and displays
 *  information about it.  WAYPOINT_LIST packets contain a bit for every possible
 *  waypoint, and are used to communicate information about flight plans.
 *  WAYPOINT_LIST packets arrive from the autopilot in response to requesting
 *  waypoints that don't exist or deleting waypoints.  In either case when this
 *  packet is received it can be interpreted as giving a list of all waypoints
 *  resident on the avionics.
 *  \param pPkt points to valid autopilot packet received from an avionics.*/
void ProcessWaypointList(const AutopilotPkt_t* pPkt)
{
	// This is the list we are going to fill out from the packet.  It is
	//   really just an array of bytes in which each bit is set or clear
	//   for the respective waypoint.
	FPMask_t Points;

	// Decode the packet and get any flags for it.  The flags only have
	//   meaning when the autopilot receives the packet.  In our case
	//   list packets always mean the same thing: this
	//   packet contains the list of valid waypoints on the autopilot
	UInt8 Flags = DecodeWaypointListPacket(pPkt, Points);

	// 0xFF is the failure return code (since 0 is a valid flag)
	if(Flags != 0xFF)
	{
		SInt32 i;

		printf("WAYPOINT_LIST packet, these waypoints onboard AP:\n\r");

		// Whenever a WAYPOINT_LIST packet comes from the autopilot it contains
		//   the list of all the valid waypoints currently onboard the autopilot
		//   If we are keeping a copy of the flight plan locally we can use this
		//   list to make sure we correctly cull any old waypoints out and
		//   conversely ask for any waypoints we don't have
		//
		// This is an example of walking the list
		i = FindFirstFPMaskBitSet(Points);
		while((i >= 0) && (i < NUM_FP_POINTS))
		{
			printf("WP:%2d\n\r", i);
			i = FindNextFPMaskBitSet(Points, (UInt32)i);

		}// while more points in the mask

	}// if valid waypoint list packet
	else
		printf("Invalid WAYPOINT_LIST packet\n\r");

	printf("\n\r");

}// ProcessWaypointList


/*! This is the helper function that processes a RADIO_SETTINGS packet and
 *  displays information about it.  RADIO_SETTINGS packets contain information
 *  about the settings of the internal radio, including power, frequency or
 *  channel and radio type.  This packet arrives in response to a request of
 *  the radio settings or as an acknowledgement of a change in radio settings.
 *  \param pPkt points to valid autopilot packet received from an avionics.*/
void ProcessRadioSettings(const AutopilotPkt_t* pPkt)
{
	RadioSettings_t Radio;

	if(DecodeRadioSettingsPacket(pPkt, &Radio))
	{
		printf("RADIO_SETTINGS packet\n\r");

		// The radio settings packet contains the actual settings, power,
		//   hopping channle or frequency, but it also contains the radio
		//   type which we use to interpret the data somewhat.  The biggest
		//   question is whether the radio is hopping, or coherent.  Coherent
		//   radios do not have hopping channels and conversely hopping
		//   radios do not have a single frequency.

		switch(Radio.Type)
		{
		case MHX_910:		//!< Microhard MHX 910
			printf("MHX 910 ");
			break;
		case MHX_2400:		//!< Microhard MHX 2400
			printf("MHX 2400 ");
			break;
		case MHX_920:		//!< Microhard MHX 920
			printf("MHX 920 ");
			break;
		case MHX_2420:		//!< Microhard MHX 2420
			printf("MHX 2420 ");
			break;
		case MHX_425:		//!< Microhard MHX 425
			printf("MHX 425 ");
			break;
		case MHX_320:		//!< Microhard MHX 320
			printf("MHX 320 ");
			break;
		case MHX_1320:		//!< Microhard MHX 1320
			printf("MHX 1320 ");
			break;
		case DDL_2450:		//!< Nano DDL 2.4
			printf("NanoDDL 2.4 ");
			break;
		case MHX_1700:		//!< Microhard MHX 1700
			printf("MHX 1700 ");
			break;
		case TS_4000:		//!< TS 4000 radio
			printf("TS 4000 ");
			break;
		case BAI_FW_1300:	//!< BAI radio
			printf("BAI raido ");
			break;
		case EXT_MHX_X20:    //!< Externally connected microhard X20 radio
			printf("External MHX ");
			break;
		case AGIG:           //!< Proprietary radio
			printf("AGIG ");
			break;
		case PICO_900:       //!< Microhard P900
			printf("p900 ");
			break;
		case PICO_2400:      //!< Microhard P2400
			printf("p2400 ");
			break;
		default:
			printf("Unknown ");
			break;
		}// switch on radio type

		// Print out radio power in Watts
		printf("Pow:%0.3f ", Radio.Power);

		switch(Radio.Type)
		{
			// these are the coherent radio types
		case MHX_425:		//!< Microhard MHX 425
		case MHX_320:		//!< Microhard MHX 320
		case MHX_1320:		//!< Microhard MHX 1320
		case TS_4000:		//!< TS 4000 radio
		case BAI_FW_1300:	//!< BAI radio
			printf("Freq:%0.3f ", Radio.Freq);
			break;

		default:	// hopping radio type
			printf("Chan:%d ", (int)Radio.Channel);
		}

		// Print out narrow or wide band
		if(Radio.Speed == 1)
			printf("Narrow Band\n\r");
		else
			printf("Wide Band\n\r");

	}// if valid packet
	else
		printf("Invalid RADIO_SETTINGS packet\n\r");

	printf("\n\r");

}// ProcessRadioSettings


/*! This is the helper function that processes a SYSTEM_VERSION packet and
 *  displays information about it.  SYSTEM_VERSION packets contain information
 *  about the firmware and the hardware in the avionics.  This packet arrives
 *  in response to an explicit request of the system version.  This packet
 *  contains the information required to deduce the type and version of the
 *  controller inside the avionics, and is therefore often used to determine
 *  which controller XML file to load.
 *  \param pPkt points to valid autopilot packet received from an avionics.*/
void ProcessSystemVersion(const AutopilotPkt_t* pPkt)
{
	Version_t Version;
	Board_t Board;

	if(DecodeSystemVersionPacket(pPkt, &Version, &Board))
	{
		printf("SYSTEM_VERSION packet:\n\r");

		// Print the version number and release date
		printf("V%d.%d.%d %d/%d/%02d", (int)Version.MajorVersion,
					(int)Version.MinorVersion, (int)Version.SubVersion, (int)Version.Date.month,
					(int)Version.Date.day, (int)(Version.Date.year-2000));

		// Indicate released or test
		if(Version.Released)
			printf(" Rel\n\r");
		else
			printf(" Test\n\r");

		// Print out the type of controller in this packet.  If we were
		//   loading a controller XML file we would use this information
		//   to determine what file to load
		UInt8 Con, Ver;
		Con = (UInt8)((Board.Config >> 8) & 0x000000FF);
		Ver = (UInt8)((Board.Config) & 0x0000003F);
		printf("Controller:%d Version:%d\n\r", (int)Con, (int)Ver);

		// Print the hardware details
		if((Board.Config & CRISTA_IMU_HW) && (Board.Config & UBLOX_GPS_HW))
			printf("HW: Pic II, ");
		else if(Board.Config & CRISTA_IMU_HW)
			printf("HW: Pic Plus, ");
		else if(Board.Config & PICCOLO_LT_IMU_HW)
			printf("HW: Pic LT, ");
		else
			printf("HW: Piccolo, ");

		// Radio type information
		UInt8 Type = (UInt8)((Board.Config & RADIO_CONFIG_BITS)>>20);
		switch(Type)
		{
			case MHX_910: printf("MHX910\n\r"); break;
			case MHX_2400: printf("MHX2400\n\r"); break;
			case MHX_920: printf("MHX920\n\r"); break;
			case MHX_2420: printf("MHX2420\n\r"); break;
			case MHX_425: printf("MHX425\n\r"); break;
			case MHX_320: printf("MHX320\n\r"); break;
			case MHX_1320: printf("MHX1320\n\r"); break;
			case DDL_2450: printf("NanoDDL 2.4\n\r"); break;
			case MHX_1700: printf("MHX1700\r\n"); break;
			case PICO_900: printf("p900\n\r"); break;
			case PICO_2400: printf("p2400\n\r"); break;
		}


	}
	else
		printf("Invalid SYSTEM_VERSION packet\n\r");

	printf("\n\r");
}


void ProcessBandwidthMode(const AutopilotPkt_t* pPkt){ProcessNotImplementedPacket(pPkt);}
void ProcessAutopilotLoop(const AutopilotPkt_t* pPkt){ProcessNotImplementedPacket(pPkt);}
void ProcessTransponderControl(const AutopilotPkt_t* pPkt){ProcessNotImplementedPacket(pPkt);}
void ProcessThrottleTrim(const AutopilotPkt_t* pPkt){ProcessNotImplementedPacket(pPkt);}
void ProcessMissionLimits(const AutopilotPkt_t* pPkt){ProcessNotImplementedPacket(pPkt);}
void ProcessActuatorTable(const AutopilotPkt_t* pPkt){ProcessNotImplementedPacket(pPkt);}
void ProcessSensorOrientation(const AutopilotPkt_t* pPkt){ProcessNotImplementedPacket(pPkt);}
void ProcessAltimeterSetting(const AutopilotPkt_t* pPkt){ProcessNotImplementedPacket(pPkt);}
void ProcessMagnetometerCalibration(const AutopilotPkt_t* pPkt){ProcessNotImplementedPacket(pPkt);}
void ProcessExternalSerialSetup(const AutopilotPkt_t* pPkt){ProcessNotImplementedPacket(pPkt);}
void ProcessExternalIOSetup(const AutopilotPkt_t* pPkt){ProcessNotImplementedPacket(pPkt);}
void ProcessControllerData(const AutopilotPkt_t* pPkt){ProcessNotImplementedPacket(pPkt);}
void ProcessSensorError(const AutopilotPkt_t* pPkt){ProcessNotImplementedPacket(pPkt);}
void ProcessResetReport(const AutopilotPkt_t* pPkt){ProcessNotImplementedPacket(pPkt);}
void ProcessDoubletData(const AutopilotPkt_t* pPkt){ProcessNotImplementedPacket(pPkt);}
void ProcessRawVibratinoData(const AutopilotPkt_t* pPkt){ProcessNotImplementedPacket(pPkt);}
void ProcessLinkBridge(const AutopilotPkt_t* pPkt){ProcessNotImplementedPacket(pPkt);}

void ProcessNotImplementedPacket(const AutopilotPkt_t* pPkt)
{
    printf("Packet %d size %d not implemented\n\n\r", pPkt->PktType, pPkt->Size);
}


/*! This function is a callback registered to the communications manager.  The
 *  communications manager will call this function when a new avionics is
 *  discovered.  We use the call back to signal when we have detected an
 *  avionics.
 *  \param NetworkID gives the serial number of the new avionics.  0 is a
 *         reserved address which indicates a ground station.
 *  \param Parameter is an echo of the parameter registered with the call back.*/
void NewNetwork(UInt16 NetworkID, void* Parameter)
{
	// If NetworkID is zero than this call back is indicating that a ground
	//   station was added to the communications manager.  We are waiting
	//   for an avionics
	if((SerialNumber == 0) && (NetworkID != 0))
		SerialNumber = NetworkID;

}// NewNetwork

