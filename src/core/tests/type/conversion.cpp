#include "y/type/conversion.hpp"
#include "y/utest/run.hpp"

#include "y/kemp/natural.hpp"

using namespace Yttrium;
using namespace Kemp;

#define SHOW(TEST) std::cerr << #TEST << " = " << (TEST) << std::endl;

Y_UTEST(type_conversion)
{
    SHOW(Y_Is_SuperSubClass(Object,Number));
    SHOW(Y_Is_SuperSubClass(Object,Natural));
    SHOW(Y_Is_SuperSubClass(Number,apn));
    SHOW(Y_Is_SuperSubClass_Strict(Number,apn));
    SHOW(Y_Is_SuperSubClass_Strict(apn,apn));

}
Y_UDONE()
