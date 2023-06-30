//! \file

#ifndef Y_System_AtExit_Included
#define Y_System_AtExit_Included 1


#include "y/type/ints.hpp"

namespace Yttrium
{

    struct AtExit
    {
        static const char * const CallSign;

        typedef void *       Arguments;
        typedef void       (*Procedure)(Arguments);
        typedef int          Longevity;
        static const size_t    MaxNumber = 64;
        static const Longevity MaximumLongevity = IntegerFor<AtExit::Longevity>::Maximum;
        
        Procedure procedure;
        Arguments arguments;
        Longevity longevity;

        static void Register(Procedure,Arguments,Longevity);

    };

}

#endif
