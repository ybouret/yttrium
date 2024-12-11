
#include "y/chemical/plexus/mix.hpp"
#include "y/woven/survey/integer.hpp"
#include "y/woven/subspaces.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Mix:: buildCombinatorics(XMLog &xml, Equilibria &eqs)
        {
            Y_XML_SECTION(xml, "Mix::Combinatorics");


            {
                WOVEn::IntegerSurvey survey(xml);
                const Matrix<int>    mu(TransposeOf,my.topology);
                WOVEn::Explore(mu,survey,false);
                survey.sort();
            }
        }
    }

}
