
//! \file

#ifndef Y_Net_Socket_Style_Included
#define Y_Net_Socket_Style_Included 1


#include "y/associative/named/variables.hpp"

#if defined(Y_BSD)
#include <sys/socket.h>
#endif

namespace Yttrium
{
    namespace Network
    {

        typedef Named::Variable<int>                 CommunicationStyle;
        typedef Named::Variables<CommunicationStyle> CommunicationStyleSet_;


        class CommunicationStyleSet : public CommunicationStyleSet_
        {
        public:
            static const char * const              CallSign;
            static const char * const              Unknown;
            static CommunicationStyle::ConstType   Missing;

            explicit CommunicationStyleSet();
            virtual ~CommunicationStyleSet() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(CommunicationStyleSet);
        };


    }
}

#endif
