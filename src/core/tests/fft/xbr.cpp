#include "y/utest/run.hpp"
#include "y/utest/timing.hpp"
#include "y/calculus/base2.hpp"
#include "y/type/utils.hpp"
#include "y/calculus/byte-count.hpp"
#include "y/string.hpp"
#include "y/stream/libc/output.hpp"

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
                //Swap(data[j],data[i]);
                //Swap(data[j+1],data[i+1]);
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


}


Y_UTEST(fft_xbr)
{
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
        << ", uint" << std::setw(2) << bps * 8
        << ", tabSize = " << std::setw(7) << tsz
        << std::endl;
        if(nswp>0 && tsz<=65536)
        {
            //std::cerr << "|_OK" << std::endl;
            const String     headerName = FormatString("xbr%u.hpp",unsigned(size));
            Libc::OutputFile header(headerName);
            header("#infdef XBR%u_INCLUDED\n", unsigned(size));
            header("#define XBR%u_INCLUDED\n", unsigned(size));
            header << "\n";
            header << "#include \"y/config/starting.hpp\"\n";
            header << "namespace Yttrium {\n";
            header << "\tnamespace Core {\n";
            header("\t\textern const uint16_t XBR%u[%u];\n",unsigned(size),unsigned(nswp));
            header << "\t}\n";
            header << "}\n";
            header << "#endif\n";
            
        }
    }
}
Y_UDONE()
