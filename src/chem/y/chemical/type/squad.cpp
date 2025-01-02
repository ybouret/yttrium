
#include "y/chemical/type/squad.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Squad:: Squad() noexcept : species(), sformat() {}

        Squad:: ~Squad() noexcept
        {
        }

        Squad:: Squad(const Squad &_) : species(_.species), sformat(_.sformat) {}


        void Squad:: buildSpeciesFormat() noexcept {
            sformat.forget();
            for(const SNode *sn=species.head;sn;sn=sn->next)
                sformat.enroll(**sn);
        }


    }

}


