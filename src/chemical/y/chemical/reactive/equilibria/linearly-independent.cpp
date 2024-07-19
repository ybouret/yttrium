#include "y/chemical/reactive/equilibria/linearly-independent.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const LinearlyIndependent:: CallSign = "Chemical::LinearlyIndependent";

        LinearlyIndependent:: LinearlyIndependent(const size_t capacity) :
        bank(capacity<<1),
        rank(bank,AsParameter),
        list(bank)
        {

        }

        LinearlyIndependent:: ~LinearlyIndependent() noexcept
        {
        }


        void LinearlyIndependent:: init() noexcept
        {
            rank.free();
            list.free();
        }

        

        bool LinearlyIndependent:: keep(const Equilibrium &eq)
        {
            // local copy
            ERank here(rank);

            //__________________________________________________________________
            //
            //
            // check eq is in here
            //
            //__________________________________________________________________
            if(!here.insert(eq)) return false;
            assert(here.has(eq));

            //__________________________________________________________________
            //
            //
            // check at least one of the parent is not present
            //
            //__________________________________________________________________
            {
                size_t n = eq.sire.size;
                if(n>0)
                {
                    for(const ENode *en=eq.sire.head;en;en=en->next)
                    {
                        const Equilibrium &parent = **en;
                        if( !here.insert(parent) )
                            --n; // already present
                        assert(here.has(parent));
                    }
                    if(n<=0) return false; // all parents were present!
                }
            }

            //__________________________________________________________________
            //
            //
            // keep modified list
            //
            //__________________________________________________________________
            list << eq;
            rank.tradeWith(here);
            return true;
        }

        LinearlyIndependent::ConstInterface & LinearlyIndependent:: surrogate() const noexcept
        {
            return list;
        }


        


    }

}
