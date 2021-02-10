// PayloadDecode.cpp : Defines the entry point for the console application.
//

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

#include "CommManager.h"
#include "serial.h"


// Local functions
static void RunPayloadRelay(CCommManager* pComm, FILE* pFile);
static void RunPayloadDisplay(CCommManager* pComm, FILE* pFile);
static void NewNetwork(UInt16 NetworkID, void* Parameter);
static BOOL KBInput(void);

// Local data
static UInt16 SerialNumber = 0;
static BOOL TelemEnabled = FALSE;
static UInt8 PayloadStream = PAYLOAD_STREAM;
static UInt8 PortNum = 0;
static UInt8 Parity = NO_PARITY;
static Serial_t Port;

int main(int argc, char* argv[])
{
	CCommManager* pComm = NULL;
	UInt8 TelemPortNum = 0;
	UInt32 Baud = 9600, TelemBaud=2400;
	char Server[128] = "localhost:2000";
	FILE* pFile = NULL;

	int i;
	char* param;

	printf("\n\rPayload decode utility for moving data to and from a payload stream\n\r\n\r");
	printf("Usage: PayloadDecode [-P=x] [-B=x] [-PARITY=x] [-SERVER=server:port] [-SN=serialnumber] [-STREAM=n]\n\r");
	printf("       -P=x  specifies the serial port number to relay data to, default of 1\n\r");
	printf("       -B=x  specifies the serial port baud rate, default of 9600\n\r");
	printf("       -PARITY=x specifies the parity of the serial port, 0 for none, 1 for odd, 2 for even\n\r");
	printf("       -SERVER=server:port  specifies the piccolo communications server to\n\r");
	printf("                            connect to, defaults to localhost:2000\n\r");
	printf("       -SN=serialnumber  specifies the serial number of the avionics to\n\r");
	printf("                         relay payload data to and from.  Defaults to the\n\r");
	printf("                         first avionics detected\n\r");
	printf("       -T=x  specifies the telemetry serial port number to use for listening\n\r");
	printf("             for comms data (rather than server mode)\n\r");
	printf("       -TB=x specifies the telemetry baud rate, default 2400\n\r");
	printf("       -STREAM=x specifies the payload stream to retrieve (1 for primary payload (default), 2 for secondary payload)\n\r\n\r");
	printf("       -FILE=filename specifies a binary file to which the downlinked payload bytes will be saved.");

	// parse the command line parameters
	for(i = 1; i < argc; i++)
	{
		param = strstr(argv[i], "-P=");
		if(param)
		{
			param += 3;						// Move forward past parameter name
			PortNum = (UInt8)atoi(param)-1;	// Get the port number in the parameter; subtract 1 because COM1 is actually 0.
			continue;						// Move to next parameter
		}// If found a port number parameter

		param = strstr(argv[i], "-B=");
		if(param)
		{
			param += 3;					// Move forward past parameter name
			Baud = (UInt32)atoi(param);	// Get the port number in the parameter
			continue;					// Move to next parameter
		}// If found a baud rate parameter

		param = strstr(argv[i], "-PARITY=");
		if(param)
		{
			int parity;
			param += 8;
			parity = atoi(param);
			switch(parity)
			{
			default: Parity = NO_PARITY; break;
			case 1: Parity = ODD_PARITY; break;
			case 2: Parity = EVEN_PARITY; break;
			}
		}// If found a parity parameter

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

		param = strstr(argv[i], "-T=");
		if(param)
		{
			TelemEnabled=TRUE;
			param += 3;						// Move forward past parameter name
			TelemPortNum = (UInt8)atoi(param)-1;	// Get the port number in the parameter; subtract 1 because COM1 is actually 0.
			continue;						// Move to next parameter
		}// If found a port number parameter

		param = strstr(argv[i], "-TB=");
		if(param)
		{
			TelemEnabled=TRUE;
			param += 4;							// Move forward past parameter name
			TelemBaud = (UInt16)atoi(param);	// Get the serial number in the parameter
			continue;							// Move to next parameter
		}// If found a port number parameter

		param = strstr(argv[i], "-STREAM=");
		if(param)
		{
			UInt8 temp;

			param += 8;							// Move forward past parameter name
			temp = (UInt8)atoi(param);		// Get the stream number

			// Choose between the first and the second payload stream depending on the parameter value
			if(temp == 1)
				PayloadStream = PAYLOAD_STREAM;
			else if(temp == 2)
				PayloadStream = PAYLOAD2_STREAM;
			continue;							// Move to next parameter
		}// If found a port number parameter

		param = strstr(argv[i], "-FILE=");
		if(param)
		{
			char fname[256];

			// Move forward past parameter name
			param += 6;

			// Read file name
			strcpy(fname, param);

			// Attempt to open a file with this name
			pFile = fopen(fname, "wb");

			// Move to next parameter
			continue;
		}

	}// for all parameters


	// Instantiate comm manager object
	if (TelemEnabled)
		pComm = new CCommManager(TelemPortNum, TelemBaud, 0, -1, FALSE, 0, 0, FALSE);	//Serial port telemetry
	else
		pComm = new CCommManager(0xFF, 57600, Server, -1, FALSE), 0, 0, FALSE;		//Server telemetry

	// If object construction was successful
	if(pComm)
	{
		if(pComm->GetLastError())
			printf("%s", pComm->GetLastError());
		else
		{
			// Register new avionics callback function
			pComm->SetNewNetworkCallBack(NewNetwork, 0);

			// Open the serial port
			if(OpenCOMM(PortNum, BOTH_DIR, Baud, Parity, 8, FLOW_NONE, 8192, &Port))
				// Run the payload relay function
				RunPayloadRelay(pComm, pFile);
			else
			{
				printf("Failed to open serial port %d!, running payload display\r\n", (int)PortNum+1);
				RunPayloadDisplay(pComm, pFile);
			}
				
			// Close the serial port
			CloseCOMM(&Port);
			
			// Close the binary log file
			fclose(pFile);

		}// If no errors in the comm manager

		delete pComm;

	}// If created the communications manager
	else
		printf("Failed to create comm manager!\r\n");

	return 0;
}// main


