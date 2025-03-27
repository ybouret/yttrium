#include "y/chemical/plexus/conservation/canons.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            Canons:: ~Canons() noexcept
            {
            }

            Canons:: Canons(const ListOf<Law> &laws) :
            CxxListOf<Canon>()
            {
                for(const Law *law=laws.head;law;law=law->next)
                {
                    
                }
            }

        }
    }

}


