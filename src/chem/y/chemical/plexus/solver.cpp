
#include "y/chemical/plexus/solver.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Solver:: ~Solver() noexcept {}

        Solver:: Solver(const Mix &_) :
        mix(_),
        Csolve(mix->size,mix->species.size),
        deltaC(mix->size,mix->species.size),
        Cini(mix->species.size),
        Cend(mix->species.size),
        aftermath(),
        pbank(),
        plist(pbank)
        {

        }

        const char * const Solver:: CallSign = "Chemical::Solver";


      
        void Solver:: update() noexcept
        {
            forget();
            for(const ProNode *pn=plist.head;pn;pn=pn->next)
                enroll(**pn);
            
        }

      


    }

}
