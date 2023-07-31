
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
        const uint64_t   qw = ran.to<uint64_t>(i);
        const IO::Pack64 p(qw);
    }

#endif

#if 0
    {
        Libc::OutputFile fp("pack64.dat");
        {
            size_t written = 0;
            for(unsigned i=0;i<=3;++i)
            {
                const uint64_t qw = ran.to<uint64_t>(i);
                std::cerr << Hexadecimal(qw);
                std::cerr << std::endl;
                written += IO::Pack64::Emit(fp,qw);
            }
            std::cerr << "written=" << written << std::endl;
        }
    }

    {
        Libc::InputFile fp("pack64.dat");
        while(fp.ready())
        {
            uint64_t qw = IO::Pack64::Read(fp,"trial");
            std::cerr << "qw=" << Hexadecimal(qw) << std::endl;
            break;
        }
    }
#endif

}
Y_UDONE()
