//! \file

#ifndef Y_Net_API_Included
#define Y_Net_API_Included 1

#include "y/string.hpp"
#include "y/singleton.hpp"
#include "y/system/duration.hpp"
#include "y/system/exception.hpp"

#if defined(Y_WIN)
#include <ws2tcpip.h>
#endif

#if defined(Y_BSD)
#include <sys/select.h>
#endif

namespace Yttrium
{

    namespace Network
    {

#if defined(Y_BSD)
        typedef Libc::Exception  Exception; //!< alias
        typedef int              SystemSocket;
#define INVALID_SOCKET           (-1)
#endif

#if defined(Y_WIN)
        typedef Win32::Exception Exception; //!< alias
        typedef SOCKET           SystemSocket;
#endif

        


        //! API
        class API : public Singleton<API>
        {
        public:
            //______________________________________________________________________
            //
            //
            // Definitions
            //
            //______________________________________________________________________
            static const char * const      CallSign;                                //!< "Network"
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 9; //!< Longevity

            //______________________________________________________________________
            //
            //
            // Methods
            //
            //______________________________________________________________________
            static bool IsError(const int returnValue)  noexcept; //!< returnValue to error
            static int  LastError()                     noexcept; //!< errno, WSAGetLastError()...
            static bool IsInvalid(const SystemSocket &) noexcept;

            SystemSocket socket_create(const int protocolFamily, const int type, const int protocol);
            void         socket_delete(SystemSocket &s) noexcept;


            const String hostName;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(API);
            friend class Singleton<API>;
            explicit API();
            virtual ~API() noexcept;
            
        };

    }


    


}

#endif
