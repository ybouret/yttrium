
#include "y/chemical/reactive/conservation/laws.hpp"
#include "y/chemical/reactive/cluster/grouping.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/algebra/ortho-space.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {

            using namespace MKL;

            Laws:: ~Laws() noexcept {}

            const char * const Laws:: CallSign = "Chemical::Conservation::Laws";
            
            Laws:: Laws(const Grouping &g, XMLog &xml) :
            Law::List()
            {
                Y_XML_SECTION(xml,CallSign);

                Y_XML_COMMENT(xml, "building orthogonal space of topology");
                Y_XMLOG(xml, "Nu = " << g.iTopology);
                
                Matrix<apz> Q;
                if(!OrthoSpace::Make(Q,g.iTopology))
                    throw Specific::Exception(CallSign,"no orthogonal space of topology");

                Y_XMLOG(xml, "Q  = " << Q);

            }

        }

    }

}

