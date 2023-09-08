#include "y/type/temporary.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(type_temporary)
{

    int a = 5;
    Y_CHECK(5==a);
    {
        const Temporary<int> tmp(a,12);
        Y_CHECK(12==a);
    }
    Y_CHECK(5==a);

}
Y_UDONE()
