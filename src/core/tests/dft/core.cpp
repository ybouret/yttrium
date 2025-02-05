#include "y/dft/dft.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/system/rtti.hpp"
#include "y/type/utils.hpp"
#include <cstdio>

using namespace Yttrium;

#include "y/random/park-miller.hpp"
#include "y/mkl/antelope/add.hpp"
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

        const size_t           n = 1<<p;
        Vector<T>              data(n*2,0), orig(n*2,0);
        MKL::Antelope::Add<T>  xadd(data.size());
        WallTime               chrono;

        rms                  = 0;
        uint64_t    tmx      = 0;
        size_t      cycles   = 0;
        uint64_t    out      = chrono.Ticks();
        do
        {
            ++cycles;
            for(size_t i=data.size();i>0;--i)
            {
                data[i] = orig[i] = ran.symm<T>();
            }

            {
                const uint64_t ini = WallTime::Ticks();
                DFT::Forward(data()-1,n);
                DFT::Reverse(data()-1,n);
                tmx += WallTime::Ticks() - ini;
            }

            xadd.free();
            for(size_t i=data.size();i>0;--i)
            {
                xadd << Squared( data[i] / n - orig[i] );
            }
            const T tmp = sqrt( xadd.sum() / n );
            if(tmp>rms) rms = tmp;

        }
        while( chrono(chrono.Ticks()-out) < duration );


        long double ellapsed = chrono(tmx);
        return static_cast<double>( static_cast<long double>(cycles) / ellapsed );
    }


    template <typename T> static inline
    void DFT_TestDual(const unsigned p,
                      Random::Bits  &ran)
    {
        const size_t  n = 1<<p;
        std::cerr << RTTI::Name<T>() << " / " << n << std::endl;
        {
            Vector<T> data1(n*2,0);
            Vector<T> data2(n*2,0);
            Vector<T> copy1(n*2,0);
            Vector<T> copy2(n*2,0);

            for(size_t i=data1.size();i>0;--i)
            {
                data1[i] = data2[i] = copy1[i] = copy2[i] = ran.symm<T>();
            }

            Y_ASSERT( 0 == memcmp( data1(), copy1(), data1.size() * sizeof(T) ));
            Y_ASSERT( 0 == memcmp( data1(), copy1(), data1.size() * sizeof(T) ));

            DFT::Forward(data1()-1,data2()-1,n);
            DFT::Forward(copy1()-1,n);
            DFT::Forward(copy2()-1,n);

            Y_ASSERT( 0 == memcmp( data1(), copy1(), data1.size() * sizeof(T) ));
            Y_ASSERT( 0 == memcmp( data1(), copy1(), data1.size() * sizeof(T) ));
        }


        {
            Vector<T> data1(n,0);
            Vector<T> data2(n,0);
            Vector<T> copy1(n,0);
            Vector<T> copy2(n,0);

            for(size_t i=data1.size();i>0;--i)
            {
                data1[i] = data2[i] = copy1[i] = copy2[i] = ran.symm<T>();
            }

            Y_ASSERT( 0 == memcmp( data1(), copy1(), data1.size() * sizeof(T) ));
            Y_ASSERT( 0 == memcmp( data1(), copy1(), data1.size() * sizeof(T) ));

            DFT::RealForward(data1()-1, data2()-1, n);
            DFT::RealForward(copy1()-1,n);
            DFT::RealForward(copy2()-1,n);

            Y_ASSERT( 0 == memcmp( data1(), copy1(), data1.size() * sizeof(T) ));
            Y_ASSERT( 0 == memcmp( data1(), copy1(), data1.size() * sizeof(T) ));
        }



    }
}

#include "y/container/matrix.hpp"

