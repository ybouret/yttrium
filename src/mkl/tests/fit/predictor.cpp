#include "y/mkl/fit/predictor.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace MKL;

template <typename T>
static inline
void testPred()
{
    Fit::Predictor<T> pred;
    pred.assign(0.8);
    std::cerr << *pred << " => " << pred.compute() << std::endl;
    pred.refine(0.7);
    std::cerr << *pred << " => " << pred.compute() << std::endl;

    pred.refine(0.68);
    std::cerr << *pred << " => " << pred.compute() << std::endl;

    Libc::OutputFile fp("pred.dat");
    pred.save(fp);

}

Y_UTEST(fit_predictor)
{
    Y_SIZEOF(Fit::Predictor<float>);
    Y_SIZEOF(Fit::Predictor<double>);
    Y_SIZEOF(Fit::Predictor<long double>);
    Y_SIZEOF(Fit::Predictor< XReal<float> >);
    Y_SIZEOF(Fit::Predictor< XReal<double> >);
    Y_SIZEOF(Fit::Predictor< XReal<long double> >);

    testPred<float>();
    testPred<double>();
    testPred<long double>();
    testPred< XReal<float> >();
    testPred< XReal<double> >();
    testPred< XReal<long double> >();
}
Y_UDONE()
