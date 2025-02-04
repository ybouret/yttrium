#include "y/apex/natural.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/system/wtime.hpp"
#include "y/text/human-readable.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/sequence/vector.hpp"
#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace Apex;

Y_UTEST(apex_dft)
{

    long double duration = 0.1;

    Random::ParkMiller ran;
    WallTime           chrono;

    if(argc>1)
    {
        duration = ASCII::Convert::ToReal<long double>(argv[1],"duration");
    }

    Vector<double> speed;
    Vector<size_t> nbits;

    const String fn = "dft.dat";
    OutputFile::Overwrite(fn);

    const size_t maxBits = 4096;
    for(size_t lbits=1;lbits<=maxBits;lbits <<= 1)
    {
        for(size_t rbits=lbits;rbits<=maxBits;rbits <<= 1 )
        {
            (std::cerr << std::setw(5) << lbits << '.' << std::setw(5) << rbits).flush();
            const Natural lhs(ran,lbits);
            const Natural rhs(ran,rbits);
            const uint64_t outTMX = WallTime::Ticks();
            uint64_t      mulTMX = 0;
            uint64_t      fftTMX = 0;
            size_t        cycles = 0;
            do
            {
                ++cycles;
                const Natural mul( Natural::Mul( Coerce(*lhs), Coerce(*rhs), Natural::MulOps, &mulTMX), AsBlock );
                const Natural fft( Natural::FFT( Coerce(*lhs), Coerce(*rhs), &fftTMX),                  AsBlock );
                Y_ASSERT(mul==fft);
            } while( chrono( WallTime::Ticks() - outTMX) < duration );
            const long double mulSpeed = static_cast<long double>(cycles) / chrono(mulTMX);
            const long double fftSpeed = static_cast<long double>(cycles) / chrono(fftTMX);
            std::cerr << " | mul " << HumanReadable(mulSpeed);
            std::cerr << " | fft " << HumanReadable(fftSpeed);

            std::cerr << std::endl;

            if(lbits==rbits)
            {
                nbits << lbits;
                speed << fftSpeed;
                AppendFile fp(fn);
                fp("%.15g %.15g %.15g\n", log10(double(lbits)), log10(double(fftSpeed)), log10(double(mulSpeed)) );
            }

        }
    }


}
Y_UDONE()

