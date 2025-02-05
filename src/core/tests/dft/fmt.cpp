
#include "y/dft/dft.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/system/rtti.hpp"
#include "y/type/utils.hpp"
#include <cstdio>

using namespace Yttrium;

#include "y/random/park-miller.hpp"
#include "y/system/wtime.hpp"
#include "y/text/human-readable.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/stream/libc/output.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/calculus/bit-count.hpp"

#include <cstring>

namespace
{

    class Swp
    {
    public:
        Swp(const size_t _i, const size_t _j) noexcept : i(_i), j(_j) { assert(i<j); }
        Swp(const Swp &_) noexcept : i(_.i), j(_.j) { assert(i<j); }
        ~Swp() noexcept {}

#if 0
        inline friend std::ostream & operator<<(std::ostream &os, const Swp &swp)
        {
            os << '('  << swp.i << ',' << swp.j << ')';
            return os;
        }
#endif

        inline unsigned requiredBits() const noexcept
        {
            assert(i<j);
            const unsigned maxBits = BitsFor(j);
            return maxBits;
        }


        const size_t i;
        const size_t j;
    private:
        Y_DISABLE_ASSIGN(Swp);
    };

    static inline
    void FormatBuild(Vector<Swp>  &swps,
                     const size_t  size)
    {
        const size_t n = size << 1;
        swps.free();
        for(size_t i=1,j=1;i<n;i+=2)
        {
            if(j>i)
            {
                //Swap2(data+i,data+j);
                swps << Swp(i,j);
            }
            size_t m=size;
            while( (m >= 2) && (j > m) )
            {
                j -= m;
                m >>= 1;
            }
            j += m;
        }

    }
}

#include "y/text/plural.hpp"
#include "y/random/fill.hpp"

namespace
{


    double duration = 0.2;


    static inline void Generate()
    {
        Random::ParkMiller ran;
        Vector<Swp>        swps;

        for(unsigned p=0; p <= 16; ++p)
        {
            const size_t size = size_t(1) << p;
            std::cerr << "size=" << std::setw(5) << size << " = 2^" << std::setw(2) << p << ":";
            FormatBuild(swps,size);
            const size_t nswp = swps.size();
            unsigned maxBits = 0;
            for(size_t i=nswp;i>0;--i) maxBits = Max(maxBits,swps[i].requiredBits());

            std::cerr << " #" << std::setw(6) << nswp;
            const unsigned useBits = NextPowerOfTwo( Max<unsigned>(maxBits,8) );
            const unsigned useWord = useBits/8;
            const size_t   useRoom = useWord * 2 * nswp;

            std::cerr << " maxBits=" << std::setw(2) << maxBits;
            std::cerr << " useBits=" << std::setw(2) << useBits;
            std::cerr << " useWord=" << std::setw(2) << useWord;
            std::cerr << " useRoom=" << std::setw(7) << useRoom << " byte" << Plural::s(useRoom);
            std::cerr << std::endl;

            if(useRoom>0 && useRoom<65536)
            {
                const String id  = Formatted::Get("%u", unsigned(size) );
                const String nn  = Formatted::Get("%u", unsigned(nswp) );
                const String hdr = "_" + id + ".hpp";
                {

                    OutputFile   fp(hdr);
                    fp << "//! \\file\n";
                    fp << "#ifndef Y_DFT_Fmt" << id << "_Included\n";
                    fp << "#define Y_DFT_Fmt" << id << "_Included\n";
                    fp << "#include \"y/config/starting.hpp\"\n";
                    fp << "namespace Yttrium {\n\n";
                    {
                        fp << "\tstruct DFT_Fmt" << id << " {\n";
                        fp << "\t\tstatic const unsigned Count=" << nn << ";     //!< count\n";
                        fp << "\t\tstatic const uint16_t Table[" << nn << "][2]; //!< table\n";
                        fp << "\t\tstatic const size_t   Result=" << Formatted::Get( "%u", unsigned(size<<1) ) << "; //!< 2*size\n";
                        fp << "\t};\n";
                    }
                    fp << "}\n";
                    fp << "#endif\n";
                }

                {
                    const String fn = "_" + id + ".cpp";
                    OutputFile   fp(fn);
                    fp << "#include \"" << hdr  << "\"\n";
                    fp << "namespace Yttrium {\n\n";
                    {
                        fp << "\tconst uint16_t DFT_Fmt" << id << "::Table[" << nn << "][2] = {\n";
                        {
                            for(size_t i=1;i<=nswp;++i)
                            {
                                const Swp &swp = swps[i];
                                fp("\t\t{ 0x%04x, 0x%04x }", unsigned(swp.i), unsigned(swp.j) );
                                if(i<nswp) fp << ",";
                                fp << "\n";
                            }
                        }
                        fp << "\t};\n";
                    }
                    fp << "}\n";
                }
                

            }

        }
    }


    template <typename T,typename FMT> static inline
    void TestFMT(Random::Bits &ran)
    {
        static const size_t size = FMT::Result/2;
        Vector<T>    data(FMT::Result,0);
        for(size_t i=FMT::Result;i>0;--i) data[i] = ran.to<T>();
        Vector<T>    temp(data);
        Y_ASSERT( 0 == memcmp(data(),temp(),data.size()*sizeof(T)) );

        DFT::Format(data()-1,size);
        DFT::Format<T,FMT>(temp()-1);
        Y_ASSERT( 0 == memcmp(data(),temp(),data.size()*sizeof(T)));

        WallTime chrono;
        uint64_t tmxRaw = 0;
        uint64_t tmxTab = 0;
        uint64_t start = WallTime::Ticks();
        data.ld(0);
        T * const entry = data()-1;
        size_t   cycles = 0;
        do
        {
            ++cycles;
            {
                const uint64_t mark = WallTime::Ticks();
                DFT::Format_(entry,size);
                tmxRaw += WallTime::Ticks() - mark;
            }
            {
                const uint64_t mark = WallTime::Ticks();
                DFT::Format<T,FMT>(entry);
                tmxTab += WallTime::Ticks() - mark;
            }
        } while( chrono.since(start) < duration );
        const long double speedRaw = static_cast<long double>(cycles) / chrono(tmxRaw);
        const long double speedTab = static_cast<long double>(cycles) / chrono(tmxTab);
        std::cerr << " raw: " << HumanReadable(speedRaw) << " | tab: " << HumanReadable(speedTab) << std::endl;


    }


    template <typename FMT>
    static inline
    void TestFMT()
    {
        std::cerr << "Testing FMT" << FMT::Result/2 << std::endl;
        Random::ParkMiller ran;

        TestFMT<float,FMT>(ran);
        TestFMT<double,FMT>(ran);
        TestFMT<long double,FMT>(ran);

    }
}




Y_UTEST(dft_fmt)
{

    if(argc>1)
    {
        duration = ASCII::Convert::ToReal<double>(argv[1],"duration");
    }

    if(false)
        Generate();


    TestFMT<DFT_Fmt4>();
    TestFMT<DFT_Fmt8>();
    TestFMT<DFT_Fmt16>();
    TestFMT<DFT_Fmt32>();
    TestFMT<DFT_Fmt64>();
    TestFMT<DFT_Fmt128>();
    TestFMT<DFT_Fmt256>();
    TestFMT<DFT_Fmt512>();
    TestFMT<DFT_Fmt1024>();
    TestFMT<DFT_Fmt2048>();
    TestFMT<DFT_Fmt4096>();
    TestFMT<DFT_Fmt8192>();
    TestFMT<DFT_Fmt16384>();
    TestFMT<DFT_Fmt32768>();


}
Y_UDONE()

