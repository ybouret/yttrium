#include "y/utest/run.hpp"
#include "y/utest/timing.hpp"
#include "y/calculus/base2.hpp"
#include "y/type/utils.hpp"
#include "y/calculus/byte-count.hpp"
#include "y/string.hpp"
#include "y/stream/libc/output.hpp"
#include "y/container/cxx/array.hpp"
#include "y/fft/fft.hpp"
#include "y/system/rtti.hpp"
#include "../main.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/mkl/v2d.hpp"
#include "y/sequence/vector.hpp"

#include <cstring>

using namespace Yttrium;

namespace Yttrium
{
    static double Duration = 0.05;

#if 0
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
        static const size_t Required = 2 * sizeof(T);
        void               *myBuffer[ Y_WORDS_GEQ(Required) ];
        assert(IsPowerOfTwo(size));
        assert(0!=data);
        const size_t n = (size<<1);

        for(size_t j=1,i=1;i<n;i+=2)
        {
            if(j>i)
            {
                memcpy(myBuffer,&data[i],Required);
                memcpy(&data[i],&data[j],Required);
                memcpy(&data[j],myBuffer,Required);
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

#endif
    //static inline Hexadecimal B2H(const uint8_t x) { return Hexadecimal(x); }

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
            for(size_t i=arr.size();i>0;--i)
                arr[i] = brr[i] = Bring<T>::Get(ran);

            Y_ASSERT( 0 == memcmp( &arr[1], &brr[1], bytes) );

            {
                const uint64_t mark = WallTime::Ticks();
                XBitRev::Run_(arr.legacy(),size);
                tmxA += WallTime::Ticks()-mark;
            }

            {
                const uint64_t mark = WallTime::Ticks();
                XBitRev::Run(brr.legacy(),size);
                tmxB += WallTime::Ticks()-mark;
            }

            if(0!=memcmp( &arr[1], &brr[1], bytes) )
            {
                for(size_t i=1;i<=arr.size();++i)
                {
                    std::cerr << arr[i] << "/" << brr[i] << std::endl;
                }
                const uint8_t * const p = (uint8_t*) &arr[1];
                const uint8_t * const q = (uint8_t*) &brr[1];
                for(size_t i=0;i<bytes;++i)
                {
                    std::cerr << "\t" << Hexadecimal(p[i]) << "/" << Hexadecimal(q[i]);
                    if(p[i]!=q[i]) std::cerr << " <--";
                    std::cerr << std::endl;
                }

            }

            Y_ASSERT( 0 == memcmp( &arr[1], &brr[1], bytes) );


        }
        while( chrono(tmxA) < Duration );

        const double rateA = cycles / chrono(tmxA);
        const double rateB = cycles / chrono(tmxB);

        std::cerr << "\trate = " << HumanReadable( uint64_t(rateA) ) << "Ops / " << HumanReadable(rateB) << "Ops";

        std::cerr << std::endl;
    }

    typedef V2D<uint16_t> XBR;

    static inline
    uint16_t CountXBR(const uint16_t size, Vector<XBR> &xbr)
    {
        assert(IsPowerOfTwo(size));
        xbr.free();
        const size_t n     = (size<<1);
        for(uint16_t j=1,i=1;i<n;i+=2)
        {
            if(j>i)
            {
                const XBR br(i,j);
                xbr << br;
            }
            size_t m=size;
            while (m >= 2 && j > m)
            {
                j -= m;
                m >>= 1;
            }
            j += m;
        }
        return (uint16_t)(xbr.size());
    }


}


#define SHOW(SIZE) std::cerr << "#" << SIZE << " = " << sizeof(XBR##SIZE)/sizeof( XBR##SIZE  [0]) << " => " << sizeof(XBR##SIZE) << " bytes" << std::endl

Y_UTEST(fft_xbr)
{

    if(argc>1) Duration = atof(argv[1]);

    Random::Rand ran;
    for(unsigned shift=0;shift<=10;++shift)
    {
        const size_t size = 1 << shift;
        std::cerr << "size=" << size << std::endl;
        TestXBR<float>(size,ran);
        TestXBR<double>(size,ran);
        TestXBR<long double>(size,ran);
    }

    SHOW(4);
    SHOW(8);
    SHOW(16);
    SHOW(32);
    SHOW(64);
    SHOW(128);
    SHOW(256);
    SHOW(512);
    SHOW(1024);
    SHOW(2048);
    SHOW(4096);


}
Y_UDONE()


static inline void OutputXBR(const XBR &p, OutputStream &fp)
{
    fp("\t\t{ 0x%04x, 0x%04x }", p.x, p.y);
}

Y_UTEST(fft_xbr_build)
{
    Vector<XBR>  xbr;
    for(unsigned shift=0;shift<=12;++shift)
    {
        const uint16_t size = 1 << shift;
        const uint16_t nbr = CountXBR(size,xbr);
        const size_t bytes = nbr * sizeof(XBR);
        std::cerr << "size=" << size << " => xch=" << nbr << ", bytes=" << bytes << std::endl;
        //std::cerr << xbr << std::endl;

        std::cerr << std::endl;
        if(nbr<=0) continue;


        {
            const String headerName = Formatted::Get("xbr%u.hpp",size);
            OutputFile   header(headerName);
            header << "//! \\file\n";
            header("#ifndef Y_XBR%u_Included\n",size);
            header("#define Y_XBR%u_Included\n",size);
            header << "#include \"y/config/starting.hpp\"\n";
            header("namespace Yttrium {\n");
            header("\textern const uint16_t XBR%u[%u][2]; //!< XBitReversal Table\n",size,nbr);
            header("}\n");
            header("#endif\n");
        }

        {
            const String sourceName = Formatted::Get("xbr%u.cpp",size);
            //std::cerr << "-> " << sourceName << std::endl;
            OutputFile   source(sourceName);
            source("#include \"xbr%u.hpp\"\n",size);
            source << "namespace Yttrium {\n";
            source("\tconst uint16_t XBR%u[%u][2]={\n",size,nbr);
            OutputXBR(xbr[1],source);
            for(uint16_t i=2;i<=nbr;++i)
            {
                source << ",\n";
                OutputXBR(xbr[i],source);
            }
            source <<"\n";
            source << "\t};\n";
            source << "}\n";
        }
    }
}
Y_UDONE()
