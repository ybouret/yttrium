//! \file

#ifndef Y_Apex_Natural_Cxx_Included
#define Y_Apex_Natural_Cxx_Included 1

#include "y/apex/n/proto.hpp"

namespace Yttrium
{
    namespace Apex
    {
        typedef UnsignedInt<sizeof(void*)>::Type   SystemCore; //!< alias
        typedef UnsignedInt<sizeof(void*)/2>::Type SystemWord; //!< alias
        typedef Proto<SystemCore,SystemWord>       Prototype;  //!< alias

        //! helper to convert impl to Prototype
#define PROTO(ADDR)       (*static_cast<Prototype *>(ADDR))

        //! helper to convert CONST impl to Prototype
#define CONST_PROTO(ADDR) (*static_cast<const Prototype *>(ADDR))
    }
}

#endif

