
//! \file

#ifndef Y_Concurrent_Quark_Included
#define Y_Concurrent_Quark_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Memory
    {
        class Straps;
        class Corpus;
    };

    namespace Concurrent
    {
        struct Mem
        {
            static Memory::Straps &StrapsInstance();
            static Memory::Corpus &CorpusInstance();

        };
    }

}

#endif

