
//! \file

#ifndef Y_DFT_Included
#define Y_DFT_Included 1

#include "y/mkl/complex.hpp"
#include "y/calculus/exact-shift.hpp"


namespace Yttrium
{
    //! inner real for DFT
    template <typename T> struct DFT_Real        { typedef T      Type; /*!< alias */ };

    //! inner real fo float DFT
    template <>           struct DFT_Real<float> { typedef double Type; /*!< alias */ };


    //__________________________________________________________________________
    //
    //
    //
    //! Discrete Fourier Transform
    //
    //
    //__________________________________________________________________________
    struct DFT
    {

        //______________________________________________________________________
        //
        //
        //! precomputed trigonometric tables
        //
        //______________________________________________________________________
        template <typename T> struct Table
        {
            static const T PositiveSin[64]; //!<  sin(pi/(2^i))
            static const T NegativeSin[64]; //!< -sin(pi/(2^i))
            static const T CosMinusOne[64]; //!< cos(pi/(2^i))-1`
        };

        //______________________________________________________________________
        //
        //
        //! swap two consecutive blocks
        //
        //______________________________________________________________________
        template <typename T> static inline
        void Swap2(T * lhs, T *rhs) {
            { const T t(*lhs); *(lhs++) = *rhs; *(rhs++) = t; }
            { const T t(*lhs); *(lhs)   = *rhs; *rhs     = t; }
        }

        //! Format using algorithm
        template <typename T> static inline
        size_t Format(T data[], const size_t size) noexcept
        {
            const size_t n = size << 1;
            for(size_t i=1,j=1;i<n;i+=2)
            {
                if(j>i)
                    Swap2(data+i,data+j);
                size_t m=size;
                while( (m >= 2) && (j > m) )
                {
                    j -= m;
                    m >>= 1;
                }
                j += m;
            }
            return n;
        }

        //! format using precomputed table
        template <typename T> static inline
        size_t Format(T                  data[],
                      const size_t       size,
                      const void * const swps,
                      const size_t       nswp) noexcept
        {
            const uint32_t * dw = static_cast<const uint32_t *>(swps);
            for(size_t i=nswp;i>0;--i)
            {
                union { const uint32_t u; uint16_t s[2]; } _ = { *(dw++) };
                Swap2(data+_.s[0],data+_.s[1]);
            }
            return size<<1;
        }

        //______________________________________________________________________
        //
        //
        //! Transform Using a Sine Table
        /**
         - Replaces data[1..2*size] :
         - by its              Discrete Fourier transform         if SinTable = PositiveSin
         - by 'size' times its Inverse Discrete Fourier transform if SinTable = NegativeSin
         */
        //______________________________________________________________________

