
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

using namespace Yttrium;
using namespace Apex;


static inline
SignType CompareSpeed(const double lhs, const double rhs) noexcept
{
    return Sign::Of(rhs,lhs);
}

Y_UTEST(apex_add)
{
    Random::ParkMiller ran;

    unsigned        maxBits  = 1024;
    double         duration = 0.1;
    if(argc>1)     duration = ASCII::Convert::ToReal<double>(argv[1],"duration");


    uint64_t      tmx[Natural::NumAddOps] = { 0 };
    double        spd[Natural::NumAddOps] = { 0 };
    size_t        idx[Natural::NumAddOps] = { 0 };
    Y_STATIC_ZARR(tmx);

    WallTime chrono;
    {
        const String fn = "apex-add.dat";
        OutputFile::Overwrite(fn);
        for(unsigned lbits=1;lbits<=maxBits; lbits <<=1)
        {
            for(unsigned rbits=lbits;rbits<=maxBits; rbits <<=1)
            {
                const uint64_t ini    = WallTime::Ticks();
                uint64_t       end    = ini;
                size_t         cycles = 0;
                double         timing = 0;
                do
                {
                    ++cycles;
                    Natural  lhs(ran,lbits);
                    Natural  rhs(ran,rbits);
                    for(unsigned i=0;i<Natural::NumAddOps;++i)
                    {
                        AddOps   addOps = Natural::AddOpsTable[i];
                        BlockPtr sum    = Natural::Add(lhs._block(), rhs._block(), addOps, &tmx[i]);
                        if(lbits<=63&&rbits<=63)
                        {
                            const uint64_t l64 = lhs.lsw();
                            const uint64_t r64 = rhs.lsw();
                            const uint64_t s64 = l64+r64;
                            Y_ASSERT( s64 == sum->make<Plan8>().word[0] );
                        }
                    }
                    end    = WallTime::Ticks();
                    timing = chrono(end-ini);
                }
                while(  timing < duration );

                for(unsigned i=0;i<Natural::NumAddOps;++i)
                {
                    spd[i] = double( static_cast<long double>(cycles)/chrono(tmx[i]) );
                }

                AppendFile fp(fn);
                fp("%u %u", unsigned(lbits), unsigned(rbits));
                for(unsigned i=0;i<Natural::NumAddOps;++i)
                {
                    fp(" %.15g", spd[i]);
                }
                fp << "\n";

                Indexing::Tableau(idx, Natural::NumAddOps, CompareSpeed, spd);

                

                const String here = Formatted::Get("%u.%u",lbits,rbits);
                std::cerr <<  std::setw(11) << here.c_str();
                const size_t best = idx[0];
                for(unsigned i=0;i<Natural::NumAddOps;++i)
                {
                    char pfx = ' ';
                    if(best==i) pfx='*';
                    std::cerr << "|"  << pfx << HumanReadable( uint64_t(spd[i]) );
                }
                std::cerr << std::endl;

            }
        }

        std::cerr << "           ";
        for(unsigned i=0;i<Natural::NumAddOps;++i)
        {
            std::cerr << "|   " << Natural::AddOpsLabel[i];
        }
        std::cerr << std::endl;
    }
}
Y_UDONE()
