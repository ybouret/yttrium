
#include "y/chemical/plexus/equalizer/hauler.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizer
        {


            Hauler:: ~Hauler() noexcept
            {
            }

            Hauler:: Hauler(const SBank &_) noexcept : SRepo(_)
            {
            }

            Hauler:: Hauler(const Hauler &_) : SRepo(_)
            {
            }

            void Hauler:: vanish(XWritable &C, const Level L) const noexcept
            {
                const xreal_t zero;
                for(const SNode *sn=head;sn;sn=sn->next)
                {
                    (**sn)(C,L) = zero;
                }
            }

            void Hauler:: zforward(const Components &E,
                                   XWritable        &C,
                                   const Level       L) const noexcept
            {
                assert(size>0);
                E.boldMove(C,L,getXi());
                vanish(C,L);
            }

            void Hauler:: zreverse(const Components &E,
                                   XWritable        &C,
                                   const Level       L) const noexcept
            {
                assert(size>0);
                E.boldMove(C,L,-getXi());
                vanish(C,L);
            }

        }

    }

}
