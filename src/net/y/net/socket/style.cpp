

#include "y/net/socket/style.hpp"
#include "y/system/exception.hpp"



namespace Yttrium
{
    namespace Network
    {

        const char * const   StyleSet:: CallSign = "Style";
        const char * const   StyleSet:: Unknown  = "SOCK_UNKNOWN";
        Style::ConstType     StyleSet:: Missing  = -1;


#define Y_Net_Style(NAME) do { if(!Y_Named_Variable_Decl(*this,SOCK_##NAME)) throw Specific::Exception(CallSign,"multiple SOCK_" #NAME); } while(false)

        StyleSet:: StyleSet() :
        StyleSet_( Unknown, Missing )
        {
            Y_Net_Style(STREAM);
            Y_Net_Style(DGRAM);
            std::cerr << (*this)->byName << std::endl;
        }


        StyleSet:: ~StyleSet() noexcept
        {

        }


    }

}


