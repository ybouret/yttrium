
#ifndef Y_Net_Socket_Protocol_Family_Included
#define Y_Net_Socket_Protocol_Family_Included 1


#include "y/associative/named/variables.hpp"

#if defined(Y_BSD)
#include <sys/socket.h>
#endif

namespace Yttrium
{
    namespace Network
    {

        typedef Named::Variable<int>             ProtocolFamily;
        typedef Named::Variables<ProtocolFamily> ProtocolFamilySet_;


        class ProtocolFamilySet : public ProtocolFamilySet_
        {
        public:
            static const char * const CallSign;
            static const char * const Unknown;
            static ProtocolFamily::ConstType Missing;

            explicit ProtocolFamilySet();
            virtual ~ProtocolFamilySet() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ProtocolFamilySet);
        };


    }
}

#endif

