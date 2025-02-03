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
            size_t istep;
            long_T wtemp,wr,wpr,wpi,wi,theta;
            T tempr,tempi;

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

            size_t mmax=2;
            while(n>mmax)
            {
                istep = mmax << 1;
                theta = isign*(6.28318530717959/mmax);
                wtemp = sin(0.5*theta);
                wpr   = -2.0*wtemp*wtemp;
                wpi   = sin(theta);
                wr    = 1.0;
                wi    = 0.0;
                for(size_t m=1;m<mmax;m+=2)
                {
                    for(size_t i=m;i<=n;i+=istep)
                    {
                        const size_t j=i+mmax;
                        tempr=wr*data[j]-wi*data[j+1];
                        tempi=wr*data[j+1]+wi*data[j];
                        data[j]    = data[i]-tempr;
                        data[j+1]  = data[i+1]-tempi;
                        data[i]   += tempr;
                        data[i+1] += tempi;
                    }
                    wr=(wtemp=wr)*wpr-wi*wpi+wr;
                    wi=wi*wpr+wtemp*wpi+wi;
                }
                mmax=istep;
            }
        }
    };

}

Y_UTEST(dft_core)
{

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
        //std::cerr << data << std::endl;
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

}
Y_UDONE()


