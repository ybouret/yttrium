
//! \file


#ifndef Y_Chemical_Equilibrium_Included
#define Y_Chemical_Equilibrium_Included 1


#include "y/chemical/type/xnode.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        class Equilibrium
        {
        public:
            static const char         Prefix    = '@';
            static const char         Separator = ':';
            static const char * const Symbol;    //!< "<=>";
            static const char * const CallSign;  //!< "Equilibrium"

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibrium);
        };
    }

}


#endif
