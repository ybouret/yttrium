
#include "y/kemp/element.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/ptr/auto.hpp"
#include "y/system/wtime.hpp"
#include "y/text/human-readable.hpp"
#include <cstring>

#include "y/text/ascii/convert.hpp"
#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace Kemp;

Y_UTEST(kemp_mulperf)
{
    Random::ParkMiller ran;
    WallTime           chrono;
    double             maxTmx  = 0.05;
    unsigned           maxBits = 4096;

    uint64_t tmxOps[Element::Kinds+1] = { 0 };
    uint64_t & tmxFFT = tmxOps[Element::Kinds];

    OutputFile fp("mulperf.dat");
    for(unsigned i=8;i<=maxBits;i *= 2)
    {

        std::cerr << std::setw(4) << i << "[";
        for(unsigned j=8;j<=maxBits; j *= 2)
        {
            (std::cerr << '*').flush();

            size_t  cycles = 0;
            memset(tmxOps,0,sizeof(tmxOps));
            do
            {
                ++cycles;
                Element L(i,ran);
                Element R(j,ran);
                for(unsigned k=0;k<Element::Kinds;++k)
                {
                    L.set( Element::Inner[ ran.in<size_t>(0,3)] );
                    R.set( Element::Inner[ ran.in<size_t>(0,3)] );
                    AutoPtr<Element> P = Element::Mul[k](L,R,tmxOps[k]);
                }
                {
                    AutoPtr<Element> P = Element::MulFFT(L,R,tmxFFT);
                }
            } while( chrono(tmxFFT) < maxTmx);

            fp("%u %u",i,j);
            for(size_t k=0;k<=Element::Kinds;++k)
            {
                const long double ell  = chrono(tmxOps[k]);
                const double      rate = double(cycles/ell);
                fp(" %.15g",rate);
            }
            fp << '\n';
            fp.flush();
        }
        fp << '\n';
        std::cerr << "]" << std::endl;
    }


}
Y_UDONE()

