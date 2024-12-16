
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
        pbank(),
        plist(pbank)
        {
            
        }

        const char * const Solver:: CallSign = "Chemical::Solver";

        void Solver:: run(XMLog &xml, XWritable &C, const Level L, const XReadable &K)
        {

            Y_XML_SECTION(xml,CallSign);

            {
                plist.free();
                bool crucial = false;
                for(const ENode *en=mix->head;en;en=en->next)
                {
                    const Equilibrium &eq  = **en;
                    const xReal        eK  = eq(K,TopLevel);
                    const size_t       ei  = plist.size+1;
                    XWritable         &cc  = Csolve[ei];
                    XWritable         &dc  = deltaC[ei];
                    const Outcome      out = aftermath.solve(eq,eK,cc,SubLevel,C,L);

                    switch(out.st)
                    {
                        case Blocked: continue;
                        case Crucial: crucial = true; break;
                        case Running: if(crucial) continue; break;
                    }



                }
            }

        }


    }

}
