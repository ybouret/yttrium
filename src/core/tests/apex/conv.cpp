
#include "y/apex/natural.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/text/ascii/convert.hpp"

using namespace Yttrium;


Y_UTEST(apex_conv)
{

    Random::Rand ran;
    apn numer(ran.leq(10),ran);
    apn denom(1+ran.leq(10),ran);
    std::cerr.precision(30);

    uint64_t nn = 0;
    uint64_t dd = 0;
    if(argc>2)
    {
        numer = nn = ASCII::Convert::To<uint64_t>(argv[1],"numer");
        denom = dd = ASCII::Convert::To<uint64_t>(argv[2],"denom");
        std::cerr << float(nn)/dd << std::endl;
    }

    std::cerr << numer << "/" << denom << std::endl;
    const float f = apn::ToReal<float>(numer,denom);
    std::cerr << f << std::endl;
    const double d = apn::ToReal<double>(numer,denom);
    std::cerr << d << std::endl;
    const long double x = apn::ToReal<long double>(numer,denom);
    std::cerr << x << std::endl;




}
Y_UDONE()
