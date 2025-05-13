
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

        typedef Named::Variable<int>    Style;
        typedef Named::Variables<Style> StyleSet_;


        class StyleSet : public StyleSet_
        {
        public:
            static const char * const CallSign;
            static const char * const Unknown;
            static Style::ConstType   Missing;

            explicit StyleSet();
            virtual ~StyleSet() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(StyleSet);
        };


    }
}

#endif
