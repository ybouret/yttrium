
#include "y/chemical/plexus/conservation/canon.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {


            Canon:: Canon(const Law &first) :
            LList(),
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

            void Canon:: compile()  
            {
                assert( 0 == maxNameLength );
                assert( 0 == species->size );

                for(const LNode *l=head;l;l=l->next)
                {
                    const Law &law = **l; enroll( law );
                    for(const Actor *a=law->head;a;a=a->next)
                    {
                        Coerce(species) << a->sp;
                        Coerce(sformat).enroll(a->sp);
                    }
                }
            }

        }

    }

}
