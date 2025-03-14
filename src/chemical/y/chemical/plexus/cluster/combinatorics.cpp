
#include "y/chemical/plexus/clusters.hpp"
#include "y/mkl/algebra/ortho-space.hpp"
#include "y/system/exception.hpp"

#include "y/apex/api/ortho/coven/survey/integer.hpp"
#include "y/apex/api/count-non-zero.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;
        using namespace Apex;
        using namespace Ortho;
        using namespace Coven;

        void Cluster:: combinatorics(XMLog &xml)
        {
            Y_XML_SECTION(xml, "combinatorics");
            
            const IntegerSurvey survey(xml,topologyT,0);
            std::cerr << "survey=" << survey << std::endl;
            if(survey->size<=0) {
                Y_XML_COMMENT(xml,"no combinatorics");
                return;
            }
        }

    }

}
