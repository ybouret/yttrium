
//! \file

#ifndef Y_Net_Socket_Protocol_Included
#define Y_Net_Socket_Protocol_Included 1


#include "y/associative/named/variables.hpp"

#if defined(Y_BSD)
#include <netinet/in.h>
#endif

namespace Yttrium
{
    namespace Network
    {

        typedef Named::Variable<int>       Protocol;
        typedef Named::Variables<Protocol> ProtocolSet_;


        class ProtocolSet : public ProtocolSet_
        {
        public:
            static const char * const  CallSign;
            static const char * const  Unknown;
            static Protocol::ConstType Missing;

            explicit ProtocolSet();
            virtual ~ProtocolSet() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ProtocolSet);
        };


    }
}

#endif

