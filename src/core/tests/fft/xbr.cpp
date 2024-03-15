#include "y/utest/run.hpp"
#include "y/utest/timing.hpp"
#include "y/calculus/base2.hpp"
#include "y/type/utils.hpp"
#include "y/calculus/byte-count.hpp"
#include "y/string.hpp"
#include "y/stream/libc/output.hpp"
#include "y/container/cxx/array.hpp"
#include "y/fft/fft.hpp"
#include "y/random/fill.hpp"
#include "y/system/rtti.hpp"

#include <cstring>

using namespace Yttrium;

namespace Yttrium
{
    static double Duration = 0.05;

    //! bit reversal data[1..2*size]
    template <typename T>
    static inline
    size_t BR(T data[], const size_t size)
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



    //! bit reversal data[1..2*size]
    template <typename T>
    static inline
    size_t BR2(T data[], const size_t size)
    {
        assert(IsPowerOfTwo(size));
        assert(0!=data);
        const size_t n = (size<<1);
        for(size_t j=1,i=1;i<n;i+=2)
        {
            if(j>i)
            {
                const T temp[2] = { data[i], data[i+1] }; assert(2*sizeof(T)==sizeof(temp));
                memcpy(&data[i],&data[j],2*sizeof(T));
                memcpy(&data[j],&temp[0],2*sizeof(T));
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


    template <typename T>
    static inline
    void TestXBR(const size_t size, Random::Bits &ran)
    {
        const String &id = RTTI::Name<T>();
        CxxArray<T>   arr(size*2);
        CxxArray<T>   brr(size*2);
        const size_t  bytes = arr.size() * sizeof(T);
        WallTime chrono;
        uint64_t tmxA   = 0;
        uint64_t tmxB   = 0;
        size_t   cycles = 0;
        (std::cerr << " (*) " << std::setw(16) << id << ":").flush();
        do {
            ++cycles;
            Random::Fill::Block(&arr[1],bytes,ran,1,255);
            memcpy( &brr[1], &arr[1], bytes);
            Y_ASSERT( 0 == memcmp( &arr[1], &brr[1], bytes) );

            {
                const uint64_t mark = WallTime::Ticks();
                BR(arr.legacy(),size);
                tmxA += WallTime::Ticks()-mark;
            }

            {
                const uint64_t mark = WallTime::Ticks();
                BR2(brr.legacy(),size);
                tmxB += WallTime::Ticks()-mark;
            }
            Y_ASSERT( 0 == memcmp( &arr[1], &brr[1], bytes) );


        }
        while( chrono(tmxA) < Duration );

        const double rateA = cycles / chrono(tmxA);
        const double rateB = cycles / chrono(tmxB);

        std::cerr << "\trate = " << HumanReadable( uint64_t(rateA) ) << "Ops / " << HumanReadable(rateB) << "Ops";

        std::cerr << std::endl;
    }

    static inline
    size_t CountXBR(const size_t size, uint64_t &imax)
    {
        assert(IsPowerOfTwo(size));

        size_t       count = 0;
        const size_t n     = (size<<1);
        for(size_t j=1,i=1;i<n;i+=2)
        {
            if(j>i)
            {
                count +=2;
                imax = Max(imax,uint64_t(j+1));
            }
            size_t m=size;
            while (m >= 2 && j > m)
            {
                j -= m;
                m >>= 1;
            }
            j += m;
        }
        return count;
    }




#if 0
    template <typename T>
    static inline void TestXBR(const unsigned shift)
    {
        const size_t size = 1 << shift;

        CxxArray<T> source(size*2);
        for(size_t i=1;i<=size;++i) source[i] = T(i);

        {
            CxxArray<T> normal(source); Y_ASSERT(0==memcmp(&source[1],&normal[1],size*sizeof(T)));
            CxxArray<T> optimz(source); Y_ASSERT(0==memcmp(&source[1],&optimz[1],size*sizeof(T)));
            Y_ASSERT(FFT::BitReversal(  &normal[1]-1, size) == FFT::XBitReversal( &optimz[1]-1, size));
            Y_ASSERT(0==memcmp(&optimz[1],&normal[1],size*sizeof(T)));
        }

        T *data= &source[1] - 1;
        Timing tmx;
        (std::cerr << std::setw(8) << size << " | Normal: ").flush();
        Y_Timing(tmx, FFT::BitReversal(data,size), Duration);
        const uint64_t rate = tmx.speed();
        (std::cerr << HumanReadable(rate) << "Op/s").flush();
        (std::cerr << std::setw(8) << size << " | Optimz: ").flush();
        Y_Timing(tmx, FFT::XBitReversal(data,size), Duration);
        const uint64_t xrate = tmx.speed();
        (std::cerr << HumanReadable(xrate) << "Op/s").flush();
        const double speedUp = double(xrate)/double(rate);
        std::cerr << " | speedUp = " << speedUp;

        std::cerr << std::endl;


    }
#endif


}


Y_UTEST(fft_xbr)
{

    if(argc>1) Duration = atof(argv[1]);

    Random::Rand ran;
    for(unsigned shift=2;shift<=10;++shift)
    {
        const size_t size = 1 << shift;
        std::cerr << "size=" << size << std::endl;
        TestXBR<float>(size,ran);
        TestXBR<double>(size,ran);
        TestXBR<long double>(size,ran);
    }
}
Y_UDONE()
