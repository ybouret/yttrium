#include "y/mkl/numeric.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/system/rtti.hpp"

using namespace Yttrium;

namespace Yttrium
{

    template <typename T> struct DFT_Real        { typedef T      Type; };
    template <>           struct DFT_Real<float> { typedef double Type; };

    struct DFT
    {


        /**
         Replaces data[1..2*nn] by its Discrete Fourier transform, if isign is input as 1; or replaces
         data[1..2*nn] by nn times its inverse discrete Fourier transform, if isign is input as−1.
         data is a complex array of length nn or, equivalently, a real array of length 2*nn. nn MUST
         be an integer power of 2
         */
        static inline
        void Transform_(float         data[],
                        unsigned long nn,
                        const int     isign)
        {
            unsigned long n,mmax,m,j,istep,i;
            double wtemp,wr,wpr,wpi,wi,theta;
            float tempr,tempi;

            n = nn << 1;
            j = 1;
            for (i=1;i<n;i+=2)
            {
                if (j > i)
                {
                    Swap(data[j],data[i]);
                    Swap(data[j+1],data[i+1]);
                }
                m=nn;
                while (m >= 2 && j > m)
                {
                    j -= m;
                    m >>= 1;
                }
                j += m;
            }

            mmax=2;
            while(n>mmax)
            {
                istep=mmax << 1;
                theta = isign*(6.28318530717959/mmax);
                wtemp = sin(0.5*theta);
                wpr   = -2.0*wtemp*wtemp;
                wpi   = sin(theta);
                wr    = 1.0;
                wi    = 0.0;
                for (m=1;m<mmax;m+=2)
                {
                    for (i=m;i<=n;i+=istep)
                    {
                        j=i+mmax;
                        tempr=wr*data[j]-wi*data[j+1];
                        tempi=wr*data[j+1]+wi*data[j];
                        data[j]=data[i]-tempr;
                        data[j+1]=data[i+1]-tempi;
                        data[i] += tempr;
                        data[i+1] += tempi;
                    }
                    wr=(wtemp=wr)*wpr-wi*wpi+wr;
                    wi=wi*wpr+wtemp*wpi+wi;
                }
                mmax=istep;
            }
        }


        /**
         - Replaces data[1..2*nn] by its Discrete Fourier transform if isign is input as 1
         - or replaces data[1..2*nn] by nn times its inverse discrete Fourier transform, if isign is input as −1.
         - data is a complex array of length nn or, equivalently, a real array of length 2*nn
         - nn MUSTbe an integer power of 2
         */
        template <typename T> static inline
        void Transform(T            data[],
                       const size_t size,
                       const int    isign)
        {
            typedef typename DFT_Real<T>::Type long_T;
            static const long_T  long_PI = MKL::Numeric<long_T>::PI;
            const size_t n = size << 1;
            for(size_t i=1,j=1;i<n;i+=2)
            {
                if(j>i)
                {
                    Swap(data[j],data[i]);
                    Swap(data[j+1],data[i+1]);
                }
                size_t m=size;
                while (m >= 2 && j > m)
                {
                    j -= m;
                    m >>= 1;
                }
                j += m;
            }

            std::cerr << "size=" << size << std::endl;
            size_t mmax=2;
            while(n>mmax)
            {
                const size_t istep = mmax << 1;
                //const long_T theta = isign*(6.28318530717959/mmax);
                std::cerr << "\tmmax/2=" << (mmax>>1) << std::endl;
                const long_T theta = isign*(long_PI/(mmax>>1));
                long_T       wtemp = sin(0.5*theta);
                long_T       wpr   = -2.0*wtemp*wtemp;
                long_T       wpi   = sin(theta);
                long_T       wr    = 1;
                long_T       wi    = 0;
                for(size_t m=1;m<mmax;m+=2)
                {
                    for(size_t i=m;i<=n;i+=istep)
                    {
                        const size_t j  = i+mmax;
                        const size_t i1 = i+1;
                        const size_t j1 = j+1;
                        const T tempr = static_cast<T>(wr*data[j]  - wi*data[j1]);
                        const T tempi = static_cast<T>(wr*data[j1] + wi*data[j]);
                        data[j]       = data[i]-tempr;
                        data[j1]      = data[i1]-tempi;
                        data[i]      += tempr;
                        data[i1]     += tempi;
                    }
                    wr=(wtemp=wr)*wpr-wi*wpi+wr;
                    wi=wi*wpr+wtemp*wpi+wi;
                }
                mmax=istep;
            }
        }
    };

}

