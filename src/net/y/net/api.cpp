#include "y/net/api.hpp"
#include "y/system/exception.hpp"
#include <cstring>
#include "y/memory/buffer/of.hpp"


#if defined(Y_BSD)
#include <cerrno>
#include <unistd.h>
#include <sys/socket.h>
#endif

namespace Yttrium
{

    namespace Network
    {
        const char * const API::CallSign = "Network";

        bool API:: IsError(const int returnValue) noexcept
        {
#if defined(Y_BSD)
            return returnValue < 0;
#endif

#if defined(Y_WIN)
            return SOCKET_ERROR == returnValue;
#endif
        }


        bool API:: IsInvalid(const SystemSocket &s) noexcept
        {
#if defined(Y_BSD)
            return s < 0;
#endif

#if defined(Y_WIN)
            return INVALID_SOCKET == s;
#endif
        }



        int API:: LastError() noexcept
        {
#if defined(Y_BSD)
            return errno;
#endif

#if defined(Y_WIN)
            return WSAGetLastError();
#endif
        }


#if defined(Y_WIN)
        static WSADATA wsa;
#endif

        API:: API() :
        Singleton<API>(),
        hostName(),
        unknown( new String(Core::Unknown) ),
        pf(unknown)
        {
#if defined(Y_WIN)
            memset(&wsa, 0, sizeof(wsa));
            const int res = WSAStartup(MAKEWORD(2, 2), &wsa);
            if (res != 0)  throw Win32::Exception(res,"WSAStartup");
#endif
            // getting host name
            {
                size_t buflen = 256;
            GETHOSTNAME:
                Memory::BufferOf<char,Memory::Pooled> membuf(buflen);
                    char * const buffer = &membuf[0];
                    assert( Memory::OutOfReach::Are0(buffer,buflen) );
                    Y_GIANT_LOCK();
                    if( IsError( gethostname(buffer,buflen)) )
                    {
                        buflen <<= 1;
                        goto GETHOSTNAME;
                    }
                    Coerce(hostName) = buffer;
            }

            Y_Net_Add(pf,PF_INET);
            Y_Net_Add(pf,PF_INET6);


        }

        API:: ~API() noexcept
        {
#if defined(Y_WIN)
            (void)WSACleanup();
            memset(&wsa, 0, sizeof(wsa));
#endif
        }


        SystemSocket API:: socket_create(const int protocolFamily, const int type, const int protocol)
        {
            Y_GIANT_LOCK();
            const SystemSocket s = socket(protocolFamily,type,protocol);
            if( IsInvalid(s) ) {
                const int err = LastError();
                throw Exception(err,"socket()");
            }
            return s;
        }

        void API:: socket_delete(SystemSocket &s) noexcept
        {
            Y_GIANT_LOCK();
            assert( ! IsInvalid(s) );
#if defined(Y_BSD)
            (void) close(s);
#endif

#if defined(Y_WIN)
            closesocket(s);
#endif
            s = INVALID_SOCKET;
        }


    }



}



