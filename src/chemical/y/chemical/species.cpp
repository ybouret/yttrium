
#include "y/chemical/species.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        
        Species::~Species() noexcept
        {
        }

        void Species:: viz(OutputStream &fp) const
        {
            Node(fp,this) << '[';
            Label(fp,name) << ",shape=egg";
            Endl(fp<<']');
        }

        std::ostream & operator<<(std::ostream &os, const Species &sp)
        {
            os << '[' << sp.name << ']';
            return os;
        }

        xreal_t Species:: Concentration(Random::Bits &ran)
        {
            const real_t p = PMIN + (PMAX-PMIN)*ran.to<real_t>();
            return pow(10.0,p);
        }

    }
}
