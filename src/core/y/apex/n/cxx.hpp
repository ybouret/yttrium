//! \file

#ifndef Y_Apex_Natural_Cxx_Included
#define Y_Apex_Natural_Cxx_Included 1

#include "y/apex/n/proto.hpp"

namespace Yttrium
{
    namespace Apex
    {
        typedef UnsignedInt<sizeof(void*)>::Type   SystemCore;
        typedef UnsignedInt<sizeof(void*)/2>::Type SystemWord;

        typedef Proto<SystemCore,SystemWord> Prototype;

#define PROTO(ADDR)       (*static_cast<Prototype *>(ADDR))
#define CONST_PROTO(ADDR) (*static_cast<const Prototype *>(ADDR))
    }
}

#endif

