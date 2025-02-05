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

    Vector<double> fspeed;
    Vector<double> mspeed;
    Vector<size_t> bits;

    const String fn = "dft.dat";
    OutputFile::Overwrite(fn);

    const size_t minBits = 256;
    const size_t maxBits = 32768;
    for(size_t lbits=minBits;lbits<=maxBits;lbits <<= 1)
    {
        for(size_t rbits=lbits;rbits<=maxBits;rbits <<= 1 )
        {
            (std::cerr << std::setw(5) << lbits << '.' << std::setw(5) << rbits).flush();

            const uint64_t started = WallTime::Ticks();
            uint64_t       mulTMX = 0;
            uint64_t       fftTMX = 0;
            size_t         cycles = 0;
            do
            {
                Natural lhs(ran,lbits);
                Natural rhs(ran,rbits);
                lhs.plan(ran);
                rhs.plan(ran);
                ++cycles;
                // timing long multiplication
                const uint64_t mmark = WallTime::Ticks();
                const Natural mul( Natural::Mul( Coerce(*lhs), Coerce(*rhs), Natural::MulOps), AsBlock );
                mulTMX += WallTime::Ticks() - mmark;

                // timing FFT multiplication
                lhs.plan(ran);
                rhs.plan(ran);
                const uint64_t fmark = WallTime::Ticks();
                const Natural fft( Natural::FFT( Coerce(*lhs), Coerce(*rhs) ), AsBlock );
                fftTMX += WallTime::Ticks() - fmark;


                Y_ASSERT(mul==fft);
            } while( chrono.since(started) < duration );


            const long double mulSpeed = static_cast<long double>(cycles) / chrono(mulTMX);
            const long double fftSpeed = static_cast<long double>(cycles) / chrono(fftTMX);
            std::cerr << " | mul " << HumanReadable(mulSpeed);
            std::cerr << " | fft " << HumanReadable(fftSpeed);
            if(fftSpeed>=mulSpeed) std::cerr << " (*)";


            std::cerr << std::endl;

            if(lbits==rbits)
            {
                bits << lbits;
                fspeed << fftSpeed;
                mspeed << mulSpeed;
                AppendFile fp(fn);
                fp("%.15g %.15g %.15g\n", log10(double(lbits)), log10(double(fftSpeed)), log10(double(mulSpeed)) );
            }
        }
    }

    const size_t n = bits.size();
    for(size_t i=1;i<n;++i)
    {
        const double f1 = fspeed[i+1];
        const double m1 = mspeed[i+1];

        if(f1>=m1)
        {
            std::cerr << "crossed before " << bits[i+1] << std::endl;
            const double f0 = fspeed[i];
            const double m0 = mspeed[i];
            const double x0 = bits[i];
            const double x1 = bits[i+1];

            const double num = -( (m0-f0)*x1+(f1-m1)*x0 );
            const double den = m1-m0-f1+f0;
            const double xi = floor(num/den+0.5);;
            std::cerr << "crossover@" << xi << std::endl;
            break;
        }

    }


}
Y_UDONE()