#define SER_BUFSIZE 16384

void RunPayloadRelay(CCommManager* pComm, FILE* pFile)
{
	Queue_t* pRxQ;
	UInt8 Data[SER_BUFSIZE];
	SInt16 Byte;
	UInt32 Size;
	UInt32 Num, i;
	UInt32 Down = 0;
	UInt32 Up = 0;


	if(SerialNumber == 0)
	{
		printf("Waiting to detect first avionics\r\n");

		while(SerialNumber == 0)
		{
			pComm->RunNetwork();

			// sleep for 50 milliseconds
			#ifdef WIN32
			Sleep(50);
			#else
			usleep(50000);
			#endif

			// see if the user wants to quit
			if(KBInput()) return;

		}// while still waiting to detect a network

	}// If user didn't specify an explicit serial number
	else
	{
		pComm->CreateNetwork(SerialNumber);
	}// If user did specify a serial number


	printf("Press Q to quit\n");

	// Print header line out
	printf("\r\n");
	printf("  SN#   Down     Up\r\n");
  //printf("65534 999999 999999\n");


	// If we get here then the comm manager is up and running and all the
	//    network structures are built, start moving data
	pRxQ = pComm->GetStreamRxBuffer(SerialNumber, PayloadStream);
	while(1)
	{
		// Run the network
		pComm->RunNetwork();

		// Sleep to let the rest of the system run
		#ifdef WIN32
		Sleep(50);
		#else
		usleep(50000);
		#endif

		// print status
		printf("\r%5hd %6u %6u", SerialNumber, Down, Up);

		// Check to see if it is time to exit
		if(KBInput()) return;

		// Look for data coming over the payload stream
		Size = 0;
		Byte = DeQ(pRxQ);
		while((Byte >= 0) && (Size < SER_BUFSIZE))
		{
			Data[Size++] = (UInt8)Byte;
			Byte = DeQ(pRxQ);
		}// while still receive dat

		// If we received some data from the avionics and the output serial port is still open
		if(Size)
		{
			UInt32 i;

			// Move the receive data out the serial port
			WriteBlockQuick(&Port, Data, Size);

			// Save data to binary file
			if(pFile) for(i=0; i<Size; i++)
				fputc(Data[i], pFile);
		}

		// Track amount of downstream data
		Down += Size;

		// Look for data coming over the serial port
		i = 0;
		Byte = ReadByteQuick(&Port);
		while((Byte > -1) && (i < 100))
		{
			Data[i] = (UInt8)Byte;
			i++;
			Byte = ReadByteQuick(&Port);
		}
		Num = i;

		// Now move the data to the payload stream
		if(Num)
			pComm->SendBlock(SerialNumber, PayloadStream, Data, Num);

		// Track amount of upstream data
		Up += Num;


	}// forever loop

}// RunPayloadRelay


void RunPayloadDisplay(CCommManager* pComm, FILE* pFile)
{
	Queue_t* pRxQ;
	SInt16 Byte;


	if(SerialNumber == 0)
	{
		printf("Waiting to detect first avionics\r\n");

		while(SerialNumber == 0)
		{
			pComm->RunNetwork();

			// sleep for 50 milliseconds
			#ifdef WIN32
			Sleep(50);
			#else
			usleep(50000);
			#endif

			// see if the user wants to quit
			if(KBInput()) return;

		}// while still waiting to detect a network

	}// If user didn't specify an explicit serial number
	else
	{
		pComm->CreateNetwork(SerialNumber);
	}// If user did specify a serial number


	printf("Press Q to quit\n\n");

	// If we get here then the comm manager is up and running and all the
	//    network structures are built, start moving data
	pRxQ = pComm->GetStreamRxBuffer(SerialNumber, PayloadStream);
	while(1)
	{
		// Run the network
		pComm->RunNetwork();

		// Sleep to let the rest of the system run
		#ifdef WIN32
		Sleep(50);
		#else
		usleep(50000);
		#endif

		// Look for data coming over the payload stream
		Byte = DeQ(pRxQ);
		while(Byte >= 0)
		{
			// Print byte to screen
			putchar(Byte);

			// Save data to binary file
			if(pFile) fputc(Byte, pFile);

			// Get another byte
			Byte = DeQ(pRxQ);
		}

		// Check for data to send
		if(_kbhit())
		{
			UInt8 key = (UInt8)_getch();
			
			pComm->SendBlock(SerialNumber, PayloadStream, &key, 1);
		}// If a keystroke is waiting

	}// forever loop

}// RunPayloadDisplay


void NewNetwork(UInt16 NetworkID, void* Parameter)
{
	if((SerialNumber == 0) && (NetworkID != 0))
		SerialNumber = NetworkID;
}// NewNetwork


BOOL KBInput(void)
{
	if(_kbhit())
	{
		int key = _getch();
		if((key == 'q') || (key == 'Q'))
			return TRUE;
	}// If a keystroke is waiting

	return FALSE;
}// KBInput


#ifndef WIN32

char kb_getch(void);
int _kbhit(void);

// return any character that has been typed, or zero if nothing
char getch(void)
{
    return 0;
}// kb_getch

int _kbhit(void)
{
	return 0;
}

#endif
