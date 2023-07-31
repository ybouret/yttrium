
#include "y/io/stream/pack64.hpp"
#include "y/type/ints.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace Yttrium;


Y_UTEST(io_pack64)
{
    const uint64_t reg[] = { 0, 1, 0x1234, 0x10000, 0x12345678, IntegerFor<uint64_t>::Maximum };
    for(size_t i=0;i<sizeof(reg)/sizeof(reg[0]);++i)
    {
        const uint64_t qw = reg[i];
        std::cerr << Hexadecimal(qw);
        std::cerr << " => bytes=" << IO::Pack64::BytesFor(qw);
        std::cerr << std::endl;
        const IO::Pack64 p(qw);
    }

    Y_SIZEOF(IO::Pack64);
    

}
Y_UDONE()
