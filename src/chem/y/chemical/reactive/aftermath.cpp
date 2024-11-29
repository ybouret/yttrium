
#include "y/chemical/reactive/aftermath.hpp"
#include "y/system/exception.hpp"

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

                xReal operator()(const xReal xi)
                {
                    return eq.activity(xmul, eK, C, L, xi);
                }

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

            while(true)
            {
                XTriplet x = { zero, zero, zero };
                XTriplet f = { eq.activity(xmul, eK, C1, L1), zero, zero };

                std::cerr << "f=" << real_t(f.a) << std::endl;

                switch( Sign::Of(f.a) )
                {
                    case __Zero__: goto DONE;

                    case Negative:
                        std::cerr << "need xi<0 for " << eq.attrText() << std::endl;
                        exit(1);

                    case Positive:
                        std::cerr << "need xi>0 for " << eq.attrText() << std::endl;
                        switch(eq.attr)
                        {
                            case Nebulous: goto DONE;
                            case ReacOnly:
                            case Definite:
                                // reach one limiting reactant => reacActivity=0
                                x.c = eq.reac.limitingExtent(C1,L1);
                                f.c = eq.prodActivity(xmul,C1, L1,x.c); assert(f.c<0.0);
                                break;

                            case ProdOnly:

                                exit(1);
                                break;
                        }
                        break;
                }

                std::cerr << "x.c=" << real_t(x.c) << std::endl;
                std::cerr << "f.c=" << real_t(f.c) << std::endl;


                exit(1);

            }

            DONE:



            // done
            return outcome;
        }
    }

}
