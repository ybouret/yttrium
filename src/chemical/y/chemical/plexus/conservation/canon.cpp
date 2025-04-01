
#include "y/chemical/plexus/conservation/canon.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {


            Canon:: Canon(const Law &first) :
            LList(),
            species(),
            anxious(),
            sformat(),
            next(0),
            prev(0)
            {
                (*this) << first;
            }

            Canon:: ~Canon() noexcept
            {
            }

            bool Canon:: accepts(const Law &law) const noexcept
            {
                for(const LNode *ln=head;ln;ln=ln->next)
                {
                    if( (**ln).hasCommonActorWith(law) ) return true;
                }
                return false;
            }


            bool Canon:: accepts(const Canon &rhs) const noexcept
            {
                for(const LNode *l=head;l;l=l->next)
                {
                    if(rhs.accepts(**l)) return true;
                }
                return false;
            }

        }

    }

}

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            void Canon:: compile(const EList &definite)
            {
                assert( 0 == maxNameLength );
                assert( 0 == species->size );
                
                // collect species
                for(const LNode *l=head;l;l=l->next)
                {
                    const Law &law = **l; enroll( law );
                    for(const Actor *a=law->head;a;a=a->next)
                    {
                        Coerce(species) << a->sp;
                        Coerce(sformat).enroll(a->sp);
                    }
                }

                // collect definite into anxious
                for(const ENode *en=definite.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    if(eq.gotAnyOf(*species))
                        Coerce(anxious) << eq;
                }
            }

        }

    }

}
