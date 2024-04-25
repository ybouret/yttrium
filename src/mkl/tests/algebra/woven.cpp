

#include "y/woven/subspaces.hpp"
#include "y/woven/survey/integer.hpp"
#include "y/utest/run.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/sequence/vector.hpp"
using namespace Yttrium;



Y_UTEST(algebra_woven)
{

    Vector<unsigned> coef;
    for(int i=1;i<argc;++i)
    {
        coef << ASCII::Convert::To<unsigned>(argv[i],"coef");
    }

    if(coef.size()<2)
    {
        std::cerr << "not enough coefficients" << std::endl;
        return 0;
    }




}
Y_UDONE()
