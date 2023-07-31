

#include "y/io/stream/pack64.hpp"
#include "y/type/ints.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/utest/run.hpp"
#include "y/io/libc/output.hpp"
#include "y/io/libc/input.hpp"
#include "y/random/bits.hpp"


using namespace Yttrium;


Y_UTEST(io_ints)
{

    Random::Rand ran;

    for(size_t loop=0;loop<10;++loop)
    {
        const uint8_t  u8  = ran.to<uint8_t>();
        const uint16_t u16 = ran.to<uint16_t>();
        const uint32_t u32 = ran.to<uint32_t>();
        const uint64_t u64 = ran.to<uint64_t>();

        std::cerr << Hexadecimal(u8, Hexadecimal::Compact);
        std::cerr << Hexadecimal(u16,Hexadecimal::Compact);
        std::cerr << Hexadecimal(u32,Hexadecimal::Compact);
        std::cerr << Hexadecimal(u64,Hexadecimal::Compact);
        std::cerr << std::endl;

        const size_t sz = ran.leq(10000);

        {
            Libc::OutputFile fp("ints.dat");
            size_t written = 0;
            written += fp.emitCBR(u8);
            written += fp.emitCBR(u16);
            written += fp.emitCBR(u32);
            written += fp.emitCBR(u64);
            written += fp.emitVBR(sz);
            std::cerr << "written=" << written << std::endl;
        }


        {
            Libc::InputFile fp("ints.dat");
            Y_CHECK(u8  == fp.readCBR<uint8_t>("u8") );
            Y_CHECK(u16 == fp.readCBR<uint16_t>("u16") );
            Y_CHECK(u32 == fp.readCBR<uint32_t>("u32") );
            Y_CHECK(u64 == fp.readCBR<uint64_t>("u64") );
        }
    }
    


}
Y_UDONE()
