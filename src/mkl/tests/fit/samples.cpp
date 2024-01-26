

#include "y/mkl/fit/sequential.hpp"

#include "y/mkl/fit/sample/heavy.hpp"
#include "y/mkl/fit/sample/light.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace MKL;

Y_UTEST(fit_samples)
{

    static const double _t1[] = { 30,80,140,200,270,320,430,550,640,720,830,890 };
    static const double _x1[] = { 4.414520,5.011710,5.632319,6.194379,6.721311,7.330211,8.009368,8.735363,9.297424,9.707260,10.339578,10.878220};
    const size_t        _n1   = sizeof(_t1)/sizeof(_t1[0]);

    static const double _t2[] = { 60,120,200,270,360,460,580,670,790,920,1040 };
    static const double _x2[] = { 5.199063,5.854801,6.662763,7.365340,8.067916,8.782201,9.578454,10.175644,10.878220,11.651054,12.213115};
    const size_t        _n2   = sizeof(_t2)/sizeof(_t2[0]);

    Fit::Sample<double,double>::Pointer S1 = new Fit::LightSample<double,double>("S1",_t1,_x1,_n1);
    Fit::Sample<double,double>::Pointer S2 = new Fit::LightSample<double,double>("S2",_t2,_x2,_n2);

    Fit::Sample<double,double>::Pointer H1 = new Fit::HeavySample<double,double>("H1");


    std::cerr << S1 << std::endl;
    std::cerr << S2 << std::endl;
    std::cerr << H1 << std::endl;

}
Y_UDONE()
