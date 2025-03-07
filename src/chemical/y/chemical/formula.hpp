//! \file


#ifndef Y_Chemical_Formula_Included
#define Y_Chemical_Formula_Included 1


#include "y/chemical/type/xcode.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        class Formula
        {
        public:
            static const char * const CallSign;
            static const char * const Name; //!< "Name"
            static const char * const Body; //!< "Body"
            static const char * const Mult; //!< "Mult"
            static const char * const Z;    //!< "Z"
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Formula);
        };
    }

}


#endif

