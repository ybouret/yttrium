
#include "y/io/stream/pack64.hpp"
#include "y/type/ints.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/utest/run.hpp"
#include "y/io/libc/output.hpp"
#include "y/io/libc/input.hpp"

using namespace Yttrium;


Y_UTEST(io_pack64)
{

    const uint64_t reg[] = { 0, 1, 0x1234 }; //, 0x10000, 0x12345678, IntegerFor<uint64_t>::Maximum };
    {
        Libc::OutputFile fp("pack64.dat");
        {
            size_t written = 0;
            for(size_t i=0;i<sizeof(reg)/sizeof(reg[0]);++i)
            {
                const uint64_t qw = reg[i];
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

        }
    }



}
Y_UDONE()
