#include "y/mkl/numeric.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/system/rtti.hpp"
#include "y/type/utils.hpp"

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


        template <typename T> struct Table
        {
            static const T PositiveSin[64]; //!<  sin(pi/(2^i))
            static const T NegativeSin[64]; //!< -sin(pi/(2^i))
            static const T CosMinusOne[64]; //!< cos(pi/(2^i))-1`
        };


        /**
         - Replaces data[1..2*size] :
         - by its Discrete Fourier transform if SinTable = PositiveSin
         - by size times its Inverse Discrete Fourier transform if SinTable = NegativeSin
         - data is a complex array of length size or, equivalently, a real array of length 2*nn
         - size MUST be an integer power of 2
         */
        template <typename T> static inline
        void Transform(T            data[],
                       const size_t size,
                       const typename DFT_Real<T>::Type SinTable[]) noexcept
        {
            typedef typename DFT_Real<T>::Type long_T;
            const size_t n = size << 1;
            for(size_t i=1,j=1;i<n;i+=2)
            {
                if(j>i)
                {
                    Swap(data[j],data[i]);
                    Swap(data[j+1],data[i+1]);
                }
                size_t m=size;
                while( (m >= 2) && (j > m) )
                {
                    j -= m;
                    m >>= 1;
                }
                j += m;
            }

            size_t mmax=2;
            size_t indx=0; // mmax/2 = 2^indx
            while(n>mmax)
            {
                const size_t istep = (mmax << 1);
                long_T       wpr   = Table<long_T>::CosMinusOne[indx];
                long_T       wpi   = SinTable[indx];
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
                    const long_T wt=wr;
                    wr=wt*wpr-wi*wpi+wt;
                    wi=wi*wpr+wt*wpi+wi;
                }
                mmax=istep;
                ++indx;
            }
        }

        template <typename T> static inline
        void Forward(T            data[],
                     const size_t size) noexcept
        {
            Transform(data,size,Table< typename DFT_Real<T>::Type >::PositiveSin);
        }

        template <typename T> static inline
        void Reverse(T            data[],
                     const size_t size) noexcept
        {
            Transform(data,size,Table< typename DFT_Real<T>::Type >::NegativeSin);
        }
    };


#define Y_DFT_REP(FIRST) \
FIRST,       Y_DFT(0x01), Y_DFT(0x02), Y_DFT(0x03), Y_DFT(0x04), Y_DFT(0x05), Y_DFT(0x06), Y_DFT(0x07), \
Y_DFT(0x08), Y_DFT(0x09), Y_DFT(0x0a), Y_DFT(0x0b), Y_DFT(0x0c), Y_DFT(0x0d), Y_DFT(0x0e), Y_DFT(0x0f), \
Y_DFT(0x10), Y_DFT(0x11), Y_DFT(0x12), Y_DFT(0x13), Y_DFT(0x14), Y_DFT(0x15), Y_DFT(0x16), Y_DFT(0x17), \
Y_DFT(0x18), Y_DFT(0x19), Y_DFT(0x1a), Y_DFT(0x1b), Y_DFT(0x1c), Y_DFT(0x1d), Y_DFT(0x1e), Y_DFT(0x1f), \
Y_DFT(0x20), Y_DFT(0x21), Y_DFT(0x22), Y_DFT(0x23), Y_DFT(0x24), Y_DFT(0x25), Y_DFT(0x26), Y_DFT(0x27), \
Y_DFT(0x28), Y_DFT(0x29), Y_DFT(0x2a), Y_DFT(0x2b), Y_DFT(0x2c), Y_DFT(0x2d), Y_DFT(0x2e), Y_DFT(0x2f), \
Y_DFT(0x30), Y_DFT(0x31), Y_DFT(0x32), Y_DFT(0x33), Y_DFT(0x34), Y_DFT(0x35), Y_DFT(0x36), Y_DFT(0x37), \
Y_DFT(0x38), Y_DFT(0x39), Y_DFT(0x3a), Y_DFT(0x3b), Y_DFT(0x3c), Y_DFT(0x3d), Y_DFT(0x3e), Y_DFT(0x3f)



#define Y_DFT(i) sin( MKL::Numeric<double>::PI / static_cast<double>( uint64_t(1) << i ) )

    template <>
    const double DFT::Table<double>::PositiveSin[64] =
    {
        Y_DFT_REP(0.0)
    };

#undef Y_DFT

#define Y_DFT(i) -sin( MKL::Numeric<double>::PI / static_cast<double>( uint64_t(1) << i ) )

    template <>
    const double DFT::Table<double>::NegativeSin[64] =
    {
        Y_DFT_REP(0.0)
    };


#undef Y_DFT

#define Y_DFT(i) cos( MKL::Numeric<double>::PI / static_cast<double>( uint64_t(1) << i ) ) - 1.0

    template <>
    const double DFT::Table<double>::CosMinusOne[64] =
    {
        Y_DFT_REP(-2.0)
    };


#undef Y_DFT

#define Y_DFT(i) sinl( MKL::Numeric<long double>::PI / static_cast<long double>( uint64_t(1) << i ) )

    template <>
    const long double DFT::Table<long double>::PositiveSin[64] =
    {
        Y_DFT_REP(0.0L)
    };

#undef Y_DFT

#define Y_DFT(i) -sinl( MKL::Numeric<long double>::PI / static_cast<long double>( uint64_t(1) << i ) )

    template <>
    const long double DFT::Table<long double>::NegativeSin[64] =
    {
        Y_DFT_REP(0.0L)
    };

#undef Y_DFT

#define Y_DFT(i) cosl( MKL::Numeric<long double>::PI / static_cast<long double>( uint64_t(1) << i ) ) - 1.0L

    template <>
    const long double DFT::Table<long double>::CosMinusOne[64] =
    {
        Y_DFT_REP(-2.0L)
    };


}

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
            ellapsed = chrono(tmx);
        }
        while( ellapsed < duration );



        return static_cast<double>( static_cast<long double>(cycles) / ellapsed );
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

    Core::Display(std::cerr, DFT::Table<double>::PositiveSin, 64) << std::endl;
    Core::Display(std::cerr, DFT::Table<double>::NegativeSin, 64) << std::endl;
    Core::Display(std::cerr, DFT::Table<double>::CosMinusOne, 64) << std::endl;


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
            }
            std::cerr << data << std::endl;
        }
    }



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

}
Y_UDONE()


