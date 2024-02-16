#include "y/mkl/fit/zero-bode.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace Yttrium;
using namespace MKL;

Y_UTEST(fit_zbode)
{

    Fit::ZeroBode<float> bode(3);

    std::cerr << bode.x << " / " << bode.y << "=>" << bode.inferred() <<  std::endl;

    bode.feed(2,1.1);
    std::cerr << bode.x << " / " << bode.y << "=>" << bode.inferred() <<  std::endl;

    bode.feed(1,1.02);
    std::cerr << bode.x << " / " << bode.y << "=>" << bode.inferred() <<  std::endl;

    bode.save("bode2.dat");

    bode.feed(0.5,1.01);
    std::cerr << bode.x << " / " << bode.y << "=>" << bode.inferred() <<  std::endl;
    bode.save("bode3.dat");


}
Y_UDONE()
