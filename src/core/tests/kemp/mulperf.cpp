
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

    uint64_t tmxStd = 0, tmxKar = 0, tmxFFT = 0;

    OutputFile fpStd("mulstd.dat");
    OutputFile fpKar("mulkar.dat");
    OutputFile fpFFT("mulfft.dat");


    for(unsigned i=8;i<=maxBits;i *= 2)
    {

        std::cerr << std::setw(4) << i << "[";
        for(unsigned j=8;j<=maxBits; j *= 2)
        {
            (std::cerr << '*').flush();

            size_t  cycles = 0;
            tmxStd = 0;
            tmxKar = 0;
            tmxFFT = 0;
            do
            {
                ++cycles;
                Element L(i,ran);
                Element R(j,ran);

                {
                    L.set( Element::Inner[ ran.in<size_t>(0,3)] );
                    R.set( Element::Inner[ ran.in<size_t>(0,3)] );
                    AutoPtr<Element> P = Element::Mul[Ops64_32](L,R,tmxStd);
                }

                {
                    L.set( Element::Inner[ ran.in<size_t>(0,3)] );
                    R.set( Element::Inner[ ran.in<size_t>(0,3)] );
                    AutoPtr<Element> P = Element::Kar[Ops64_32](L,R,tmxKar);
                }

                {
                    AutoPtr<Element> P = Element::MulFFT(L,R,tmxFFT);
                }


            } while( chrono(tmxFFT) < maxTmx);

            fpStd("%u %u",i,j);
            fpKar("%u %u",i,j);
            fpFFT("%u %u",i,j);

            {
                const long double ell  = chrono(tmxStd);
                const double      rate = double(cycles/ell);
                fpStd(" %.15g",rate);
            }


            {
                const long double ell  = chrono(tmxKar);
                const double      rate = double(cycles/ell);
                fpKar(" %.15g",rate);
            }

            {
                const long double ell  = chrono(tmxFFT);
                const double      rate = double(cycles/ell);
                fpFFT(" %.15g",rate);
            }


            fpStd << '\n';
            fpStd.flush();

            fpKar << '\n';
            fpKar.flush();

            fpFFT << '\n';
            fpFFT.flush();

        }
        fpStd << '\n';
        fpKar << '\n';
        fpFFT << '\n';

        std::cerr << "]" << std::endl;
    }


}
Y_UDONE()

