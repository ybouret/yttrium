
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

namespace
{
    static const uint16_t Table[3][2] =
    {
        { 2,3 },
        { 5,7 },
        { 11,13}
    };

    static void Process(const void * const t, const size_t n)
    {
        const uint32_t * const word = static_cast<const uint32_t *>(t);
        for(size_t i=0;i<n;++i)
        {
            union {
                uint32_t w;
                uint16_t s[2];
            } alias = { word[i] };
            Core::Display(std::cerr,alias.s,2) << std::endl;
        }
    }


}

Y_UTEST(dft_fmt)
{
    Vector<Swp> swps;

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

            if(size>=256) break;
        }

    }
    
    Process(Table,3);


}
Y_UDONE()

