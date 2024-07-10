
#include "y/chemical/species.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        double Species:: Conc(Random::Bits &ran)
        {
            static const double pmin(PMIN);
            static const double pmax(PMAX);
            static const double delta(pmax-pmin);
            const double p = pmin + delta * ran.to<double>();
            return pow(10.0,p);
        }

        Species:: ~Species() noexcept {
        }

        std::ostream & operator<<(std::ostream &os, const Species &sp)
        {
            return os<<sp.name;
        }


        void Species:: viz(OutputStream &fp,
                           const String &color,
                           const bool    conserved) const
        {
            Node(fp,this) << '[';
            Label(fp,name);
            fp << ",shape=oval,style=" << (conserved ? "dashed" : "solid" );
            fp << ",color=" << color;
            fp << ']';
            Endl(fp);

        }
    }

}