static inline
double linfit(const Readable<double> &x, const Readable<double> &y)
{
    assert(x.size()>0);
    assert(x.size() == y.size() );

    const size_t               n = x.size();
    MKL::Antelope::Add<double> sum_x, sum_xx, sum_y, sum_xy;
    for(size_t i=n;i>0;--i)
    {
        sum_x  << x[i];
        sum_xx << Squared(x[i]);
        sum_y  << y[i];
        sum_xy << x[i] * y[i];
    }

    const double S   = n;
    const double Sx  = sum_x.sum();
    const double Sxx = sum_xx.sum();
    const double Sy  = sum_y.sum();
    const double Sxy = sum_xy.sum();

    const double Delta = S * Sxx - Sx*Sx;
    const double b = (Sxx*Sy - Sx*Sxy)/Delta;
    const double a = (S*Sxy - Sx * Sy)/Delta;

    std::cerr << "\t" << a << "*x+(" << b << ")" << std::endl;

    return b;
}

#include "y/random/shuffle.hpp"

template <typename T> static inline
void TestTwoDFT(const unsigned p,
                Random::Bits  &ran)
{
    const size_t n = 1 << p;
    Vector<T>    data1(n,0);
    Vector<T>    data2(n,0);
    for(size_t i=1;i<=n;++i)
    {
        data1[i] = i;
        data2[i] = i;
    }
    Random::Shuffle::Range(data1,ran);
    Random::Shuffle::Range(data2,ran);


    Vector<T> fft1(2*n,0), fft2(2*n,0);
    DFT::Forward(fft1()-1, fft2()-1, data1()-1, data2()-1, n);
    DFT::Reverse(fft1()-1,n);
    DFT::Reverse(fft2()-1,n);

    for(size_t i=1;i<=n;++i)
    {
        Y_ASSERT( static_cast<size_t>(floor(fft1[1+(i-1)*2]/n+0.5) ) == static_cast<size_t>( data1[i] ) );
        Y_ASSERT( static_cast<size_t>(floor(fft2[1+(i-1)*2]/n+0.5) ) == static_cast<size_t>( data2[i] ) );

    }

}

template <typename T> static inline
double TestMultiply(const unsigned pmax, Random::Bits &ran)
{
    const String & id = RTTI::Name<T>();

    std::cerr <<  std::setw(16) << id << " speed:";
    MKL::Antelope::Add<T> xadd;
    for(unsigned p=0;p<=pmax;++p)
    {
        const size_t n = 1 << p;
        const size_t n2 = 2*n;
        Vector<T>   fft1(n2,0), fft2(n2,0);
        WallTime    chrono;
        uint64_t    tmx    = 0;
        size_t      cycles = 0;
        uint64_t    out      = chrono.Ticks();
        do
        {
            ++cycles;
            for(size_t i=n2;i>0;--i)
            {
                fft1[i] = ran.symm<T>();
                fft2[i] = ran.symm<T>();
            }
            const uint64_t ini = chrono.Ticks();
            DFT::Multiply(fft1()-1,fft2()-1,n);
            tmx += chrono.Ticks() - ini;
        } while( chrono(chrono.Ticks()-out) < duration);
        const long double ell   = chrono(tmx);
        const double      speed = double( (static_cast<long double>(cycles)*n) / (ell) );
        (std::cerr << p << '/').flush();
        xadd << speed;
    }
    const double res = xadd.sum() / (1+pmax);
    std::cerr << " => " << HumanReadable(res) << std::endl;
    return res;
}

