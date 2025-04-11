
#include "y/chemical/plexus/equalizer/two-sided/klist.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizer
        {
            KList:: KList() noexcept : ESolo() {}
            
            KList:: ~KList() noexcept {}

            void KList:: show(XMLog &xml, const char * const uuid) const
            {
                Show(xml,uuid,*this);
            }
        }

    }

}

