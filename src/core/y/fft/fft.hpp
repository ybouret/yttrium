//! \file

#ifndef Y_FFT_Included
#define Y_FFT_Included 1

#include "y/fft/xbitrev.hpp"
#include "y/calculus/xlog2.hpp"


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
    //! Computing FFT
    //
    //__________________________________________________________________________
    class FFT
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________

        //! table of pre-computed data
        template <typename T>
        struct Table
        {
            static const T PI;              //!< 3.14...
            static const T PositiveSin[64]; //!< PositiveSin[i] =  sin(PI/2^i)
            static const T NegativeSin[64]; //!< NegativeSin[i] = -sin(PI/2^i)
            static const T Minus2SinSq[64]; //!< Minus2SinSq[i] = -2*sin(PI/2^i)^2
        };

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

#if 0
        //______________________________________________________________________
        //
        //! bit reversal of BOTH data1[1..size*2] AND data2[1..size*2]
        //______________________________________________________________________
        template <typename T> static inline
        size_t BitReversal(T data1[], T data2[], const size_t size) noexcept
        {
            assert(IsPowerOfTwo(size));
            assert(0!=data1);
            assert(0!=data2);
            const size_t n = (size<<1);
            for(size_t j=1,i=1;i<n;i+=2)
            {
                if(j>i)
                {
                    const size_t j1 = j+1;
                    const size_t i1 = i+1;
                    Swap(data1[j],  data1[i]);
                    Swap(data1[j1], data1[i1]);
                    Swap(data2[j],  data2[i]);
                    Swap(data2[j1], data2[i1]);
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
#endif



        //______________________________________________________________________
        //
        //! Forward FFT transform of data[1..2*size]
        //______________________________________________________________________
        template <typename T>
        static inline void Forward(T            data[],
                                   const size_t size) noexcept
        {
            Transform(data,size, Table< typename LongTypeFor<T>::Type >::PositiveSin );
        }

        //______________________________________________________________________
        //
        //! Forward FFT transform of data[1..2*size]
        //______________________________________________________________________
        template <typename T>
        static inline void Reverse(T            data[],
                                   const size_t size) noexcept
        {
            Transform(data,size, Table< typename LongTypeFor<T>::Type >::NegativeSin );
        }


        //______________________________________________________________________
        //
        //! Forward FFT transform of data1[1..2*size] and data2[1..2*size]
        //______________________________________________________________________
        template <typename T>
        static inline void Forward(T            data1[],
                                   T            data2[],
                                   const size_t size) noexcept
        {
            Transform(data1, data2, size, Table< typename LongTypeFor<T>::Type >::PositiveSin );
        }


        //______________________________________________________________________
        //
        //! Transform of real data, forward
        //______________________________________________________________________
        template <typename T>
        static inline void ForwardReal(T data[], const size_t n)
        {
            typedef typename LongTypeFor<T>::Type REAL;

            assert(0!=data);
            assert(IsPowerOfTwo(n));

            static const T c1= 0.5;
            static const T c2=-0.5;

            const size_t nc = n>>1;
            size_t       sh = ExactLog2(nc);
            Forward(data,nc);
            const REAL wpi = Table<REAL>::PositiveSin[sh++];
            const REAL wpr = Table<REAL>::Minus2SinSq[sh];
            RealCore<T,REAL>(data, n, wpr, wpi, c1, c2);

            {
                const T temp = data[1];
                data[1] = temp+data[2];
                data[2] = temp-data[2];
            }
        }

        //______________________________________________________________________
        //
        //! Transform of real data1 and data2, forward
        //______________________________________________________________________
        template <typename T>
        static inline void ForwardReal(T data1[], T data2[], const size_t n)
        {
            typedef typename LongTypeFor<T>::Type REAL;

            assert(0!=data1); assert(0!=data2);
            assert(IsPowerOfTwo(n));

            static const T c1= 0.5;
            static const T c2=-0.5;

            const size_t nc = n>>1;
            size_t       sh = ExactLog2(nc);
            Forward(data1,data2,nc);
            const REAL wpi = Table<REAL>::PositiveSin[sh++];
            const REAL wpr = Table<REAL>::Minus2SinSq[sh];
            RealCore<T,REAL>(data1, data2, n, wpr, wpi, c1, c2);

            {
                const T temp = data1[1];
                data1[1] = temp+data1[2];
                data1[2] = temp-data1[2];
            }

            {
                const T temp = data2[1];
                data2[1] = temp+data2[2];
                data2[2] = temp-data2[2];
            }
        }



        //______________________________________________________________________
        //
        //! Transform of real data, reverse
        //______________________________________________________________________
        template <typename T>
        static inline void ReverseReal(T data[], const size_t n)
        {
            typedef typename LongTypeFor<T>::Type REAL;

            assert(data);
            assert(IsPowerOfTwo(n));

            static const T c1=0.5;
            static const T c2=0.5;

            const size_t nc  = n>>1;
            size_t       sh  = ExactLog2(nc);
            const REAL   wpi = Table<REAL>::NegativeSin[sh++];
            const REAL   wpr = Table<REAL>::Minus2SinSq[sh];

            RealCore<T,REAL>(data, n, wpr, wpi, c1, c2);

            const T temp = data[1];
            data[1]=c1*(temp+data[2]);
            data[2]=c1*(temp-data[2]);
            Reverse(data,nc);

        }



    private:
        //! internal computation for RealCore
#define Y_FFT_REAL_CORE(data)          \
do {                                   \
const T h1r =  c1*(data[i1]+data[i3]); \
const T h1i =  c1*(data[i2]-data[i4]); \
const T h2r = -c2*(data[i2]+data[i4]); \
const T h2i =  c2*(data[i1]-data[i3]); \
data[i1] =  h1r+wr*h2r-wi*h2i;         \
data[i2] =  h1i+wr*h2i+wi*h2r;         \
data[i3] =  h1r-wr*h2r+wi*h2i;         \
data[i4] = -h1i+wr*h2i+wi*h2r;         \
} while(false)

        template <typename T, typename REAL>
        static inline void RealCore(T            data[],
                                    const size_t n,
                                    const REAL   wpr,
                                    const REAL   wpi,
                                    const T      c1,
                                    const T      c2)
        {
            static const REAL  one(1);
            REAL wr          = one+wpr;
            REAL wi          = wpi;
            const size_t np3 = n+3;
            const size_t ns2 = n>>2;
            for(size_t i=2;i<=ns2;i++)
            {
                const size_t i1=(i<<1)-1;
                const size_t i2=1+i1;
                const size_t i3=np3-i2;
                const size_t i4=1+i3;

                Y_FFT_REAL_CORE(data);

                const REAL wt = wr;
                wr=wt*wpr-wi*wpi+wr;
                wi=wi*wpr+wt*wpi+wi;
            }
        }

        template <typename T, typename REAL>
        static inline void RealCore(T            data1[],
                                    T            data2[],
                                    const size_t n,
                                    const REAL   wpr,
                                    const REAL   wpi,
                                    const T      c1,
                                    const T      c2)
        {
            static const REAL  one(1);
            REAL wr          = one+wpr;
            REAL wi          = wpi;
            const size_t np3 = n+3;
            const size_t ns2 = n>>2;
            for(size_t i=2;i<=ns2;i++)
            {
                const size_t i1=(i<<1)-1;
                const size_t i2=1+i1;
                const size_t i3=np3-i2;
                const size_t i4=1+i3;

                Y_FFT_REAL_CORE(data1);
                Y_FFT_REAL_CORE(data2);

                const REAL wt = wr;
                wr=wt*wpr-wi*wpi+wr;
                wi=wi*wpr+wt*wpi+wi;
            }
        }


    public:
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
            FFT::Forward(fft1,size);
            Expand(fft1,fft2,size);
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
        static inline void Transform(T *                                  data,
                                     const size_t                         size,
                                     const typename LongTypeFor<T>::Type  sine[]) noexcept
        {
            assert(0!=data); assert(IsPowerOfTwo(size));
            typedef typename LongTypeFor<T>::Type REAL;

            const size_t n    = XBitRev::Run(data,size);
            const REAL  *saux = Table<REAL>::Minus2SinSq;
            size_t mmax=2;
            while(n>mmax)
            {
                const size_t istep = (mmax << 1);
                REAL         wpi   = *sine;
                REAL         wtemp = *(++sine);
                REAL         wpr   = *(++saux);
                REAL         wr    = REAL(1.0);
                REAL         wi    = REAL(0.0);
                for(size_t m=1;m<mmax;m+=2)
                {
                    for(size_t i=m;i<=n;i+=istep)
                    {
                        const size_t i1    = i+1;
                        const size_t j     = i+mmax;
                        const size_t j1    = j+1;
                        const T      tempr = wr*data[j] -wi*data[j1];
                        const T      tempi = wr*data[j1]+wi*data[j];
                        data[j]   = data[i] -tempr;
                        data[j1]  = data[i1]-tempi;
                        data[i]  += tempr;
                        data[i1] += tempi;
                    }
                    wr=(wtemp=wr)*wpr-wi*wpi+wr;
                    wi=wi*wpr+wtemp*wpi+wi;
                }
                mmax=istep;
            }
        }

        //______________________________________________________________________
        //
        //! Transform data1[1..n=size*2] and data2[1..n=size*2] simultaneously
        //______________________________________________________________________
        template <typename T>
        static inline void Transform(T *                                  data1,
                                     T *                                  data2,
                                     const size_t                         size,
                                     const typename LongTypeFor<T>::Type  sine[]) noexcept
        {
            assert(0!=data1); assert(0!=data2); assert(IsPowerOfTwo(size));
            typedef typename LongTypeFor<T>::Type REAL;

            const size_t n    = XBitRev::Run_(data1,data2,size);
            const REAL  *saux = Table<REAL>::Minus2SinSq;
            size_t mmax=2;
            while(n>mmax)
            {
                const size_t istep = (mmax << 1);
                REAL         wpi   = *sine;
                REAL         wtemp = *(++sine);
                REAL         wpr   = *(++saux);
                REAL         wr    = REAL(1.0);
                REAL         wi    = REAL(0.0);
                for(size_t m=1;m<mmax;m+=2)
                {
                    for(size_t i=m;i<=n;i+=istep)
                    {
                        const size_t i1    = i+1;
                        const size_t j     = i+mmax;
                        const size_t j1    = j+1;
                        {
                            const T      tempr = wr*data1[j] -wi*data1[j1];
                            const T      tempi = wr*data1[j1]+wi*data1[j];
                            data1[j]   = data1[i] -tempr;
                            data1[j1]  = data1[i1]-tempi;
                            data1[i]  += tempr;
                            data1[i1] += tempi;
                        }
                        {
                            const T      tempr = wr*data2[j] -wi*data2[j1];
                            const T      tempi = wr*data2[j1]+wi*data2[j];
                            data2[j]   = data2[i] -tempr;
                            data2[j1]  = data2[i1]-tempi;
                            data2[i]  += tempr;
                            data2[i1] += tempi;
                        }
                    }
                    wr=(wtemp=wr)*wpr-wi*wpi+wr;
                    wi=wi*wpr+wtemp*wpi+wi;
                }
                mmax=istep;
            }
        }
        
    };

#if !defined(_MSC_VER)
    template <> const double      FFT:: Table<double>::      PositiveSin[]; //!< decl
    template <> const double      FFT:: Table<double>::      NegativeSin[]; //!< decl
    template <> const double      FFT:: Table<double>::      Minus2SinSq[]; //!< decl

    template <> const long double FFT:: Table<long double>:: PositiveSin[]; //!< decl
    template <> const long double FFT:: Table<long double>:: NegativeSin[]; //!< decl
    template <> const long double FFT:: Table<long double>:: Minus2SinSq[]; //!< decl
#endif


}

#endif

