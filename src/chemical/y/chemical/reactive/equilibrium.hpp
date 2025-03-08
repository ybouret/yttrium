
//! \file


#ifndef Y_Chemical_Equilibrium_Included
#define Y_Chemical_Equilibrium_Included 1


#include "y/chemical/reactive/actors.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Interface for equilibrium
        //
        //
        //______________________________________________________________________
        class Equilibrium
        {
        public:
            static const char         Prefix    = '@'; //!< for parser
            static const char         Separator = ':'; //!< for parser
            static const char * const Symbol;    //!< "<=>";
            static const char * const Reac;      //!< "Reac";
            static const char * const Prod;      //!< "Prod";

            static const char * const CallSign;  //!< "Equilibrium"

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibrium);
        };
    }

}


#endif
