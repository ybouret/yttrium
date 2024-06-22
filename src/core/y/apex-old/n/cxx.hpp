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
        typedef Prototype::Pointer                 AutoProto;  //!< alias

        //! helper to convert impl to Prototype
#define PROTO(APN)       (*static_cast<Prototype *>( (APN).impl ) )

        //! helper to convert CONST impl to Prototype
#define CONST_PROTO(APN) (*static_cast<const Prototype *>( (APN).impl ) )
    }
}

#endif

