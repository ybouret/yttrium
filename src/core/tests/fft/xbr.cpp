#include "y/utest/run.hpp"
#include "y/utest/timing.hpp"
#include "y/calculus/base2.hpp"
#include "y/type/utils.hpp"
#include "y/calculus/byte-count.hpp"
#include "y/string.hpp"
#include "y/stream/libc/output.hpp"
#include "y/container/cxx/array.hpp"
#include "y/fft/fft.hpp"

#include <cstring>

using namespace Yttrium;

namespace Yttrium
{

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


    static double Duration = 0.5;

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


}


Y_UTEST(fft_xbr)
{

    if(argc>1) Duration = atof(argv[1]);
    
    for(unsigned shift=0;shift<=10;++shift)
    {
        TestXBR<float>(shift);
        TestXBR<double>(shift);
        TestXBR<long double>(shift);
    }

    return 0;

    {
        Libc::OutputFile header("xbr.hxx");

        for(unsigned shift=0;shift<=20;++shift)
        {
            const size_t   size = 1 << shift;
            uint64_t       imax = 0;
            const size_t   nswp = CountXBR(size,imax);
            const unsigned bps  = NextPowerOfTwo(ByteCount::For(imax));
            const size_t   tsz  = bps * nswp;
            std::cerr << "2^" << std::setw(2) << shift
            << " = " <<  std::setw(7) << size
            << " : " <<  std::setw(7) << nswp
            << ", imax = " << std::setw(8) << imax
            << ", uint"    << std::setw(2) << bps * 8
            << ", tabSize = " << std::setw(7) << tsz
            << std::endl;

            if(nswp>0&&nswp<=256)
            {
                header << "template <typename T> static inline\n";
                header(   "size_t XBR%u(T data[]) noexcept {\n", unsigned(size) );

                const size_t n = (size<<1);
                for(size_t j=1,i=1;i<n;i+=2)
                {
                    if(j>i)
                    {
                        //Swap(data[j],data[i]);
                        //Swap(data[j+1],data[i+1]);
                        header("\tSwap(data[0x%04x],data[0x%04x]);\n", unsigned(j), unsigned(i));
                        header("\tSwap(data[0x%04x],data[0x%04x]);\n", unsigned(j+1), unsigned(i+1));

                    }
                    size_t m=size;
                    while (m >= 2 && j > m)
                    {
                        j -= m;
                        m >>= 1;
                    }
                    j += m;
                }

                header(   "\treturn %u;\n", unsigned(n));
                header << "}\n\n";
            }

        }
    }
}
Y_UDONE()
