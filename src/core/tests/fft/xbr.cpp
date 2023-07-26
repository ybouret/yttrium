
#include "y/fft/fft.hpp"
#include "y/utest/run.hpp"
#include "y/memory/legacy.hpp"
#include "y/utest/timing.hpp"
#include "y/type/utils.hpp"
#include "y/calculus/bit-count.hpp"
#include "y/calculus/align.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/memory/buffer/of.hpp"
#include "y/text/hexadecimal.hpp"

using namespace Yttrium;


namespace
{


    

    template <typename T> static inline
    void checkXBR(const unsigned shift, FFT &fft)
    {
        const size_t nc = 1<<shift;
        const size_t nr = nc*2;
        std::cerr << "Check XBR 2^" << std::setw(2) << shift;


        Memory::BufferOf<T>            rbuf(nr);
        Memory::BufferOf< Complex<T> > cbuf(nc);
        Memory::BufferOf< Complex<T> > xbuf(nc);

        Y_ASSERT(rbuf.measure() == cbuf.measure() );
        Y_ASSERT(rbuf.measure() == xbuf.measure() );

        std::cerr << "| " << std::setw(8) << nc << " complexes";
        std::cerr << ", " << std::setw(8) << nr << " reals";
        std::cerr << ", in " << HumanReadable(rbuf.measure()) << "b";
        std::cerr.flush();

        T *r = &rbuf[0];
        T *c = &cbuf[0].re;
        T *x = &xbuf[0].re;
        for(size_t i=0;i<nr;++i) r[i] = c[i] = x[i] = static_cast<T>(i+1);

        FFT::MakeXBR(r-1,nc);
        FFT::MakeXBR(&cbuf[0],nc);
        fft.makeXBR(&xbuf[0],nc,shift);
        Y_ASSERT(rbuf.HasSameContentThan(cbuf));
        Y_ASSERT(rbuf.HasSameContentThan(xbuf));

        std::cerr << std::endl;
    }


    
    
}

Y_UTEST(fft_xbr)
{
    FFT::Verbose = 1;
    FFT &fft = FFT::Instance();
    std::cerr << fft.callSign() << std::endl;


    for(unsigned p=0;p<=20;++p)
    {
        checkXBR<float>(p,fft);
        checkXBR<double>(p,fft);
        checkXBR<long double>(p,fft);
    }




}
Y_UDONE()

