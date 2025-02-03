
//! \file

#ifndef Y_DFT_Included
#define Y_DFT_Included 1

#include "y/mkl/complex.hpp"


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


        template <typename T> static inline
        void Swap2(T * lhs, T *rhs) {
            { const T t(*lhs); *(lhs++) = *rhs; *(rhs++) = t; }
            { const T t(*lhs); *(lhs)   = *rhs; *rhs     = t; }
        }

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
                    Swap2(data+i,data+j);
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


        /**
         unpack for fft1[1..2n] info fft1[1..2n] and fft2[1..2n]
         */
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

        /**
         Given two real input arrays data1[1..n] and data2[1..n], this routine calls four1 and
         returns two complex output arrays, fft1[1..2n] and fft2[1..2n], each of complex length
         n (i.e., real length 2*n), which contain the discrete Fourier transforms of the respective data
         */
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

        /**

         */
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

