// DebugDecode.cpp : Defines the entry point for the console application.
//

#include <conio.h>
#include "CommManager.h"

static void RunDebugDecode(CCommManager* pComm);
static void NewNetwork(UInt16 NetworkID, void* Parameter);

static UInt16 SerialNumber = 0;

static BOOL TelemEnabled=FALSE;

int main(int argc, char* argv[])
{
	CCommManager* pComm = NULL;
	char Server[128] = "localhost:2000";
	int i;
	char* param;
	
	printf("Debug decode utility for moving data to and from the debug stream\n");
	printf("Usage: DebugDecode [-SERVER=server:port] [-SN=serialnumber]\n");
	printf("\t-SERVER=server:port specifies the piccolo communications server to\n");
	printf("\t                    connect to, defaults to localhost:2000\n");
	printf("\t-SN=serialnumber specifies the serial number of the avionics to\n");
	printf("\t                 relay debug data to and from.  Defaults to the\n");
	printf("\t                 first avionics detected\n");

	// parse the command line parameters
	for(i = 1; i < argc; i++)
	{
		param = strstr(argv[i], "-SERVER=");
		if(param)
		{
			param += 8;				// Move forward past parameter name
			strcpy(Server, param);	// Get the server name in the parameter
			continue;				// Move to next parameter
		}// If found a server parameters

		param = strstr(argv[i], "-SN=");
		if(param)
		{
			param += 4;							// Move forward past parameter name
			SerialNumber = (UInt16)atoi(param);	// Get the serial number in the parameter
			continue;							// Move to next parameter
		}// If found a port number parameter

	}// for all parameters

	// Open the serial port
	pComm = new CCommManager(0xFF, 57600, Server, -1, FALSE, 0, 0, FALSE);	//Server telemetry
	//pComm = new CCommManager(4, 57600, NULL, -1, FALSE);	//Server telemetry

	if(pComm)
	{
		if(pComm->GetLastError())
			printf("%s", pComm->GetLastError());
		else
		{
			pComm->SetNewNetworkCallBack(NewNetwork, 0);
			RunDebugDecode(pComm);
		}// If no errors in the comm manager
			
		delete pComm;
	}// If created the communications manager
	else
		printf("Failed to create comm manager!\n");

	return 0;
}// main


void RunDebugDecode(CCommManager* pComm)
{
	StreamPkt_t UpPacket;
	Queue_t* pRxQ;
	SInt16 Data;
	FILE* pFile;

	if(SerialNumber == 0)
	{
		printf("Waiting to detect first avionics\n");

		while(SerialNumber == 0)
		{
			pComm->RunNetwork();
			Sleep(50);

			// Check to see if it is time to exit
			if(_kbhit())
			{
				int key = _getch();
				if((key == 'q') || (key == 'Q'))
					return;
				if((key == 'c') || (key == 'C'))
					system("cls");
			}// If a keystroke is waiting

		}// while still waiting to detect a network

	}// If user didn't specify an explicit serial number
	else
	{
		pComm->CreateNetwork(SerialNumber);
	}// If user did specify a serial number

	// Tell the user how to exit
	printf("Press Q to quit\n\n");

	// Open the file
	pFile = fopen("DebugLog.txt", "a");

	if(!pFile)
		printf("DebugLog.txt file failed to open!\n\n");

	// Initialize our uplink packet
	MakePacketHeader(&UpPacket, SerialNumber, OI_ADDRESS,
					 DEBUG_STREAM, 0, 0, 0, 0);

	// If we get here then the comm manager is up and running and all the
	//    network structures are built, start moving data
	pRxQ = pComm->GetStreamRxBuffer(SerialNumber, DEBUG_STREAM);
	while(1)
	{
		// Run the network
		pComm->RunNetwork();

		// Sleep to let the rest of the system run
		Sleep(100);

		// Look for data to display
		Data = DeQ(pRxQ);
		while(Data >= 0)
		{
			putchar((char)Data);
			fputc((char)Data, pFile);
			Data = DeQ(pRxQ);
		}
		

		// Check for keyboard data
		if(_kbhit())
		{
			int key = _getch();
			switch(key)
			{
			case 'q':	// exit code
			case 'Q':
				return;
				break;

			case 13:	// send packet code
				UpPacket.Data[UpPacket.Size++] = (UInt8)key;
				CRCPacket(&UpPacket);
				pComm->SendStreamPacket(&UpPacket);
				UpPacket.Size = 0;
				break;

			default:
				UpPacket.Data[UpPacket.Size++] = (UInt8)key;
				if(UpPacket.Size >= MAX_STREAM_DATA_SIZE)
				{
					CRCPacket(&UpPacket);
					pComm->SendStreamPacket(&UpPacket);
					UpPacket.Size = 0;
				}
				break;
			}// switch

		}// If a keystroke is waiting

	}// forever loop

}// RunPayloadRelay


void NewNetwork(UInt16 NetworkID, void* Parameter)
{
	if((SerialNumber == 0) && (NetworkID != 0))
		SerialNumber = NetworkID;
}// NewNetwork
