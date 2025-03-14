
#include "y/chemical/plexus/clusters.hpp"
#include "y/mkl/algebra/ortho-space.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;

        void Cluster:: conservations(XMLog &xml)
        {
            Y_XML_SECTION(xml, "conservations");

            Matrix<apz> Q;
            if(!OrthoSpace::Make(Q,topology))
            {
                throw Specific::Exception(CallSign, "no orthogonal space");
            }

        }
    }

}

