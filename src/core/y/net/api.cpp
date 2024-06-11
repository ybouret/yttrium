#include "y/net/api.hpp"
#include "y/system/exception.hpp"
#include <cstring>

#if defined(Y_WIN)
#include <winsock2.h>
#include <ws2tcpip.h>
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
		if (res != 0) {
			printf("WSAStartup failed: %d\n", res);
		}
		Win32::Exception excp(WSASYSNOTREADY, "Testing");
		excp.display();
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



