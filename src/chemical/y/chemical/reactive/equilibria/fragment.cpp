
#include "y/chemical/reactive/equilibria/fragment.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Fragment::  Fragment() noexcept :  EList(), species() {}
        Fragment:: ~Fragment() noexcept {}
        Fragment::  Fragment(const Fragment &sub) : 
        EList(sub), species(sub.species)
        {
            assert(sub.size==size);
            assert(sub.species.size==species.size);
        }

     
        void Fragment:: transfer(XWritable       &target,
                                 const Level      tgtlvl,
                                 const XReadable &source,
                                 const Level     &srclvl) const noexcept
        {
            for(const SNode *node=species.head;node;node=node->next)
            {
                const size_t * const indx = (**node).indx;
                target[ indx[tgtlvl] ] = source[ indx[srclvl] ];
            }
        }

    }

}


