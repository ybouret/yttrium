

#include "y/net/socket/protocol.hpp"
#include "y/system/exception.hpp"



namespace Yttrium
{
    namespace Network
    {

        const char * const  ProtocolSet:: CallSign = "Protocol";
        const char * const  ProtocolSet:: Unknown  = "PROTO_UNKNOWN";
        Protocol::ConstType ProtocolSet:: Missing  = -1;


#define Y_Net(NAME) do { if(!(*this)(#NAME,NAME)) throw Specific::Exception(CallSign,"multiple " #NAME); } while(false)

        ProtocolSet:: ProtocolSet() :
        ProtocolSet_( Unknown, Missing )
        {
            Y_Net(IPPROTO_TCP);
            Y_Net(IPPROTO_UDP);
            std::cerr << (*this)->byName << std::endl;
        }


        ProtocolSet:: ~ProtocolSet() noexcept
        {

        }


    }

}


