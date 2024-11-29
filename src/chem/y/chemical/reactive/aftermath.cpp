
#include "y/chemical/reactive/aftermath.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Aftermath:: Aftermath()
        {
        }

        Aftermath:: ~Aftermath() noexcept
        {
        }

        Outcome Aftermath:: solve(const Components &eq,
                                  const xReal       eK,
                                  XWritable        &C1,
                                  const Level       L1,
                                  const XReadable  &C0,
                                  const Level       L0)
        {

            const Situation st = eq.situation(C0,L0);
            switch(st)
            {
                case Blocked: return Outcome(Blocked,eq,eK,C0,L0);
                case Running:
                    break;
            }

            // prepare outcome
            const Outcome outcome(Running,eq,eK,C1,L1);

            // prepare workspace
            eq.transfer(C1, L1, C0, L0);

            // done
            return outcome;
        }
    }

}
