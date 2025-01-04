
#include "y/chemical/plexus/mix.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Mix:: buildConfiguration(XMLog &      xml,
                                      Equilibria & eqs,
                                      const CrBank &cb,
                                      const SBank  &sb)
        {
            buildConservations(xml);
            keeper = new Keeper(*this);

            buildCombinatorics(xml,eqs);
            solver = new Solver(*this,cb,sb);
        }
    }

}
