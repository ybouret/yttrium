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
        static size_t CountXBR(const size_t size) noexcept;

        //______________________________________________________________________
        //
        //! generic real version data[1..2*size]
        //______________________________________________________________________
        template <typename T> static size_t MakeXBR(T data[], const size_t size) noexcept;

        //______________________________________________________________________
        //
        //! generic complex version, data[0..size-1]
        //______________________________________________________________________
        template <typename T> static   size_t MakeXBR(Complex<T> data[], const size_t size) noexcept;

        //______________________________________________________________________
        //
        //! specifice makeXBR for complex data[0..(size=2^shift)-1]
        //______________________________________________________________________
        template <typename T>
        size_t makeXBR(Complex<T>     data[],
                       const size_t   size,
                       const unsigned shift) noexcept;
        


        //! data[1..2*nn]
        template <typename T> static void Forward(T data[], const size_t size) noexcept;

        //! data[1..2*nn]
        template <typename T> static void Reverse(T data[], const size_t size) noexcept;


        //! fft1[1..2n], fft2[1..2n], data1[1..n], data1[1..n]
        template <typename T>
        static void Forward( T fft1[], T fft2[], const T data1[], const T data2[], const size_t n) noexcept;

        
        //! cplx[1..size]
        template <typename T> void forward(Complex<T> cplx[], const size_t size, const unsigned shift) noexcept;

        //! cplx[1..size]
        template <typename T> void reverse(Complex<T> cplx[], const size_t size, const unsigned shift) noexcept;


        //! fft1[1..size], fft2[1..size], data1[1..size], data2[1..size]
        template <typename T> void forward(Complex<T>     fft1[],
                                           Complex<T>     fft2[],
                                           const T        data1[],
                                           const T        data2[],
                                           const size_t   size,
                                           const unsigned shift) noexcept;



    private:
        Y_DISABLE_COPY_AND_ASSIGN(FFT);
        friend class Singleton<FFT>;
        explicit FFT();
        virtual ~FFT() noexcept;

        typedef const uint32_t *PTR32;     //!< alias
        PTR32           xbrp[MaxShift+1];  //!< swap indices positions
        size_t          xbrn[MaxShift+1];  //!< number of swaps

    };

}

#endif

