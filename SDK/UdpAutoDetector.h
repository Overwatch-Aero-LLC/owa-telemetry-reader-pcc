#ifndef UDPAUTODETECTOR_H
#define UDPAUTODETECTOR_H

// Our own headers
#include "GSPacket.h"
#include "stream.h"

// For STL objects
#include <string>
#include <vector>

// UDP server type identifier enumeration
typedef enum
{
    SERVER_TYPE_UNKNOWN       = 0x0001,
    SERVER_TYPE_GROUNDSTATION = 0x0002,
    SERVER_TYPE_AUTOPILOT     = 0x0004,
    SERVER_TYPE_GIMBAL        = 0x0008,
    SERVER_TYPE_NAVIGATOR     = 0x0010,
    SERVER_TYPE_RADIO_BOARD   = 0x0020,
    SERVER_TYPE_ANY           = 0xFFFF
} UdpServerType_t;

// UDP server information flags
typedef enum
{
    SERVER_FLAG_NONE          = 0x0001,
    SERVER_FLAG_PROGRAM_MODE  = 0x0002,
    SERVER_FLAG_ANY           = 0xFFFF,
} UdpServerFlag_t;

// UDP server description structure
typedef struct
{
    std::string     Name;      //!< The name of the server as it reports it
    UdpServerType_t Type;      //!< Server type identifier
    UdpServerFlag_t Flag;      //!< Flags relevant to this server
    UInt32          Address;   //!< Source IP address
    UInt16          Port;      //!< Destination UDP port
    UInt32          TimeStamp; //!< Time stamp of last received packet
} UdpServer_t;

// We'll just use STL vectors to list UDP servers
typedef std::vector<UdpServer_t> UdpServerList_t;

// In theory, we shouldn't need more than 512 bytes for a single datagram
#define MAX_DGRAM_SIZE 512

//! Class for automatically detecting a UDP server
class UdpAutoDetector
{
public:
    //! Constructor
    UdpAutoDetector(void);

    //! Destructor
    ~UdpAutoDetector(void);

    //! Get the last returned error condition (TBD)
    std::string getLastError(void) { return m_Error; m_Error = ""; }

    //! Check for any new UDP servers
    BOOL checkForNewServers(UdpServerType_t Filter, UdpServerFlag_t Flags);

    //! Get the current list of UDP servers on the subnet
    UdpServerList_t getServerList(void) { return m_Servers; }

private:
    //! Broadcast a monitor version query
    void queryMonitorVersion(void);

    //! Check a specific socket for new servers
    BOOL checkForNewServers(UInt32 SocketIndex, UdpServerType_t Filter, UdpServerFlag_t Flags);

    //! Try adding a new server to the list
    BOOL addNewServer(const std::string &Name, UdpServerType_t Type,
        UdpServerFlag_t Flags, struct sockaddr_in *pSockAddr);

    // Private data members
    UInt32             m_Socket[2];              //!< Link1/2 sockets
    UInt8              m_Buffer[MAX_DGRAM_SIZE]; //!< Datagram buffer
    StreamPkt_t        m_StreamPkt[2];           //!< Stream packet in process
    GroundStationPkt_t m_GsPkt[2];               //!< Groundstation packet in process
    std::string        m_Error;                  //!< Last error description
    UdpServerList_t    m_Servers;                //!< List of all discovered servers
    UInt32             m_NextMonitorQuery;       //!< Timestamp of the next monitor query
};

#endif // UDPAUTODETECTOR_H