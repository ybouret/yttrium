
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
    static const uint64_t u64max = UnsignedInt<8>::Maximum;
    static const uint64_t u64last = u64max/10;
    static const uint64_t u64chk  = u64last*10;

    std::cerr << "umax  = " << Hexadecimal(u64max)  << " / " << u64max << std::endl;
    std::cerr << "ulast = " << Hexadecimal(u64last) << std::endl;
    std::cerr << "uchk  = " << Hexadecimal(u64chk) << std::endl;



    if(argc>1)
    {
        const String   s = argv[1];
        if(s.size()>0 && s[1] == '-')
        {
            const int64_t i = ASCII::Convert::ToI64(s);
            std::cerr << "i = " << i << std::endl;
        }
        else
        {
            const uint64_t u = ASCII::Convert::ToU64(s);
            const int64_t  i = ASCII::Convert::ToI64(s);
            std::cerr << "u=" << Hexadecimal(u,Hexadecimal::Compact) << std::endl;
            std::cerr << "i = " << i << std::endl;
        }
    }

}
Y_UDONE()
