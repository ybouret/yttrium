

#include "y/text/human-readable.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;

namespace
{

}


Y_UTEST(text_human_readable)
{
    
    for(size_t i=0;i<HumanReadable::Number;++i)
    {
        std::cerr << HumanReadable::Divide[i].suffix << " : " << HumanReadable::Divide[i].factor << std::endl;
    }

    HumanReadable hr1(20);
    HumanReadable hr2(20000);

    std::cerr << HumanReadable(10)        << std::endl;
    std::cerr << HumanReadable(123456789) << std::endl;

}
Y_UDONE()
