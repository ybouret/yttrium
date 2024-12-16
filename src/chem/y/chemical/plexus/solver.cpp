
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


        static inline bool isRunning(const Prospect &pro) noexcept
        {
            return Running == pro.out.st;
        }


        void Solver:: run(XMLog &xml, XWritable &C, const Level L, const XReadable &K)
        {

            Y_XML_SECTION(xml,CallSign);
            const xReal zero;

            {
                plist.free();
                bool crucial = false;
                for(const ENode *en=mix->head;en;en=en->next)
                {
                    const Equilibrium &eq  = **en;
                    const xReal        eK  = eq(K,TopLevel);
                    const size_t       ei  = plist.size+1;
                    XWritable         &cc  = mix.transfer(Csolve[ei].ld(zero),SubLevel,C,L);
                    XWritable         &dc  = deltaC[ei].ld(zero);
                    const Outcome      out = aftermath.solve(eq,eK,cc,SubLevel,C,L);

                    switch(out.st)
                    {
                        case Blocked:
                            Y_XMLOG(xml, "[Blocked] " << eq);
                            continue;

                        case Crucial:
                            crucial = true;
                            Y_XMLOG(xml, "[Crucial] " << eq);
                            break;

                        case Running:
                            if(crucial) continue;
                            Y_XMLOG(xml, "[Running] " << eq);
                            break;
                    }

                    const xReal    xi = aftermath.extent(out, C, L, dc, SubLevel);
                    const Prospect pro(out,xi,dc);
                    plist << pro;
                }

                if(plist.size<=0)
                {
                    Y_XML_COMMENT(xml, "all blocked");
                    return;
                }

                assert(plist.size>0);
                if(crucial)
                {
                    plist.removeIf(isRunning);
                    assert(plist.size>0);
                }

            }




        }


    }

}
