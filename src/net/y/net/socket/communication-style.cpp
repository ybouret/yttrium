

#include "y/net/socket/communication-style.hpp"
#include "y/system/exception.hpp"



namespace Yttrium
{
    namespace Network
    {


        const char * const            CommunicationStyleSet:: CallSign = "CommunicationStyle";
        const char * const            CommunicationStyleSet:: Unknown  = "SOCK_UNKNOWN";
        CommunicationStyle::ConstType CommunicationStyleSet:: Missing  = -1;


#define Y_Net_Style(NAME) do { if(!Y_Named_Variable_Decl(*this,SOCK_##NAME)) throw Specific::Exception(CallSign,"multiple SOCK_" #NAME); } while(false)

        CommunicationStyleSet:: CommunicationStyleSet() :
        CommunicationStyleSet_( Unknown, Missing )
        {
            Y_Net_Style(STREAM);
            Y_Net_Style(DGRAM);
            std::cerr << (*this)->byName << std::endl;
        }


        CommunicationStyleSet:: ~CommunicationStyleSet() noexcept
        {

        }


    }

}


