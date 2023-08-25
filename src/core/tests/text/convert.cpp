
#include "y/text/convert.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/text/ops.hpp"
#include "y/type/ints.hpp"
#include "y/utest/run.hpp"

#include <cstring>

using namespace Yttrium;


Y_UTEST(text_convert)
{
    static const int64_t i64max =  SignedInt<8>::Maximum;
    static const int64_t i64min = -SignedInt<8>::Minimum;
    static const uint64_t u64max = UnsignedInt<8>::Maximum;

    std::cerr << "MaxI64 = " << Hexadecimal(i64max) << std::endl;
    std::cerr << "MinI64 = " << Hexadecimal(i64min) << std::endl;
    std::cerr << "MaxU64 = " << Hexadecimal(u64max) << "/" << u64max << std::endl;


    if(argc>1)
    {
        const uint64_t u = Convert::ToU64(argv[1], StringLength(argv[1]));
        std::cerr << "u=" << Hexadecimal(u,Hexadecimal::Compact) << std::endl;
    }

}
Y_UDONE()
