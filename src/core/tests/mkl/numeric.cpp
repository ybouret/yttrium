

#include "y/mkl/numeric.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;


#define Y_SHOW(FIELD) std::cerr << std::setw(16) << #FIELD << " = " << std::setw(16) << MKL::Numeric<T>::FIELD << std::endl

template <typename T>
static inline void displayNumeric(const char *id)
{
    std::cerr << "-- " << id << " --" << std::endl;
    Y_SHOW(PI);
    Y_SHOW(MIN);
    Y_SHOW(MAX);
    Y_SHOW(EPSILON);
    Y_SHOW(MANT_DIG);
    Y_SHOW(DIG);
    Y_SHOW(MIN_EXP);
    Y_SHOW(MAX_EXP);
    Y_SHOW(RADIX);
    Y_SHOW(FTOL);
    Y_SHOW(SQRT_EPSILON);
    Y_SHOW(GOLDEN);
    Y_SHOW(INV_GOLDEN);
    std::cerr << std::endl;

}

Y_UTEST(mkl_numeric)
{
    displayNumeric<float>("float");
    displayNumeric<double>("double");
    displayNumeric<long double>("long double");

    displayNumeric< XReal<float>  >( "xflt" );
    displayNumeric< XReal<double> >( "xdbl" );
    displayNumeric< XReal<long double> >( "xldbl" );


}
Y_UDONE()
