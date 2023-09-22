//! \file

#ifndef Y_FFT_Included
#define Y_FFT_Included 1

#include "y/mkl/complex.hpp"
#include "y/calculus/base2.hpp"


namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //! type for internal trigonometry
    //
    //__________________________________________________________________________
    template <typename T>
    struct LongTypeFor
    {
        typedef T Type; //!< default
    };

    //__________________________________________________________________________
    //
    //
    //! use double precision trigonometry for float FFT
    //
    //__________________________________________________________________________
    template <>
    struct LongTypeFor<float>
    {
        typedef double Type; //!< specific for float
    };

    //__________________________________________________________________________
    //
    //
    //! Computing FFT with precomputed metrics
    //
    //__________________________________________________________________________
    class FFT
    {
    public:
        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //______________________________________________________________________
        //
        //! bit reversal of data[1..size*2]
        //______________________________________________________________________
        template <typename T> static inline
        size_t BitReversal(T data[], const size_t size) noexcept
        {
            assert(IsPowerOfTwo(size));
            assert(0!=data);
            const size_t n = (size<<1);
            for(size_t j=1,i=1;i<n;i+=2)
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


        //______________________________________________________________________
        //
        //! Forward FFT transform of data[1..2*size]
        //______________________________________________________________________
        template <typename T>
        static inline void Forward(T            data[],
                                   const size_t size) noexcept
        {
            static  const  typename LongTypeFor<T>::Type myPI(3.141592653589793238462643383279502884197);
            Transform(data,size,myPI);
        }

        //______________________________________________________________________
        //
        //! Forward FFT transform of data[1..2*size]
        //______________________________________________________________________
        template <typename T>
        static inline void Reverse(T            data[],
                                   const size_t size) noexcept
        {
            static  const  typename LongTypeFor<T>::Type myPI(-3.141592653589793238462643383279502884197);
            Transform(data,size,myPI);
        }

        template <typename T>
        static inline void Real(T data[], const size_t n, const int isign)
        {
            typedef typename LongTypeFor<T>::Type REAL;
            assert(data);
            assert(IsPowerOfTwo(n));
            unsigned long i,i1,i2,i3,i4,np3;
            T c1=0.5,c2,h1r,h1i,h2r,h2i;
            REAL wr,wi,wpr,wpi,wtemp;

            REAL theta=3.141592653589793/(REAL) (n>>1);
            if (isign == 1) {
                c2 = -0.5;
                Forward(data,n>>1);
            }
            else
            {
                c2    = 0.5;
                theta = -theta;
            }
            wtemp = sin(0.5*theta);
            wpr   = -2.0*wtemp*wtemp;
            wpi   = sin(theta);
            wr    = 1.0+wpr;
            wi    = wpi;
            np3   = n+3;
            for (i=2;i<=(n>>2);i++)
            {
                i4=1+(i3=np3-(i2=1+(i1=i+i-1)));
                h1r=c1*(data[i1]+data[i3]);
                h1i=c1*(data[i2]-data[i4]);
                h2r = -c2*(data[i2]+data[i4]);
                h2i=c2*(data[i1]-data[i3]);
                data[i1]=h1r+wr*h2r-wi*h2i;
                data[i2]=h1i+wr*h2i+wi*h2r;
                data[i3]=h1r-wr*h2r+wi*h2i;
                data[i4] = -h1i+wr*h2i+wi*h2r;
                wr=(wtemp=wr)*wpr-wi*wpi+wr;
                wi=wi*wpr+wtemp*wpi+wi;
            }

            if (isign == 1) {
                data[1] = (h1r=data[1])+data[2];
                data[2] = h1r-data[2];

            }
            else
            {
                data[1]=c1*((h1r=data[1])+data[2]);
                data[2]=c1*(h1r-data[2]);
                Reverse(data,n>>1);
            }
        }

        //______________________________________________________________________
        //
        //! computed fft1[1..2n] and fft2[1..2n] data[1..n] and data2[1..n]
        //______________________________________________________________________
        template <typename T>
        static inline void Forward(T fft1[], T fft2[], const T data1[], const T data2[], const size_t size) noexcept
        {
            assert(0!=fft1);
            assert(0!=fft2);
            assert(0!=data1);
            assert(0!=data2);

            for(size_t j=1,jj=2;j<=size;j++,jj+=2)
            {
                fft1[jj-1] = data1[j];
                fft1[jj]   = data2[j];
            }
            Core::Display(std::cerr << "pack = ", fft1+1, 2*size) << std::endl;
            FFT::Forward(fft1,size);
            Core::Display(std::cerr << "fft1  = ", fft1+1, 2*size) << std::endl;
            Core::Display(std::cerr << "fft2  = ", fft2+1, 2*size) << std::endl;
            Expand(fft1,fft2,size);
            Core::Display(std::cerr << "fft1  = ", fft1+1, 2*size) << std::endl;
            Core::Display(std::cerr << "fft2  = ", fft2+1, 2*size) << std::endl;
        }

        //______________________________________________________________________
        //
        //! expand fft1[1..2n] and fft2[1..2n] from a dual--functions transform
        //______________________________________________________________________
        template <typename T> static
        inline void Expand(T fft1[], T fft2[], const size_t n) noexcept
        {
            static const T half(0.5);
            const size_t np1 = n+1;
            const size_t nn2 = np1 << 1;
            const size_t nn3 = 1+nn2;
            fft2[1]=fft1[2];
            fft1[2]=fft2[2]=0;
            for(size_t j=np1;j>=3;j-=2)
            {
                const size_t j1 = j+1;
                const size_t j2 = nn2-j;
                const size_t j3 = nn3-j;
                const T      A  = fft1[j];
                const T      B  = fft1[j2];
                const T      C  = fft1[j1];
                const T      D  = fft1[j3];
                const T      rep=half*(A+B);
                const T      rem=half*(A-B);
                const T      aip=half*(C+D);
                const T      aim=half*(C-D);

                fft1[j]  =  rep;
                fft1[j1] =  aim;
                fft1[j2] =  rep;
                fft1[j3] = -aim;

                fft2[j]  =  aip;
                fft2[j1] = -rem;
                fft2[j2] =  aip;
                fft2[j3] =  rem;
            }
        }

    private:
        //______________________________________________________________________
        //
        //! Transform data[1..n=size*2]
        //______________________________________________________________________
        template <typename T>
        static inline void Transform(T *                           data,
                                     const size_t                  size,
                                     typename LongTypeFor<T>::Type myPI) noexcept
        {
            assert(0!=data);
            assert(IsPowerOfTwo(size));
            typedef typename LongTypeFor<T>::Type REAL;
            static  const    REAL half(0.5);
            static  const    REAL two(2);

            const size_t n = BitReversal(data,size); assert(n==size*2);
            size_t mmax=2;
            while(n>mmax)
            {
                const size_t istep = (mmax << 1);
                //const REAL   theta = (myPI/istep);
                const REAL   theta = (2*myPI/mmax);
                REAL         wtemp = std::sin(half*theta);
                REAL         wpr   = -two*wtemp*wtemp;
                REAL         wpi   = std::sin(theta);
                REAL         wr    = 1.0;
                REAL         wi    = 0.0;
                for(size_t m=1;m<mmax;m+=2)
                {
                    for(size_t i=m;i<=n;i+=istep)
                    {
                        const size_t j     = i+mmax;
                        const T      tempr = wr*data[j]  -wi*data[j+1];
                        const T      tempi = wr*data[j+1]+wi*data[j];
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

#endif

