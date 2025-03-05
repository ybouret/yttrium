

//! \file

#ifndef Y_DFTN_Included
#define Y_DFTN_Included 1

#include "y/dft/dft.hpp"

namespace Yttrium
{

    struct DFTN
    {
        /**
         Replaces data by its ndim-dimensional discrete Fourier transform, if isign is input as 1.
         nn[1..ndim] is an integer array containing the lengths of each dimension (number of complex
         values), which MUST all be powers of 2. data is a real array of length twice the product of
         these lengths, in which the data are stored as in a multidimensional complex array: real and
         imaginary parts of each element are in consecutive locations, and the rightmost index of the
         array increases most rapidly as one proceeds along data. For a two-dimensional array, this is
         equivalent to storing the array by rows. If isign is input as−1, data is replaced by its inverse
         */
        static inline
        void fourn(float          data[],
                   const size_t   nn[],
                   const unsigned ndim,
                   const int      isign)
        {
            size_t  k1,k2;
            float tempi,tempr;
            double theta,wi,wpi,wpr,wr,wtemp;

            size_t ntot=1;
            for(unsigned idim=1;idim<=ndim;idim++)
            {
                std::cerr << "nn[" << idim << "]=" << nn[idim] << std::endl;
                ntot *= nn[idim];
            }
            std::cerr << "ntot=" << ntot << std::endl;
            size_t nprev=1;
            for(unsigned idim=ndim;idim>0;--idim)
            {
                const size_t n     = nn[idim];
                const size_t nrem  = ntot/(n*nprev);
                const size_t ip1   = nprev << 1;
                const size_t ip2   = ip1*n;
                const size_t ip3   = ip2*nrem;
                {
                    size_t       i2rev = 1;
                    for(size_t i2=1;i2<=ip2;i2+=ip1)
                    {
                        if (i2 < i2rev)
                        {
                            for (size_t i1=i2;i1<=i2+ip1-2;i1+=2)
                            {
                                for (size_t i3=i1;i3<=ip3;i3+=ip2)
                                {
                                    const size_t i3rev=i2rev+i3-i2;
                                    assert(i3>=1);
                                    assert(i3rev>=1);
                                    assert(i3+1<=ntot*2);
                                    assert(i3rev+1<=ntot*2);

                                    Swap(data[i3],data[i3rev]);
                                    Swap(data[i3+1],data[i3rev+1]);
                                }
                            }
                        }
                        size_t ibit=ip2 >> 1;
                        while (ibit >= ip1 && i2rev > ibit) {
                            i2rev -= ibit;
                            ibit >>= 1;
                        }
                        i2rev += ibit;
                    }
                }

                size_t ifp1=ip1;
                while (ifp1 < ip2)
                {
                    const size_t ifp2  = ifp1 << 1;
                    theta = isign*6.28318530717959/(ifp2/ip1);
                    wtemp = sin(0.5*theta);
                    wpr   = -2.0*wtemp*wtemp;
                    wpi   = sin(theta);
                    wr=1.0;
                    wi=0.0;
                    for(size_t i3=1;i3<=ifp1;i3+=ip1)
                    {
                        for (size_t i1=i3;i1<=i3+ip1-2;i1+=2)
                        {
                            for(size_t i2=i1;i2<=ip3;i2+=ifp2)
                            {
                                k1=i2;        assert(k1>=1); assert(k1+1<=2*ntot);
                                k2=k1+ifp1;   assert(k2>=1); assert(k2+1<=2*ntot);
                                tempr=(float)wr*data[k2]-(float)wi*data[k2+1];
                                tempi=(float)wr*data[k2+1]+(float)wi*data[k2];
                                data[k2]=data[k1]-tempr;
                                data[k2+1]=data[k1+1]-tempi;
                                data[k1] += tempr;
                                data[k1+1] += tempi;
                            }
                        }
                        wr=(wtemp=wr)*wpr-wi*wpi+wr;
                        wi=wi*wpr+wtemp*wpi+wi;
                    }
                    ifp1=ifp2;
                }
                nprev *= n;
            }
        }
    };

}

#endif

