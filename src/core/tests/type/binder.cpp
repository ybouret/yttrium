#include "y/type/binder.hpp"

#include "y/utest/run.hpp"
#include "y/string.hpp"


using namespace Yttrium;



Y_UTEST(type_binder)
{
    Binder<TL1(const int)> ib(2); std::cerr << ib.arg1 << std::endl;
    Binder<NullType>       nb;

    std::cerr << ib << std::endl;
    Y_SIZEOF(ib);
    std::cerr << nb << std::endl;
    Y_SIZEOF(nb);

    int    v     = 1;
    String s     = "hello";
    Binder<TL2(String, int &)> xb(s,v);
    std::cerr << xb << std::endl;
    s = "world";
    std::cerr << xb << std::endl;
    Y_CHECK("hello" == xb.arg1);
    xb.arg2 = 2;
    std::cerr << xb << " / " << v << std::endl;
    Y_CHECK(2==v);
}
Y_UDONE()

