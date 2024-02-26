#include "y/mkl/strain/filter.hpp"
#include "y/utest/run.hpp"
#include "y/mkl/xreal.hpp"

using namespace Yttrium;
using namespace MKL;



Y_UTEST(strain_filter)
{
    Strain::Filter<float>       ff;
    Strain::Filter<double>      fd;
    Strain::Filter<long double> fl;

    Strain::Filter< XReal<float> > xff;
    Strain::Filter< XReal<double> > xfd;
    Strain::Filter< XReal<long double> > xfl;
}
Y_UDONE()

