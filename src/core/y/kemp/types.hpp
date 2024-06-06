//! \file


#ifndef Y_Kemp_Types_Included
#define Y_Kemp_Types_Included 1

#include "y/config/shallow.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        Y_SHALLOW_DECL(Exp2); //!< alias


        //______________________________________________________________________
        //
        //
        //! internal operations interface
        //
        //______________________________________________________________________
        enum Ops
        {
            // 64 bits core
            Ops64_32, //!< CORE=uint64_t, WORD=uint32_t
            Ops64_16, //!< CORE=uint64_t, WORD=uint16_t
            Ops64_8,  //!< CORE=uint64_t, WORD=uint8_t
            Ops32_16, //!< CORE=uint32_t, WORD=uint16_t
            Ops32_8,  //!< CORE=uint32_t, WORD=uint8_t
            Ops16_8   //!< CORE=uint16_t, WORD=uint8_t
        };
    }

}

#endif
