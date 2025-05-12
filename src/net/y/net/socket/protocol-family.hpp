
#ifndef Y_Net_Socket_Protocol_Family_Included
#define Y_Net_Socket_Protocol_Family_Included 1


#include "y/associative/named/variables.hpp"


namespace Yttrium
{
    namespace Network
    {

        typedef Named::Variable<int>             ProtocolFamily;
        typedef Named::Variables<ProtocolFamily> ProtocolFamilySet;
        


    }
}

#endif

