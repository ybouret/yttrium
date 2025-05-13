//! \file

#ifndef Y_Net_API_Included
#define Y_Net_API_Included 1

#include "y/string.hpp"
#include "y/singleton.hpp"
#include "y/system/duration.hpp"
#include "y/net/type/exception.hpp"

#include "y/net/socket/protocol-family.hpp"
#include "y/net/socket/communication-style.hpp"
#include "y/net/socket/protocol.hpp"

#if defined(Y_WIN)
#include <ws2tcpip.h>
#endif


namespace Yttrium
{

    namespace Network
    {

#if defined(Y_BSD)
        typedef int              SystemSocket;
#define INVALID_SOCKET           (-1)
#endif

#if defined(Y_WIN)
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

            SystemSocket socket_create(const int protocolFamily, const int style, const int protocol);
            void         socket_delete(SystemSocket &s) noexcept;


            const String          hostName;
            ProtocolFamilySet     protocolFamilySet;
            CommunicationStyleSet communicationStyleSet;
            ProtocolSet           protocolSet;
            bool                  verbose;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(API);
            friend class Singleton<API>;
            explicit API();
            virtual ~API() noexcept;
            
        };

    }


    


}

#endif
