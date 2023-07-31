
#include "y/io/stream/pack64.hpp"
#include "y/type/ints.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/utest/run.hpp"
#include "y/io/libc/output.hpp"
#include "y/io/libc/input.hpp"

#include "y/random/bits.hpp"


using namespace Yttrium;


Y_UTEST(io_pack64)
{

    Random::Rand ran;

#if 1
    for(unsigned i=0;i<=64;++i)
    {
        for(size_t loop=0;loop<32;++loop)
        {
            const uint64_t   qw = ran.to<uint64_t>(i);

            {
                Libc::OutputFile fp("pack64.dat");
                IO::Pack64::Emit(fp,qw);
            }


            {
                Libc::InputFile fp("pack64.dat");
                const uint64_t ld = IO::Pack64::Read(fp, "trial");
                std::cerr << "ld:" << Hexadecimal(ld) << std::endl;
                Y_ASSERT(ld==qw);
            }
        }

    }
#endif


}
Y_UDONE()
