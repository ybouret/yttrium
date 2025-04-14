
#include "y/chemical/plexus/equalizing/two-sided/glist.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizing
        {
            GList:: GList(const GBank &_) noexcept : GList_(_) {}

            GList:: ~GList() noexcept {}

            void GList:: show(XMLog &xml, const char * const uuid) const
            {
                Show(xml,uuid,*this);
            }

        }

    }

}

