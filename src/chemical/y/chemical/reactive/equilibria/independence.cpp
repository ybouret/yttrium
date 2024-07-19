#include "y/chemical/reactive/equilibria/independence.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Independence:: Independence(const size_t capacity) :
        bank(capacity),
        repo(bank)
        {

        }

        Independence:: ~Independence() noexcept
        {
        }


        void Independence:: start(const Equilibrium &eq)
        {
            repo.free();
            grow(eq);
            for(const ENode *en=eq.sire.head;en;en=en->next)
                grow(**en);
        }

        static inline
        SignType ecmp(const Equilibrium &lhs, const Equilibrium &rhs) noexcept
        {
            return Sign::Of(lhs.indx[TopLevel],rhs.indx[TopLevel]);
        }

        bool Independence:: grow(const Equilibrium &eq)
        {
            //__________________________________________________________________
            //
            //
            // check cases
            //
            //__________________________________________________________________
            switch(repo.size)
            {
                case 0: repo << eq; return true;
                case 1:
                    switch( ecmp(eq,**repo.head) )
                    {
                        case Positive: repo << eq; break; // push tail
                        case Negative: repo >> eq; break; // push head
                        case __Zero__:      return false; // same index
                    }
                    return true;
                default:
                    break;
            }

            // check lower node
            ENode *lower = repo.head;
            switch( ecmp(eq,**lower) )
            {
                case Negative: repo >> eq; return  true; // push head
                case __Zero__:             return false; // same index
                case Positive: break;                    // default
            }

            // check upper node
            ENode * const upper = repo.tail;
            switch( ecmp(eq,**upper) )
            {
                case Negative: break;                   // default
                case __Zero__: return false;            // same index
                case Positive: repo << eq; return true; // push tail
            }

            // sequential check
            ENode *probe = lower->next;
            while(upper != probe)
            {
                switch( ecmp(eq,**probe) )
                {
                    case Negative: goto FOUND;
                    case __Zero__: return false; // same indices
                    case Positive: break;        // check next
                }
                lower = probe;
                probe = probe->next;
            }
        FOUND:


            return false;
        }



    }

}
