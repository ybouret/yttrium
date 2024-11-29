
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


        namespace
        {
            struct CallActivity
            {
                const Components &eq;
                const xReal       eK;
                const XReadable  &C;
                const Level       L;
                XMul             &xmul;
            };
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

            // prepare workspace with initial conditions
            eq.transfer(C1,L1,C0,L0);

            CallActivity F = { eq, eK, C1, L1, xmul };

            {
                XTriplet x = { zero, zero, zero };
                XTriplet f = { eq.activity(xmul, eK, C1, L1), zero, zero };

                std::cerr << "f=" << f << std::endl;

            }




            // done
            return outcome;
        }
    }

}
