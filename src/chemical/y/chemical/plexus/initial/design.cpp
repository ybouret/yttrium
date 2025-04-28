
#include "y/chemical/plexus/initial/design.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {

            const char * const Design:: CallSign            = "Initial::Design";
            const char * const Design:: Logo                = "Logo";
            const char * const Design:: LogoExpr            = "[.][:word:]+";
            const char * const Design:: _FixedConcentration = "FixedConcentration";
            const char * const Design:: _SteadyConservation = "SteadyConservation";

            Y_PROXY_IMPL(Design,my)


            Design:: ~Design() noexcept
            {

            }

            void Design:: add(Axiom * const axiom) noexcept
            {
                assert(0!=axiom);
                assert(!my.owns(axiom));
                my.pushTail(axiom);
            }

            
          
        }
    }
}



#include "y/lingo/pattern/matching.hpp"
#include "y/chemical/plexus/initial/axiom/steady-conservation.hpp"
namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {

            void Design:: add(Clusters &     clusters,
                              const String & expr,
                              const xreal_t  Ctot)
            {
                Lingo::Matching                 m(expr,0);
                const Conservation::Law * const law = clusters.preserving(m);
                if(0==law) throw Specific::Exception(CallSign, "no conservation matching '%s' in %s", expr.c_str(), Clusters::CallSign);
                my.pushTail( new SteadyConservation(*law,Ctot) );
            }

        }

    }
}