#include "y/random/park-miller.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/type/utils.hpp"
#include "y/system/wtime.hpp"
#include "y/text/human-readable.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/stream/libc/output.hpp"

namespace
{
    static double duration = 0.01;

    template <typename T> static inline
    double DFT_Test(const unsigned p,
                    Random::Bits  &ran,
                    T             &rms)
    {
        const size_t   n = 1<<p;
        Vector<T>               data(n*2,0), orig(n*2,0);
        MKL::Antelope::Add<T>  xadd(data.size());
        WallTime               chrono;

        rms                  = 0;
        uint64_t    tmx      = 0;
        size_t      cycles   = 0;
        long double ellapsed = 0;
        do
        {
            ++cycles;
            for(size_t i=data.size();i>0;--i)
            {
                data[i] = orig[i] = ran.symm<T>();
            }

            {
                const uint64_t ini = WallTime::Ticks();
                DFT::Transform(data()-1,n,1);
                DFT::Transform(data()-1,n,-1);
                tmx += WallTime::Ticks() - ini;
            }

            xadd.free();
            for(size_t i=data.size();i>0;--i)
            {
                xadd << Squared( data[i] / n - orig[i] );
            }
            const T tmp = sqrt( xadd.sum() / n );
            if(tmp>rms) rms = tmp;
            ellapsed = chrono(tmx);
        }
        while( ellapsed < duration );

        //std::cerr << "rms=" << rms << std::endl;
        //std::cerr << "#cycle=" << cycles << " in " << ellapsed << std::endl;

        return static_cast<double>( static_cast<long double>(cycles) / ellapsed );
    }
}

Y_UTEST(dft_core)
{
    Random::ParkMiller ran;

    if( argc > 1)
    {
        duration = ASCII::Convert::ToReal<double>(argv[1],"duration");
    }

    std::cerr << "float => " << RTTI::Name< DFT_Real<float>::Type >() << std::endl;
    std::cerr << "double => " << RTTI::Name< DFT_Real<double>::Type >() << std::endl;
    std::cerr << "long double => " << RTTI::Name< DFT_Real<long double>::Type >() << std::endl;


    for(unsigned p=0;p<5;++p)
    {
        const size_t   n = 1<<p;
        Vector<float> data(n*2,0);
        for(size_t i=1;i<=data.size();++i)
            data[i] = i;

        std::cerr << data << std::endl;
        DFT::Transform_(data()-1,n,1);
        DFT::Transform_(data()-1,n,-1);
        for(size_t i=1;i<=data.size();++i)
        {
            data[i] = floor( data[i]/n + 0.5);
        }
        std::cerr << data << std::endl;
        std::cerr << std::endl;
    }

    for(unsigned p=0;p<6;++p)
    {
        const size_t   n = 1<<p;
        Vector<float> data(n*2,0);
        for(size_t i=1;i<=data.size();++i)
            data[i] = i;

        std::cerr << data << std::endl;
        DFT::Transform(data()-1,n,1);
        //std::cerr << data << std::endl;
        DFT::Transform(data()-1,n,-1);
        for(size_t i=1;i<=data.size();++i)
        {
            data[i] = floor( data[i]/n + 0.5);
        }
        std::cerr << data << std::endl;
        std::cerr << std::endl;
    }

    const String fn = "dft.dat";
    OutputFile::Overwrite(fn);

    const double factor = 1;
    for(unsigned p=0;p<=10;++p)
    {
        (std::cerr << "2^" << std::setw(2) << p << " = " << std::setw(5) << (1<<p)).flush();
        float  rms32 = 0;
        double rms64 = 0;
        long double rmsXX = 0;

        const double spd32 = DFT_Test<float>(p,ran,rms32);
        (std::cerr << " | <flt> " << HumanReadable( factor * spd32 ) << " (" << Formatted::Get("%8.3g",double(rms32)) << ")").flush();
        const double spd64 = DFT_Test<double>(p,ran,rms64);
        (std::cerr << " | <dbl> " << HumanReadable( factor * spd64 ) << " (" << Formatted::Get("%8.3g",double(rms64)) << ")").flush();

        const double spdXX = DFT_Test<long double>(p,ran,rmsXX);
        (std::cerr << " | <ldbl> " << HumanReadable( factor * spdXX ) << " (" << Formatted::Get("%8.3g",double(rmsXX)) << ")").flush();

        std::cerr << std::endl;

        AppendFile fp(fn);
        fp("%u %.15g %.15g %.15g\n", p, log10(spd32), log10(spd64), log10(spdXX) );

    }


}
Y_UDONE()


