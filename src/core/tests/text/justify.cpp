
#include "y/text/justify.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;


Y_UTEST(text_justify)
{

    size_t w = 20;
    std::cerr << '[' << Justify("Hello",w,Justify::Left) << ']' << std::endl;
    std::cerr << '[' << Justify("Hello",w,Justify::Center) << ']' << std::endl;
    std::cerr << '[' << Justify("Hello",w,Justify::Right) << ']' << std::endl;

}
Y_UDONE()
