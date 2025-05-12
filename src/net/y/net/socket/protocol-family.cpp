
#include "y/net/socket/protocol-family.hpp"
#include "y/system/exception.hpp"



namespace Yttrium
{
    namespace Network
    {

        const char * const        ProtocolFamilySet:: CallSign = "ProtocolFamily";
        const char * const        ProtocolFamilySet:: Unknown  = "PF_UNKNOWN";
        ProtocolFamily::ConstType ProtocolFamilySet:: Missing  = -1;


#define Y_Net_PF(NAME) do { if(!Y_Named_Variable_Decl(*this,PF_##NAME)) throw Specific::Exception(CallSign,"multiple PF_" #NAME); } while(false)

        ProtocolFamilySet:: ProtocolFamilySet() :
        ProtocolFamilySet_( Unknown, Missing )
        {
            Y_Net_PF(INET);
            Y_Net_PF(INET6);
            std::cerr << (*this)->byName << std::endl;
        }


        ProtocolFamilySet:: ~ProtocolFamilySet() noexcept
        {

        }


    }

}


