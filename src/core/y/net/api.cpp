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
#include "y/type/utils.hpp"

namespace Yttrium
{

    size_t Network:: select(int            nfds,
                            fd_set * const readfds,
                            fd_set * const writefds,
                            fd_set * const errorfds,
                            Duration      &duration)
    {
#if 0
		std::cerr << "d=" << double(duration) << std::endl;
		std::cerr << "d.tv@" << (void*)duration.tv() << std::endl;
		std::cerr << "r@" << (void*)readfds << std::endl;
		std::cerr << "w@" << (void*)writefds << std::endl;
		struct timeval *tv = duration.tv();
		if (tv)
		{
			std::cerr << "nsec=" << tv->tv_sec  << std::endl;
			std::cerr << "usec=" << tv->tv_usec << std::endl;
			tv->tv_usec = 0;
			tv->tv_sec = 0;
		}
#endif
        
        const int res = ::select(nfds,readfds,writefds,errorfds,duration.tv());
		std::cerr << "res=" << res << std::endl;
        if( IsError(res) ) throw Network::Exception( LastError(), "select");
        assert(res>=0);
        return static_cast<size_t>(res);
    }

    void Network:: sleepFor(double ns)
    {
		fd_set r,w,x;
		FD_ZERO(&r);
		FD_ZERO(&w);
		FD_ZERO(&x);
        Duration d = Max<double>(0,ns);
        (void) select(0,&r,&w,&x,d);
    }


}



