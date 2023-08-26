
#include "y/text/ascii/convert.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/text/ops.hpp"
#include "y/type/ints.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include <cstring>

using namespace Yttrium;


Y_UTEST(text_convert)
{


    if(argc>1)
    {
        const String   s = argv[1];
        if(s.size()>0 && s[1] == '-')
        {
            const int64_t i = ASCII::Convert::ToI64(s,"i");
            std::cerr << "i = " << i << std::endl;
        }
        else
        {
            const uint64_t u = ASCII::Convert::ToU64(s,"u");
            const int64_t  i = ASCII::Convert::ToI64(s,"i");
            std::cerr << "u = " << Hexadecimal(u,Hexadecimal::Compact) << " / " << u << std::endl;
            std::cerr << "i = " << i << std::endl;
        }
    }

    std::cerr << int(ASCII::Convert::To<int8_t>("-13")) << std::endl;
    std::cerr << unsigned(ASCII::Convert::To<uint16_t>("17")) << std::endl;

}
Y_UDONE()
