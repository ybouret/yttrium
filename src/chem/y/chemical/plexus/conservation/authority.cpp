
#include "y/chemical/plexus/conservation/authority.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {
            Authority:: Authority(Mix &mix, XMLog &xml) :
            laws(mix,xml),
            acts(laws)
            {
            }
            
        }

    }

}

