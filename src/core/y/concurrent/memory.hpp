
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
        class Quanta;
    };

    namespace Concurrent
    {
        //! accessing internal memory objects
        struct Mem
        {
            static Memory::Straps &StrapsInstance(); //!< Quark.straps
            static Memory::Corpus &CorpusInstance(); //!< Quark.corpus
            static Memory::Quanta &QuantaInstance(); //!< Quark.quanta
        };
    }

}

#endif