        template <typename T> static inline
        void Transform(T                                data[],
                       const size_t                     size,
                       const typename DFT_Real<T>::Type SinTable[]) noexcept
        {
            typedef typename DFT_Real<T>::Type long_T;
            const  size_t n = Format(data,size);
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
                        data[j]       = data[i]  - tempr;
                        data[j1]      = data[i1] - tempi;
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


        //______________________________________________________________________
        //
        //
        //! Forward Discrete Fourier Transform of data[1..2*size]
        //
        //______________________________________________________________________
        template <typename T> static inline
        void Forward(T            data[],
                     const size_t size) noexcept
        {
            Transform(data,size,Table< typename DFT_Real<T>::Type >::PositiveSin);
        }

        //______________________________________________________________________
        //
        //
        //! Forward Discrete Fourier Transform of cplx[1..size]
        //
        //______________________________________________________________________
        template <typename T> static inline
        void Forward(Complex<T>   cplx[],
                     const size_t size) noexcept
        {
            Transform( &cplx[1].re - 1,size,Table< typename DFT_Real<T>::Type >::PositiveSin);

        }

        //______________________________________________________________________
        //
        //
        //! Reverse Discrete Fourier Transform of data[1..2*size], times size
        //
        //______________________________________________________________________
        template <typename T> static inline
        void Reverse(T            data[],
                     const size_t size) noexcept
        {
            Transform(data,size,Table< typename DFT_Real<T>::Type >::NegativeSin);
        }

        //______________________________________________________________________
        //
        //
        //! Reverse Discrete Fourier Transform of cplx[1..size]
        //
        //______________________________________________________________________
        template <typename T> static inline
        void Reverse(Complex<T>   cplx[],
                     const size_t size) noexcept
        {
            Transform( &cplx[1].re - 1,size,Table< typename DFT_Real<T>::Type >::NegativeSin);
        }


        //______________________________________________________________________
        //
        //
        //! unpack for fft1[1..2n] info fft1[1..2n] and fft2[1..2n]
        //
        //______________________________________________________________________
        template <typename T> static inline
        void Unpack(T fft1[], T fft2[], const size_t n) noexcept
        {
            static const T half(0.5);

            const size_t nn1 = 1+n;
            const size_t nn2 = nn1 << 1;
            const size_t nn3 = 1+nn2;

            fft2[1]=fft1[2];
            fft1[2]=fft2[2]=0;
            for(size_t j=3;j<=nn1;j+=2)
            {
                const size_t j1  = j+1;
                const size_t j2  = nn2-j;
                const size_t j3  = nn3-j;

                const T      A   = fft1[j];
                const T      B   = fft1[j2];
                const T      rep =  half*(A+B);
                const T      rem =  half*(A-B);
                const T      C   = fft1[j1];
                const T      D   = fft1[j3];
                const T      aip = half*(C+D);
                const T      aim = half*(C-D);

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

        template <typename T> static inline
        void Unpack(Complex<T> fft1[], Complex<T> fft2[], const size_t n) noexcept
        {
            Unpack( &fft1[1].re-1, &fft2[1].re-1, n );
        }


        //______________________________________________________________________
        //
        //
        //! Two Real DFT at once
        /**
         - real data1[1..size] -> fft1[1..2*size]
         - real data2[1..size] -> fft2[1..2*size]
         */
        //______________________________________________________________________
        template <typename T> static inline
        void Forward(T fft1[], T fft2[], const T data1[], const T data2[], const size_t n)
        {
            //------------------------------------------------------------------
            //
            // pack
            //
            //------------------------------------------------------------------
            {
                T *       target = fft1;
                const T * source1 = data1;
                const T * source2 = data2;
                for(size_t j=n;j>0;--j)
                {
                    *(++target) = *(++source1);
                    *(++target) = *(++source2);
                }
            }

            //------------------------------------------------------------------
            //
            // transform
            //
            //------------------------------------------------------------------
            Forward(fft1,n);

            //------------------------------------------------------------------
            //
            // unpack
            //
            //------------------------------------------------------------------
            Unpack(fft1,fft2,n);
        }

        //______________________________________________________________________
        //
        //
        //! fft1 *= fft2
        //
        //______________________________________________________________________
        template <typename T> static inline
        void Multiply(T * fft1, const T * fft2, const size_t n) noexcept
        {
            typedef Complex<T> Cplx;

            for(size_t i=n;i>0;--i)
            {
                const Cplx z1(fft1[1],fft1[2]);
                const Cplx z2(fft2[1],fft2[2]);
                const Cplx z = Cplx::MultiAlgo(z1,z2);
                fft2     += 2;
                *(++fft1) = z.re;
                *(++fft1) = z.im;
            }
        }

    private:
        template <typename T> static inline
        void RealProcess(T                                data[],
                         const T                          c2,
                         const typename DFT_Real<T>::Type wpr,
                         const typename DFT_Real<T>::Type wpi,
                         const size_t                     n) noexcept
        {
            typedef typename DFT_Real<T>::Type long_T;
            static const T c1 = 0.5;

            long_T wr  = 1.0+wpr;
            long_T wi  = wpi;

            const size_t nq = n>>2;
            const size_t np3=n+3;
            for(size_t i=2;i<=nq;++i)
            {
                const size_t i1 = (i<<1)-1;
                const size_t i2 = i1+1;
                const size_t i3 = np3-i2;
                const size_t i4 = 1+i3;
                {
                    const T h1r =  c1*(data[i1]+data[i3]);
                    const T h1i =  c1*(data[i2]-data[i4]);
                    const T h2r = -c2*(data[i2]+data[i4]);
                    const T h2i =  c2*(data[i1]-data[i3]);
                    data[i1] = h1r+wr*h2r-wi*h2i;
                    data[i2] = h1i+wr*h2i+wi*h2r;
                    data[i3] = h1r-wr*h2r+wi*h2i;
                    data[i4] = -h1i+wr*h2i+wi*h2r;
                }
                const long_T wt = wr;
                wr=wr*wpr-wi*wpi+wr;
                wi=wi*wpr+wt*wpi+wi;
            }
        }

    public:
        template <typename T> static inline
        void RealForward(T            data[],
                         const size_t n)
        {
            assert(n>=2);
            typedef typename DFT_Real<T>::Type long_T;
            static const T c2 = -0.5;


            //------------------------------------------------------------------
            // prolog
            //------------------------------------------------------------------
            const size_t   nh = n>>1;
            const unsigned ns = ExactShift(nh);
            Forward(data,nh);

            //------------------------------------------------------------------
            // process
            //------------------------------------------------------------------
            RealProcess(data,c2,Table<long_T>::CosMinusOne[ns],Table<long_T>::PositiveSin[ns],n);

            //------------------------------------------------------------------
            // epilog
            //------------------------------------------------------------------
            {
                const T h1r = data[1];
                data[1] = h1r+data[2];
                data[2] = h1r-data[2];
            }
        }

        template <typename T> static inline
        void RealReverse(T            data[],
                         const size_t n)
        {
            assert(n>=2);
            typedef typename DFT_Real<T>::Type long_T;
            static const T c1 = 0.5;
            static const T c2 = 0.5;

            //------------------------------------------------------------------
            // prolog
            //------------------------------------------------------------------
            const size_t   nh = n>>1;
            const unsigned ns = ExactShift(nh);

            //------------------------------------------------------------------
            // process
            //------------------------------------------------------------------
            RealProcess(data,c2,Table<long_T>::CosMinusOne[ns],Table<long_T>::NegativeSin[ns],n);

            //------------------------------------------------------------------
            // epilog
            //------------------------------------------------------------------
            {
                const T h1r = data[1];
                data[1]=c1*(h1r+data[2]);
                data[2]=c1*(h1r-data[2]);
                Reverse(data,nh);
            }
        }






    };


#if !defined(_MSC_VER)
    template <> const double      DFT:: Table<double>::      PositiveSin[]; //!< decl
    template <> const double      DFT:: Table<double>::      NegativeSin[]; //!< decl
    template <> const double      DFT:: Table<double>::      CosMinusOne[]; //!< decl

    template <> const long double DFT:: Table<long double>:: PositiveSin[]; //!< decl
    template <> const long double DFT:: Table<long double>:: NegativeSin[]; //!< decl
    template <> const long double DFT:: Table<long double>:: CosMinusOne[]; //!< decl
#endif


}


#endif

