#include "y/stream/data/output-buffer.hpp"
#include "y/stream/io/pack64.hpp"
#include "y/type/ints.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"
#include "y/stream/libc/input.hpp"
#include "y/calculus/bit-count.hpp"

#include "y/random/bits.hpp"
#include "y/stream/data/input.hpp"

#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"

using namespace Yttrium;


Y_UTEST(io_pack64)
{

    Random::Rand ran;

    for(unsigned ibit=0;ibit<=64;++ibit)
    {
        const uint64_t qw = ran.to<uint64_t>(ibit); Y_ASSERT(ibit==BitCount::For(qw));
        OutputFile    fp("max64.dat");
        const size_t     nw = IO::Pack64::Emit(fp,qw);
        std::cerr << ibit << " => " << nw << std::endl;
    }

    for(unsigned i=0;i<=64;++i)
    {
        for(size_t loop=0;loop<32;++loop)
        {
            const uint64_t   qw = ran.to<uint64_t>(i);


            {
                OutputFile fp("pack64.dat");
                (void) IO::Pack64::Emit(fp,qw);
            }
            

            {
                InputFile fp("pack64.dat");
                const uint64_t ld = IO::Pack64::Read(fp, "trial");
                //std::cerr << "ld:" << Hexadecimal(ld) << std::endl;
                Y_ASSERT(ld==qw);
            }
        }

    }

    std::cerr << "Testing With OutputBuffer" << std::endl;
    OutputBuffer<9> p64;
    std::cerr << "Capacity=" << p64.Capacity << ", Required=" << p64.Required << ", Internal=" << p64.Internal << std::endl;
    for(unsigned i=0;i<=64;++i)
    {
        for(size_t loop=0;loop<32;++loop)
        {
            p64.erase();
            const uint64_t qw = ran.to<uint64_t>(i);
            const size_t   nw = IO::Pack64::Emit(p64,qw);
            Y_ASSERT( nw == p64.measure() );
            InputDataStream fp(p64);
            const uint64_t ld = IO::Pack64::Read(fp, "trial");
            Y_ASSERT(ld==qw);
        }

    }


    {
        Vector<uint8_t> target;
        Vector<size_t>  source;
        source << 1 << 2 << 3;
        IO::Pack64::Compress(target,source);
        std::cerr << "source=" << source << std::endl;

        std::cerr << "source=";
        Hexadecimal::Display(std::cerr, &source[1], source.size() ) << std::endl;

        std::cerr << "target=";
        Hexadecimal::Display(std::cerr, &target[1], target.size() ) << std::endl;
    }


}
Y_UDONE()
