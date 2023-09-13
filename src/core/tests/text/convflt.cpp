

#include "y/text/ascii/convert.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/text/ops.hpp"
#include "y/type/ints.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include <cstring>

using namespace Yttrium;


Y_UTEST(text_convflt)
{


    if(argc>1)
    {
        const char  *msg = argv[1];
        const size_t len = StringLength(msg);
        const String str = msg;

        const float f1 = ASCII::Convert::ToReal<float>(msg,len,"f1");
        const float f2 = ASCII::Convert::ToReal<float>(msg,"f2");
        const float f3 = ASCII::Convert::ToReal<float>(str,"f3");

        const double d1 = ASCII::Convert::ToReal<double>(msg,len,"d1");
        const double d2 = ASCII::Convert::ToReal<double>(msg,"d2");
        const double d3 = ASCII::Convert::ToReal<double>(str,"d3");

        const long double l1 = ASCII::Convert::ToReal<long double>(msg,len,"l1");
        const long double l2 = ASCII::Convert::ToReal<long double>(msg,"l2");
        const long double l3 = ASCII::Convert::ToReal<long double>(str,"l3");

        std::cerr << f1 << " : " << f2 << " : " << f3 << std::endl;
        std::cerr << d1 << " : " << d2 << " : " << d3 << std::endl;
        std::cerr << l1 << " : " << l2 << " : " << l3 << std::endl;

    }


}
Y_UDONE()
