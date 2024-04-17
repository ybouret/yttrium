
#include "y/system/seed.hpp"
#include "y/system/wtime.hpp"
#include "y/hashing/ibj64.hpp"
#include "y/check/crc32.hpp"
#include "y/system/pid.hpp"
#include "y/type/bitlib.hpp"
#include "y/lockable.hpp"

#include <iostream>

namespace Yttrium
{

    uint32_t SystemSeed:: Get()
    {
        Y_GIANT_LOCK();
        union
        {
            uint64_t qw[2];
            uint32_t dw[4];
        } alias = { { WallTime::Ticks(), BitLib::LRoll(ProcessId::Get()) } };
        for(size_t i=0;i<4;++i) alias.dw[i] = Hashing::IBJ32(alias.dw[i]);

        const uint32_t seed = CRC32::Of(&alias,sizeof(alias));
        std::cerr << "seed=" << seed << std::endl;
        return seed;
    }
}
