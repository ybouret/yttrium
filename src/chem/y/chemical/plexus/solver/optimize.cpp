
#include "y/chemical/plexus/solver.hpp"
#include "y/mkl/opt/minimize.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        xReal Solver:: optimize(const xReal f0, const xReal f1)
        {
            XTriplet x = { zero, zero, one };
            XTriplet f = { f0,   f0,   f1  };

            const xReal x_opt = MKL::Minimize<xReal>::Locate(MKL::Minimizing::Inside, *this, x, f);
            return (*this)(x_opt);
        }
    }

}

