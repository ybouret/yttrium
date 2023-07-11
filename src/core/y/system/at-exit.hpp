//! \file

#ifndef Y_System_AtExit_Included
#define Y_System_AtExit_Included 1


#include "y/type/ints.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //! customized atexit
    //
    //__________________________________________________________________________
    struct AtExit
    {
        //______________________________________________________________________
        //
        //
        // definitions
        //
        //______________________________________________________________________
        static const char * const CallSign;               //!< "AtExit"
        typedef void *            Arguments;              //!< alias
        typedef void            (*Procedure)(Arguments);  //!< alias
        typedef int               Longevity;              //!< alias
        static const size_t       MaxNumber =   64;       //!< internal stack size
        static const Longevity    MaximumLongevity = IntegerFor<Longevity>::Maximum; //!< for giant mutex

        //______________________________________________________________________
        //
        //
        //! registering procedure(arguments) with its longevity
        /**
         no similar longevities are accepted
         */
        //
        //______________________________________________________________________
        static void Register(Procedure,Arguments,Longevity);


        //______________________________________________________________________
        //
        //
        // fields
        //
        //______________________________________________________________________
        Procedure procedure; //!< procedure to call
        Arguments arguments; //!< arguments for procedure
        Longevity longevity; //!< longevity: the higher, the later call


    };

}

#endif
