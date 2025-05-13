
#include "y/net/socket/protocol-family.hpp"
#include "y/system/exception.hpp"



namespace Yttrium
{
    namespace Network
    {

        const char * const        ProtocolFamilySet:: CallSign = "ProtocolFamily";
        const char * const        ProtocolFamilySet:: Unknown  = "PF_UNKNOWN";
        ProtocolFamily::ConstType ProtocolFamilySet:: Missing  = -1;


#define Y_Net(NAME) do { if(!(*this)(#NAME,NAME)) throw Specific::Exception(CallSign,"multiple " #NAME); } while(false)

        ProtocolFamilySet:: ProtocolFamilySet() :
        ProtocolFamilySet_( Unknown, Missing )
        {
            Y_Net(PF_INET);
            Y_Net(PF_INET6);
            std::cerr << (*this)->byName << std::endl;
        }


        ProtocolFamilySet:: ~ProtocolFamilySet() noexcept
        {

        }


    }

}


