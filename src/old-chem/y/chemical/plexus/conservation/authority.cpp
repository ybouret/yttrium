
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
                if(acts->size>0)
                {
                    Y_XML_SECTION(xml, "Chemical::Conservation::Acts");
                    for(const Act *act=acts->head;act;act=act->next)
                    {
                        Y_XMLOG(xml, *act);
                    }
                }

            }

            Authority:: ~Authority() noexcept
            {
            }
            
        }

    }

}

