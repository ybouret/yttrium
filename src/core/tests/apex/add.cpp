
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


static inline
SignType CompareSpeed(const double lhs, const double rhs) noexcept
{
    return Sign::Of(rhs,lhs);
}

Y_UTEST(apex_add)
{
    Random::ParkMiller ran;

    unsigned       maxBits  = 4096;
    double         duration = 0.1;
    if(argc>1)     duration = ASCII::Convert::ToReal<double>(argv[1],"duration");


    uint64_t      tmx[Natural::NumOps] = { 0 };
    double        spd[Natural::NumOps] = { 0 };
    size_t        idx[Natural::NumOps] = { 0 };
    size_t        bin[Natural::NumOps] = { 0 };
    Y_STATIC_ZARR(tmx);
    Y_STATIC_ZARR(bin);

    typedef Vector<double> SVec;
    CxxArray<SVec>         svec(Natural::NumOps);

    WallTime chrono;
    {
        const String fn = "apex-add.dat";
        OutputFile::Overwrite(fn);
        for(unsigned lbits=1;lbits<=maxBits; lbits <<=1)
        {
            for(unsigned rbits=lbits;rbits<=maxBits; rbits <<=1)
            {
                const uint64_t start  = WallTime::Ticks();
                size_t         cycles = 0;
                do
                {
                    ++cycles;
                    Natural  lhs(ran,lbits);
                    Natural  rhs(ran,rbits);
                    for(unsigned i=0;i<Natural::NumOps;++i)
                    {
                        Ops      addOps = Natural::OpsTable[i];
                        lhs.plan(ran);
                        rhs.plan(ran);
                        const    uint64_t mark = WallTime::Ticks();
                        BlockPtr sum    = Natural::Add( *lhs, *rhs, addOps);
                        tmx[i] += WallTime::Ticks() - mark;
                        if(lbits<=63&&rbits<=63)
                        {
                            const uint64_t l64 = lhs.lsw();
                            const uint64_t r64 = rhs.lsw();
                            const uint64_t s64 = l64+r64;
                            Y_ASSERT( s64 == sum->make<Plan8>().word[0] );
                        }
                    }
                }
                while( chrono.since(start) < duration );

                for(unsigned i=0;i<Natural::NumOps;++i)
                {
                    spd[i] = double( static_cast<long double>(cycles)/chrono(tmx[i]) );
                }

                if(lbits==rbits)
                {
                    AppendFile fp(fn);
                    fp("%15g", log2(double(lbits)) );
                    for(unsigned i=0;i<Natural::NumOps;++i)
                    {
                        fp(" %.15g", log10(spd[i]) );
                        svec[i+1].pushTail( log10(spd[i]) );
                    }
                    fp << "\n";

                }

                Indexing::Tableau(idx, Natural::NumOps, CompareSpeed, spd);

                

                const String here = Formatted::Get("%u.%u",lbits,rbits);
                std::cerr <<  std::setw(11) << here.c_str();
                const size_t best = idx[0];
                for(unsigned i=0;i<Natural::NumOps;++i)
                {
                    char pfx = ' ';
                    if(best==i)
                    {
                        pfx='*';
                        ++bin[i];
                    }
                    std::cerr << "|"  << pfx << HumanReadable( uint64_t(spd[i]) );
                }
                std::cerr << std::endl;

            }
        }

        std::cerr << "           ";
        for(unsigned i=0;i<Natural::NumOps;++i)
        {
            std::cerr << "|   " << Natural::OpsLabel[i];
        }
        std::cerr << std::endl;
        size_t ntot = 0;
        for(size_t i=0;i<Natural::NumOps;++i)
        {
            ntot += bin[i];
        }

        MKL::Antelope::Add<double> xadd;
        for(size_t i=0;i<Natural::NumOps;++i)
        {
            std::cerr << "\t--> " << Natural::OpsLabel[i] << " @ ";
            const double percent = floor( double(bin[i]) * 10000.0 / double(ntot) + 0.5) / 100;
            std::cerr << Formatted::Get("%6.02f%%",percent);
            const double score   = xadd.average( svec[i+1] );
            std::cerr << " score = " << std::setw(15) << score;
            std::cerr << std::endl;
        }
        std::cerr << "plot";
        for(unsigned i=0;i<Natural::NumOps;++i)
        {
            if(i>0) std::cerr << ",";
            std::cerr << " '";
            if(i==0) std::cerr << fn;
            std::cerr << "' u 1:" << i+2 << " w lp";
        }
        std::cerr << std::endl;

    }




}
Y_UDONE()
