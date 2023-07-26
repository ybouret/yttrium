//! \file

#ifndef Y_FFT_Included
#define Y_FFT_Included 1

#include "y/singleton.hpp"
#include "y/type/complex.hpp"


namespace Yttrium
{

    class FFT : public Singleton<FFT>
    {
    public:
        static const char * const      CallSign;                                //!< "FFT"
        static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 6; //!< life time
        struct XBR { uint16_t i,j; };

        template <typename T>
        static inline size_t MakeXBR(T data[], const size_t size) noexcept
        {
            const size_t n = (size<<1);
            size_t j=1;
            for(size_t i=1;i<n;i+=2)
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
            return n;
        }

#if 1
        template <typename T>
        static inline size_t MakeXBR(Complex<T>   data[],
                                     const size_t size) noexcept
        {
            const size_t half  = size>>1;
            size_t j=0;
            for(size_t i=0;i<size;++i)
            {
                if(j>i)
                {
                    Swap(data[j],data[i]);
                }

                size_t m=half;
                while( (m>0) && j >= m)
                {
                    j  -= m;
                    m >>= 1;
                }
                j += m;
            }
            return size << 1;
        }
#endif

        


        template <typename T> static inline
        void Run(T data[], const size_t nn, const int isign) noexcept
        {
            typedef double LongT;
            LongT wtemp,wr,wpr,wpi,wi,theta;
            T     tempr,tempi;

            const size_t n = MakeXBR(data,nn);

            size_t mmax=2;
            while(n>mmax)
            {
                const size_t istep = (mmax << 1);
                theta=isign*(6.28318530717959/mmax);
                wtemp=sin(0.5*theta);
                wpr = -2.0*wtemp*wtemp;
                wpi=sin(theta);
                wr=1.0;
                wi=0.0;
                for(size_t m=1;m<mmax;m+=2)
                {
                    for(size_t i=m;i<=n;i+=istep)
                    {
                        const size_t j=i+mmax;
                        tempr=wr*data[j]-wi*data[j+1];
                        tempi=wr*data[j+1]+wi*data[j];
                        data[j]=data[i]-tempr;
                        data[j+1]=data[i+1]-tempi;
                        data[i]   += tempr;
                        data[i+1] += tempi;
                    }
                    wr=(wtemp=wr)*wpr-wi*wpi+wr;
                    wi=wi*wpr+wtemp*wpi+wi;
                }
                mmax=istep;
            }
            
        }

        const size_t xbrMin; //!< min size requiring bit-reversal, should be 4
        const size_t xbrMax; //!< max size with table driven bit-reversal, depends on memory

    private:
        Y_DISABLE_COPY_AND_ASSIGN(FFT);
        friend class Singleton<FFT>;
        explicit FFT();
        virtual ~FFT() noexcept;
    };

}

#endif

