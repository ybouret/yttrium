
#include "y/fft/fft.hpp"
#include "y/utest/run.hpp"
#include "y/memory/legacy.hpp"
#include "y/utest/timing.hpp"
#include "y/type/utils.hpp"
#include "y/calculus/bit-count.hpp"
#include "y/calculus/align.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/type/complex.hpp"
#include "y/memory/buffer/of.hpp"

using namespace Yttrium;


namespace
{


    template <typename T>
    static inline size_t scalarXBR(T data[], const size_t nn) noexcept
    {
        const size_t n = (nn<<1);
        size_t j=1;
        for(size_t i=1;i<n;i+=2)
        {
            if(j>i)
            {
                Swap(data[j],data[i]);
                Swap(data[j+1],data[i+1]);
            }
            size_t m=nn;
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
    static inline size_t myXBR(Complex<T>   data[],
                               const size_t size,
                               size_t      &jmax) noexcept
    {
        size_t count = 0;
        jmax = 0;

        const size_t half = size>>1;
        size_t j=0;
        for(size_t i=0;i<size;++i)
        {
            if(j>i)
            {
                Swap(data[j],data[i]);
                jmax = Max(jmax,j);
                ++count;
            }

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



    template <typename T> static inline
    void checkXBR(const size_t shift)
    {
        const size_t nc = 1<<shift;
        const size_t nr = nc*2;
        std::cerr << "Check XBR 2^" << std::setw(2) << shift;


        Memory::BufferOf<T>            rbuf(nr);
        Memory::BufferOf< Complex<T> > cbuf(nc);
        Y_ASSERT(rbuf.measure() == cbuf.measure() );

        std::cerr << " = " << std::setw(8) << nc << " complexes";
        std::cerr << " = " << std::setw(8) << nr << " reals";
        std::cerr << " " << HumanReadable(rbuf.measure());
        std::cerr.flush();

        T *r = &rbuf[0];
        T *c = &cbuf[0].re;
        for(size_t i=0;i<nr;++i) r[i] = c[i] = static_cast<T>(i+1);

        size_t jmax = 0;
        scalarXBR(r-1,nc);
        const size_t nswp = myXBR(&cbuf[0],nc,jmax);
        Y_ASSERT(rbuf.HasSameContentThan(cbuf));

        if(sizeof(T)==sizeof(float))
        {
            const size_t tableSize = sizeof(uint16_t) * nswp;
            std::cerr << " | #swap=" << std::setw(8) << nswp << ", jmax=" << std::setw(8) << jmax;
            if(tableSize<=65536) std::cerr << " tableSize = " << HumanReadable(tableSize);
        }
        std::cerr << std::endl;
    }









    
}

Y_UTEST(fft_xbr)
{

    for(size_t p=0;p<=23;++p)
    {
        checkXBR<float>(p);
        checkXBR<double>(p);
        checkXBR<long double>(p);

    }




}
Y_UDONE()

