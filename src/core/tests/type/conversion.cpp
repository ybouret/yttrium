#include "y/type/conversion.hpp"
#include "y/utest/run.hpp"

#include "y/apex/natural.hpp"

using namespace Yttrium;


#define SHOW(TEST) std::cerr << #TEST << " = " << (TEST) << std::endl;

Y_UTEST(type_conversion)
{
    SHOW(Y_Is_SuperSubClass(Object,Apex::Number));
    SHOW(Y_Is_SuperSubClass(Apex::Number,apn));
    SHOW(Y_Is_SuperSubClass_Strict(Apex::Number,apn));
    SHOW(Y_Is_SuperSubClass_Strict(apn,apn));

}
Y_UDONE()
