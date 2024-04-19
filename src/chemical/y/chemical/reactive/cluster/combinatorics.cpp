
#include "y/chemical/reactive/cluster.hpp"
#include "y/woven/subspaces.hpp"
#include "y/woven/survey/integer.hpp"
#include "y/apex/mylar.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Cluster:: buildCombinatorics(Equilibria &eqs, XMLog &xml)
        {
            WOVEn::IntegerSurvey survey(xml);

            // WOVen survey linear combination
            {
                Matrix<int> NuT(TransposeOf,Nu);
                Matrix<int> NuTx;
                Apex::Mylar::Compress(NuTx,NuT);
                Y_XMLOG(xml,"NuT  = "   << NuT);
                Y_XMLOG(xml,"NuTx = "  << NuTx);
                WOVEn::Explore(NuTx, survey, false, xml);
            }

            // convert to stoichiometry
            

        }
    }

}