Y_UTEST(dft_core)
{
    Random::ParkMiller ran;

    if( argc > 1)
    {
        duration = ASCII::Convert::ToReal<double>(argv[1],"duration");
    }

    std::cerr << "float       => " << RTTI::Name< DFT_Real<float>::Type >() << std::endl;
    std::cerr << "double      => " << RTTI::Name< DFT_Real<double>::Type >() << std::endl;
    std::cerr << "long double => " << RTTI::Name< DFT_Real<long double>::Type >() << std::endl;

#if 0
    if(false)
    {
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
    }
#endif
    
    std::cerr << std::endl << "Tables" << std::endl;
    Core::Display(std::cerr, DFT::Table<double>::PositiveSin, 64) << std::endl;
    Core::Display(std::cerr, DFT::Table<double>::NegativeSin, 64) << std::endl;
    Core::Display(std::cerr, DFT::Table<double>::CosMinusOne, 64) << std::endl;

    std::cerr << std::endl << "Sample Of Transform" << std::endl;
    if(true)
    {
        for(unsigned p=0;p<6;++p)
        {
            const size_t   n = 1<<p;
            Vector<float> data(n*2,0);
            for(size_t i=1;i<=data.size();++i)
                data[i] = i;

            DFT::Forward(data()-1,n);
            DFT::Reverse(data()-1,n);
            for(size_t i=1;i<=data.size();++i)
            {
                data[i] = floor( data[i]/n + 0.5);
                Y_ASSERT(i==static_cast<size_t>(data[i]));
            }
            std::cerr << data << std::endl;
        }

    }

    std::cerr << std::endl << "Sample Of Two Transforms" << std::endl;
    {
        for(unsigned p=0;p<=12;++p)
        {
            TestTwoDFT<float>(p, ran);
            TestTwoDFT<double>(p, ran);
            TestTwoDFT<long double>(p, ran);
        }
    }

    std::cerr << std::endl << "Multiply..." << std::endl;
    {
        TestMultiply<float>(10,ran);
        TestMultiply<double>(10,ran);
        TestMultiply<long double>(10,ran);

    }


    std::cerr << std::endl << "Scaling..." << std::endl;
    const String fn = "dft.dat";
    OutputFile::Overwrite(fn);

    const double   factor = 1;
    const unsigned pmax   = 16;
    Matrix<double> speed(3,pmax);
    Vector<double> log2p(pmax,0);

    for(unsigned p=1;p<=pmax;++p)
    {
        (std::cerr << "2^" << std::setw(2) << p << " = " << std::setw(5) << (1<<p)).flush();
        const double N     = (1<<p);
        const double bigO  = N * log(N);
        float        rms32 = 0;
        double       rms64 = 0;
        long double  rmsXX = 0;

        const double spd32 = DFT_Test<float>(p,ran,rms32);
        (std::cerr << " | <flt> " << HumanReadable( factor * spd32 ) << " (" << Formatted::Get("%8.3g",double(rms32)) << ")").flush();
        const double spd64 = DFT_Test<double>(p,ran,rms64);
        (std::cerr << " | <dbl> " << HumanReadable( factor * spd64 ) << " (" << Formatted::Get("%8.3g",double(rms64)) << ")").flush();

        const double spdXX = DFT_Test<long double>(p,ran,rmsXX);
        (std::cerr << " | <ldbl> " << HumanReadable( factor * spdXX ) << " (" << Formatted::Get("%8.3g",double(rmsXX)) << ")").flush();

        std::cerr << std::endl;

        AppendFile fp(fn);

        const double rate32 = log10(spd32/bigO);
        const double rate64 = log10(spd64/bigO);
        const double rateXX = log10(spdXX/bigO);

        fp("%u %.15g %.15g %.15g\n", p, rate32, rate64, rateXX);

        log2p[p]    = p;
        speed[1][p] = rate32;
        speed[2][p] = rate64;
        speed[3][p] = rateXX;
    }

    const double b32 = linfit(log2p,speed[1]);
    const double b64 = linfit(log2p,speed[2]);
    const double bXX = linfit(log2p,speed[3]);

    const double b = (sizeof(float)*b32+sizeof(double)*b64+sizeof(long double)*bXX)/(sizeof(float)+sizeof(double)+sizeof(long double));
    std::cerr << std::endl;
    std::cerr << "\t\tscore=" << Formatted::Get("%.15g",b) << std::endl;
    std::cerr << std::endl;



    std::cerr.flush();

    if(false)
    {
        unsigned h = 0;
        for(size_t i=0;i<8;++i)
        {
            for(size_t j=0;j<8;++j,++h)
            {
                fprintf(stderr," Y_DFT(0x%02x),",h);
            }
            fprintf(stderr," \\\n");
        }
    }

    std::cerr << "Testing Dual" << std::endl;

    for(unsigned p=1;p<=20;++p)
    {
        DFT_TestDual<float>(p,ran);
        DFT_TestDual<double>(p,ran);
    }



}
Y_UDONE()


