#include "y/net/api.hpp"
#include "y/system/exception.hpp"
#include <cstring>

#if defined(Y_BSD)
#include <cerrno>
#endif

namespace Yttrium
{
    const char * const Network::CallSign = "Network";


#if defined(Y_WIN)
    static WSADATA wsa;
#endif

    Network::Network() : Singleton<Network>()
    {
#if defined(Y_WIN)
        memset(&wsa, 0, sizeof(wsa));
        const int res = WSAStartup(MAKEWORD(2, 2), &wsa);
        if (res != 0)  throw Win32::Exception(res,"WSAStartup");
#endif
    }

    Network:: ~Network() noexcept
    {
#if defined(Y_WIN)
        (void)WSACleanup();
        memset(&wsa, 0, sizeof(wsa));
#endif
    }

    bool Network:: IsError(const int returnValue) noexcept
    {
#if defined(Y_BSD)
        return returnValue < 0;
#endif

#if defined(Y_WIN)
        return SOCKET_ERROR == returnValue;
#endif
    }


    int Network:: LastError() noexcept
    {
#if defined(Y_BSD)
        return errno;
#endif

#if defined(Y_WIN)
        return WSAGetLastError();
#endif
    }


}


#if defined(Y_BSD)
#include <sys/select.h>
#endif

namespace Yttrium
{

    size_t Network:: select(int            nfds,
                            fd_set * const readfds,
                            fd_set * const writefds,
                            fd_set * const errorfds,
                            Duration      &duration)
    {
        const int res = ::select(nfds,readfds,writefds,errorfds, duration.tv() );
        if( IsError(res) ) throw Network::Exception( LastError(), "select");



        return 0;
    }

}



