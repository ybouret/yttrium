#include "y/net/api.hpp"
#include "y/system/exception.hpp"
#include <cstring>


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
}



