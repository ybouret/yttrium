
#include "y/ink/zero-flux.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(zero_flux)
{
    for(unit_t dim=1;dim<=5;++dim)
    {
        std::cerr << "dim=" << dim << std::endl;
        const Ink::ZeroFlux zf(dim);
        for(unit_t i=-10;i<=10;++i)
        {
            std::cerr << std::setw(3) << i << " => " << zf[i] << std::endl;
        }
    }
}
Y_UDONE()


