#include "y/mkl/fit/zero-bode.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace Yttrium;
using namespace MKL;

Y_UTEST(fit_zbode)
{

    Fit::ZeroBode<float> bode(4);

    std::cerr << bode << " => " << bode.inferred() << std::endl;

    bode >> 1.2;
    std::cerr << bode << " => " << bode.inferred() << std::endl;

    bode >> 1.1;
    std::cerr << bode << " => " << bode.inferred() << std::endl;

    bode.save("bode2.dat");

    bode >> 1.1;
    std::cerr << bode << " => " << bode.inferred() << std::endl;

    bode.save("bode3.dat");

    bode >> 1.1;
    std::cerr << bode << " => " << bode.inferred() << std::endl;

    bode.save("bode3b.dat");

}
Y_UDONE()
