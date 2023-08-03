
//! \file
#ifndef Y_HASHING_MD_INCLUDED
#define Y_HASHING_MD_INCLUDED 1


#include "y/config/starting.hpp"

namespace Yttrium
{

    namespace Memory { class ReadOnlyBuffer; }
    class Digest;

    namespace Hashing
    {
        class Function;

        //______________________________________________________________________
        //
        //
        //! message digests
        //
        //______________________________________________________________________
        struct MD
        {
            static Digest Of(Function &H);                                 //!< H content to digest
            static Digest Of(Function &H, const void * ,const size_t  );   //!< block to digest
            static Digest Of(Function &H, const char *);                   //!< text to digest
            static Digest Of(Function &H, const Memory::ReadOnlyBuffer &); //!< buffer to digest
        };


    }

}

#endif
