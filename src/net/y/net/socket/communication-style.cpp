

#include "y/net/socket/communication-style.hpp"
#include "y/system/exception.hpp"



namespace Yttrium
{
    namespace Network
    {


        const char * const            CommunicationStyleSet:: CallSign = "CommunicationStyle";
        const char * const            CommunicationStyleSet:: Unknown  = "SOCK_UNKNOWN";
        CommunicationStyle::ConstType CommunicationStyleSet:: Missing  = -1;


#define Y_Net(NAME) do { if(!(*this)(#NAME,NAME)) throw Specific::Exception(CallSign,"multiple " #NAME); } while(false)

        CommunicationStyleSet:: CommunicationStyleSet() :
        CommunicationStyleSet_( Unknown, Missing )
        {
            Y_Net(SOCK_STREAM);
            Y_Net(SOCK_DGRAM);
            std::cerr << (*this)->byName << std::endl;
        }


        CommunicationStyleSet:: ~CommunicationStyleSet() noexcept
        {

        }


    }

}


