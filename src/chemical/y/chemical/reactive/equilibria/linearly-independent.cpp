#include "y/chemical/reactive/equilibria/linearly-independent.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const LinearlyIndependent:: CallSign = "Chemical::LinearlyIndependent";

        LinearlyIndependent:: LinearlyIndependent(const size_t capacity) :
        list(capacity,AsParameter)
        {

        }

        LinearlyIndependent:: ~LinearlyIndependent() noexcept
        {
        }


        void LinearlyIndependent:: start(const Equilibrium &eq)
        {
            list.free();
            (void)grow(eq);
            for(const ENode *en=eq.sire.head;en;en=en->next)
                (void)grow(**en);
        }

        
        


    }

}
