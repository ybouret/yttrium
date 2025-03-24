
#include "y/chemical/plexus/reactor.hpp"
#include "y/mkl/opt/minimize.hpp"
#include "y/mkl/opt/bracket.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;


        xreal_t Reactor:: optimize1D(const xreal_t Sini)
        {
            MKL::Bracketing::Verbose = true;
            MKL::Minimizing::Verbose = true;
            
            Reactor &self = *this;
            XTriplet U    = { 0,    0,  1 };
            XTriplet S    = { Sini, 0,  score(Cend,SubLevel) };
            const xreal_t Uopt = Minimize<xreal_t>::Locate(Minimizing::Inside,self,U,S);
            const xreal_t Sopt = self(Uopt);
            return Sopt;
        }

    }

}


