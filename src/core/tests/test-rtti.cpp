#include "y/system/rtti.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(rtti)
{
    std::cerr << TypeName<String>() << std::endl;

    const RTTI & ts = RTTI::Of<String>();
    const RTTI & td = RTTI::Of<double>();

    std::cerr << "ts.name=" << ts.name() << std::endl;
    std::cerr << "td.name=" << td.name() << std::endl;
    
}
Y_UDONE()



