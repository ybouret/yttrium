

//! \file

#ifndef Y_DFTN_Included
#define Y_DFTN_Included 1

#include "y/dft/dft.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Discrete N-Dimensional Fourier Transform
    //
    //
    //__________________________________________________________________________
    struct DFTN
    {

        //! Product of nn[1..ndim]
        static inline size_t Prod(const size_t   nn[],
                                  const unsigned dims) noexcept
        {
            size_t prod=1;
            for(unsigned i=dims;i>0;--i)
                prod *= nn[i];
            return prod;
        }


        //! Apply Transform
        /**
         - Replaces data by its dims-dimensional discrete Fourier transform, if table = PositiveSin
         - nn[1..ndim] is an integer array containing the lengths of each dimension (number of complex
         values), which MUST all be powers of 2.
         - data is a real array of length twice the product of
         these lengths, in which the data are stored as in a multidimensional complex array: real and
         imaginary parts of each element are in consecutive locations, in a row-major mode
         */
        template <typename T> static inline
        void Transform(T * const      data,
                       const size_t   nn[],
                       const unsigned dims,
                       const typename DFT_Real<T>::Type * const SinTable) noexcept
        {
            typedef typename DFT_Real<T>::Type long_T;
            static const long_T _0 = 0;
            static const long_T _1 = 1;

            assert(0!=SinTable);
            const size_t ntot = Prod(nn,dims);
            size_t       nprv = 1;
            for(unsigned idim=dims;idim>0;--idim)
            {
                const size_t n     = nn[idim];
                const size_t nrem  = ntot/(n*nprv);
                const size_t ip1   = nprv << 1;
                const size_t ip2   = ip1*n;
                const size_t ip3   = ip2*nrem;
                {
                    size_t i2rev = 1;
                    for(size_t i2=1;i2<=ip2;i2+=ip1)
                    {
                        if(i2<i2rev) {
                            const size_t i1max=i2+ip1-2;
                            const size_t delta=i2rev-i2;
                            T * const    data2=data+delta;
                            for (size_t i1=i2;i1<=i1max;i1+=2)
                            {
                                for (size_t i3=i1;i3<=ip3;i3+=ip2)
                                    DFT::Swap2(data+i3,data2+i3);
                            }
                        }
                        size_t ibit=ip2 >> 1;
                        while(ibit >= ip1 && i2rev > ibit) {
                            i2rev -= ibit;
                            ibit >>= 1;
                        }
                        i2rev += ibit;
                    }
                }


                size_t ifp1  = ip1;
                size_t iln2  = 0;
                while(ifp1<ip2)
                {
                    const size_t ifp2   = ifp1 << 1;
                    const size_t iln2p1 = iln2+1;
                    long_T       wtemp  = SinTable[iln2p1];
                    long_T       wpr    = DFT::Table<long_T>::CosMinusOne[iln2];
                    long_T       wpi    = SinTable[iln2];
                    long_T       wr     = _1;
                    long_T       wi     = _0;;
                    for(size_t i3=1;i3<=ifp1;i3+=ip1)
                    {
                        const size_t i1max = i3+ip1-2;
                        for (size_t i1=i3;i1<=i1max;i1+=2)
                        {
                            for(size_t i2=i1;i2<=ip3;i2+=ifp2)
                            {
                                const size_t k1re = i2;
                                const size_t k2re = k1re+ifp1;
                                const size_t k1im = k1re+1;
                                const size_t k2im = k2re+1;

                                const T tempr=(T)wr*data[k2re]-(T)wi*data[k2im];
                                const T tempi=(T)wr*data[k2im]+(T)wi*data[k2re];
                                data[k2re]  = data[k1re]-tempr;
                                data[k2im]  = data[k1im]-tempi;
                                data[k1re] += tempr;
                                data[k1im] += tempi;
                            }
                        }
                        wr=(wtemp=wr)*wpr-wi*wpi+wr;
                        wi=wi*wpr+wtemp*wpi+wi;
                    }
                    ifp1=ifp2;
                    iln2=iln2p1;
                }
                nprv *= n;
            }
        }

        //! Forward Transform
        template <typename T> static inline
        void Forward(T * const      data,
                     const size_t   nn[],
                     const unsigned dims) noexcept
        {
            typedef typename DFT_Real<T>::Type long_T;
            Transform(data,nn,dims,DFT::Table<long_T>::PositiveSin);
        }

        //! Reverse Transform
        template <typename T> static inline
        void Reverse(T * const      data,
                     const size_t   nn[],
                     const unsigned dims) noexcept
        {
            typedef typename DFT_Real<T>::Type long_T;
            Transform(data,nn,dims,DFT::Table<long_T>::NegativeSin);
        }

    };

}

#endif

