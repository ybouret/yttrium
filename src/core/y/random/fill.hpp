// \file

#ifndef Y_Random_Fill_Included
#define Y_Random_Fill_Included 1

#include "y/random/bits.hpp"
#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{
    namespace Random
    {
        //______________________________________________________________________
        //
        //
        //! filling with random content
        //
        //______________________________________________________________________
        struct Fill
        {
            //! each byte in 0x00..0xff
            static void Block(void  *       blockAddr,
                              size_t        blockSize,
                              Bits &        ran,
                              const uint8_t lo=0x00,
                              const uint8_t hi=0xff) noexcept;

            //! fill object with positive bytes
            template <typename T> static inline
            void Fuzzy(T &obj, Bits &ran) noexcept
            {
                Block(Memory::OutOfReach::Addr(&obj), sizeof(T), ran,0x01,0xff);
            }
            
        };

    }

}

#endif

