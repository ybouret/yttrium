
#include "y/system/seed.hpp"
#include "y/system/wtime.hpp"
#include "y/hashing/ibj64.hpp"
#include "y/check/crc32.hpp"
#include "y/system/pid.hpp"
#include "y/lockable.hpp"
#include "y/text/hexadecimal.hpp"
#include <iostream>

namespace Yttrium
{

    static inline void scatter32(uint32_t &a,
                                 uint32_t &b)
    {
        union
        {
            uint32_t u32;
            uint8_t  u8[4];
        } A = { a }, B = { b };

        //std::cerr << Hexadecimal(a) << ":" << Hexadecimal(b) << " => ";
        Swap(A.u8[0],B.u8[0]);
        Swap(A.u8[2],B.u8[2]);
        a = A.u32;
        b = B.u32;
        //std::cerr << Hexadecimal(a) << ":" << Hexadecimal(b) << std::endl;
    }

    uint32_t SystemSeed:: Get()
    {
        Y_GIANT_LOCK();

        union
        {
            uint64_t qw[2];
            uint32_t dw[4];
        } alias = { { WallTime::Ticks(), ProcessId::Get() } };
        //std::cerr << "qw=" << Hexadecimal(alias.qw[0]) << ":" << Hexadecimal(alias.qw[1]) << std::endl;
        scatter32(alias.dw[0], alias.dw[1]);
        scatter32(alias.dw[2], alias.dw[3]);
        //std::cerr << "qw=" << Hexadecimal(alias.qw[0]) << ":" << Hexadecimal(alias.qw[1]) << std::endl;

        const uint32_t seed = CRC32::Of(&alias,sizeof(alias));
        //std::cerr << "seed=0x" << Hexadecimal(seed) << std::endl;
        return seed;
    }
}
