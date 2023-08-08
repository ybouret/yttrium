

#include "y/type/fourcc.hpp"
#include "y/utest/run.hpp"
#include "y/text/hexadecimal.hpp"

using namespace Yttrium;


Y_UTEST(type_fourcc)
{

    {
        const uint32_t uuid = Y_FOURCC(0x12, 0x34, 0x56, 0x78);
        std::cerr << Hexadecimal(uuid) << std::endl;
        Y_CHECK(uuid==0x12345678);
        std::cerr << FourCC::ToText(uuid) << std::endl;
    }

    {
        const uint32_t uuid = Y_FOURCC('H', 'a', 'l', 'o');
        std::cerr << FourCC::ToText(uuid) << std::endl;
    }
}
Y_UDONE()

