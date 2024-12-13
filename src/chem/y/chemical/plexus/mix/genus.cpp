#include "y/chemical/plexus/mix.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Mix:: Genus:: ~Genus() noexcept {}

        Mix:: Genus:: Genus(const ConservationAuth &auth, const SList &species) :
        conserved(),
        unbounded()
        {
            if(auth.isValid())
            {
                for(const Conservation::Law *law=auth->laws->head;law;law=law->next)
                {
                    for(const Actor *a=(*law)->head;a;a=a->next)
                    {
                        Coerce(conserved.book) |= a->sp;
                    }
                }
            }
            Coerce(conserved).compile();


            for(const SNode *sn=species.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                if(conserved.book.has(sp)) continue;
                Coerce(unbounded.book) += sp;
            }
            Coerce(unbounded).compile();
        }



        bool Mix::  Genus:: hasConserved(const Actors &A) const noexcept
        {
            for(const Actor *a=A->head;a;a=a->next)
            {
                if( conserved.book.has(a->sp) ) return true;
            }
            return false;
        }

        bool Mix:: Genus:: isLimiting(const Equilibrium &eq) const noexcept
        {
            return hasConserved(eq.reac) && hasConserved(eq.prod);
        }

    }

}


