//! \file

#ifndef Y_FFT_Included
#define Y_FFT_Included 1

#include "y/singleton.hpp"
#include "y/type/complex.hpp"
#include "y/calculus/base2.hpp"


namespace Yttrium
{

    template <typename T>
    struct LongTypeFor
    {
        typedef T Type;
    };

    template <>
    struct LongTypeFor<float>
    {
        typedef double Type;
    };

    //__________________________________________________________________________
    //
    //
    //! Computing FFT with precomputed metrics
    //
    //__________________________________________________________________________
    class FFT : public Singleton<FFT>
    {
    public:

        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const char * const      CallSign;                                //!< "FFT"
        static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 6; //!< life time

        //______________________________________________________________________
        //
        //
        //! eXtended Bits Reversal
        //
        //______________________________________________________________________
        union XBR
        {
            uint32_t dw; //!< alias 32bits
            struct {
                uint16_t i;
                uint16_t j;
            };
        };

        static  const size_t    TableSizeXBR = 65536;                      //!< internal table bytes
        static  const size_t    AvailableXBR = TableSizeXBR/sizeof(XBR);   //!< number of total swaps
        static  const unsigned  MinShift     = iLog2<4>::Value;            //!< no swaps below 4 complexes
        static  const unsigned  MaxShift     = iLog2<AvailableXBR>::Value; //!< cumulative swaps won't exceed this value



    public:

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //______________________________________________________________________
        //
        //! counting number of exchanges for a given size
        //______________________________________________________________________
        static inline size_t CountXBR(const size_t size) noexcept
        {
            assert(IsPowerOfTwo(size));
            size_t       count = 0;
            const size_t half = size>>1;
            size_t j=0;
            for(size_t i=0;i<size;++i)
            {
                if(j>i)
                    ++count;
                size_t m=half;
                while( (m>0) && j >= m)
                {
                    j  -= m;
                    m >>= 1;
                }
                j += m;
            }
            return count;
        }


        //______________________________________________________________________
        //
        //! generic real version data[1..2*size]
        //______________________________________________________________________
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

        //______________________________________________________________________
        //
        //! generic complex version, data[0..size-1]
        //______________________________________________________________________
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

        //______________________________________________________________________
        //
        //! specifice makeXBR for complex data[0..(size=2^shift)-1]
        //______________________________________________________________________
        template <typename T>
        inline size_t makeXBR(Complex<T>     data[],
                              const size_t   size,
                              const unsigned shift) noexcept
        {
            assert( ( size_t(1) << shift) == size);
            if(shift<MinShift)
            {
                return size<<1;
            }
            else
            {
                if(shift>MaxShift)
                {
                    return MakeXBR(data,size);
                }
                else
                {
                    const uint32_t *arr = xbrp[shift]; assert(0!=arr);
                    for(size_t k=xbrn[shift];k>0;--k)
                    {
                        const XBR xbr = { arr[k] };
                        Swap(data[xbr.i],data[xbr.j]);
                    }
                    return size<<1;
                }
            }
        }

        


        //! data[1..2*nn]
        template <typename T> static inline
        void Run(T data[], const size_t nn, const int isign) noexcept
        {
            Raw(data,MakeXBR(data,nn),isign);
        }


        //! data[1..2*nn]
        template <typename T> inline
        void run(Complex<T  >   cplx[],
                 const size_t   size,
                 const unsigned shift,
                 const int      isign) noexcept
        {

            T    *       data = (&(cplx[0].re))-1;
            Raw(data,makeXBR(cplx,size,shift),isign);
        }
        

    private:
        Y_DISABLE_COPY_AND_ASSIGN(FFT);
        friend class Singleton<FFT>;
        explicit FFT();
        virtual ~FFT() noexcept;

        typedef const uint32_t *PTR32;     //!< alias
        PTR32           xbrp[MaxShift+1];  //!< swap indices positions
        size_t          xbrn[MaxShift+1];  //!< number of swaps


        template <typename T>
        static inline void Raw(T data[], const size_t n, const int isign)
        {
            typedef double LongT;
            LongT wtemp,wr,wpr,wpi,wi,theta;
            T     tempr,tempi;

            size_t mmax=2;
            while(n>mmax)
            {
                const size_t istep = (mmax << 1);
                theta=isign*(6.28318530717959/mmax);
                wtemp=sin(0.5*theta);
                wpr = -2.0*wtemp*wtemp;
                wpi = sin(theta);
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
    };

}

#endif

