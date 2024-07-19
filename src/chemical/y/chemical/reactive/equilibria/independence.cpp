#include "y/chemical/reactive/equilibria/independence.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Independence:: CallSign = "Chemical::Independence";
        
        Independence:: Independence(const size_t capacity) :
        list(capacity)
        {

        }

        Independence:: ~Independence() noexcept
        {
        }


        void Independence:: start(const Equilibrium &eq)
        {
            list.free();
            grow(eq);
            for(const ENode *en=eq.sire.head;en;en=en->next)
                grow(**en);
        }

        
        


    }

}
