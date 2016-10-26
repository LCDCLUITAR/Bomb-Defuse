/// \file IPMgr.h 
/// \brief Interface for the IPManager class.

#pragma once

#include <winsock.h>

#define WINSOCK_VERSION MAKEWORD(1,1) ///< MAKEWORD(x,y) for version x.y

/// Winsock error description.

struct WSAEDESCRIPTION{
  int nErrorCode; ///< Error code.
  char *szDescription; ///< Text description of the error.
}; //WSAEDESCRIPTION

/// \brief The IP manager class manages UDP/IP error reporting.
///
/// The IP manager is intended only to be used by the debug manager for sending
/// error strings to a debug client.

class IPManager{
  private:
    BOOL m_bInitialized; ///< TRUE if initialized OK.
    SOCKADDR_IN m_saClient; ///< Client socket info.
    SOCKET m_Socket; ///< The socket.

  public:
    IPManager(char* addr,int port); ///< Constructor.
    virtual ~IPManager(); ///< Destructor.
    BOOL SendPacket(char* message,int length); ///< Send packet.
    LPCTSTR WinsockErrorDescription(int nErrorCode); ///< Describe WS error message.
}; //IPManager
