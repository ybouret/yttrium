

#include "y/apex/natural.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/system/wtime.hpp"
#include "y/sequence/vector.hpp"
#include "y/stream/libc/output.hpp"
#include "y/string.hpp"
#include "y/text/human-readable.hpp"
#include "y/sort/indexing.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/container/cxx/array.hpp"
#include "y/mkl/antelope/add.hpp"

using namespace Yttrium;
using namespace Apex;

Y_UTEST(apex_sub)
{
    double duration  = 0.1;

    Random::ParkMiller ran;
    uint64_t           tmx[Natural::NumOps];
    double             spd[Natural::NumOps];
    WallTime           chrono;
    Y_STATIC_ZARR(tmx);
    Y_STATIC_ZARR(spd);

    for(unsigned lbits=1;lbits<=4096;lbits <<= 1)
    {
        for(unsigned rbits=1;rbits<=lbits;rbits <<= 1)
        {
            std::cerr << std::setw(4) << lbits << '.' << std::setw(4) << rbits;

            size_t         cycles = 0;
            const uint64_t tstart = WallTime::Ticks();

            do
            {
                ++cycles;
                Natural lhs(ran,lbits);
                Natural rhs(ran,rbits);
                if(rhs>lhs) lhs.xch(rhs);
                Y_ASSERT(lhs>=rhs);

                for(unsigned i=0;i<Natural::NumOps;++i)
                {
                    const Ops         ops = Natural::OpsTable[i];
                    volatile BlockPtr blk( Natural::Sub(lhs._block(), rhs._block(), ops, &tmx[i]) );
                }


            }
            while( chrono( WallTime::Ticks() - tstart) < duration );


            for(unsigned i=0;i<Natural::NumOps;++i)
            {
                spd[i] = double( static_cast<long double>(cycles)/chrono(tmx[i]) );
                std::cerr << " | " << HumanReadable( static_cast<uint64_t>(spd[i]) );
            }

            std::cerr << std::endl;
        }
    }

}
Y_UDONE()
