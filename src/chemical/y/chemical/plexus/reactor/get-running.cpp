#include "y/chemical/plexus/reactor.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        xreal_t Reactor:: getRunning(XMLog &           xml,
                                     XWritable &       C0,
                                     const XReadable & K0)
        {
            size_t cycle = 0;
        CYCLE:
            ++cycle;
            Y_XML_SECTION_OPT(xml,"GetRunning", "cycle="<<cycle);
            {
                running.free();
                bool emergency = false;
                for(const ENode *en=cluster->equilibria->head;en;en=en->next)
                {
                    const Equilibrium & eq  = **en;
                    const xreal_t       eK  = eq(K0,TopLevel);
                    XWritable        &  cc  = cluster.gather(Ceq[running.size+1],C0);
                    const Outcome       out = solve1D(eq,eK,cc,SubLevel,C0,TopLevel);
                    switch(out.st)
                    {
                        case Blocked:
                            Y_XMLOG(xml,out);
                            continue;

                        case Crucial:
                            Y_XMLOG(xml,out);
                            emergency = true;
                            running << out;
                            continue;

                        case Running:
                            if(emergency) continue; // discard in case or emergency
                            Y_XMLOG(xml,out);
                            running << out;
                            continue;
                    }
                }
                if(emergency)
                {
                    MergeSort::Call(running.removeIf(IsRunning),ByDecreasingAX);
                    const Outcome &worst = **running.head;
                    Y_XML_COMMENT(xml,"using crucial " << worst.eq.name);
                    cluster.expand(C0,worst.cc);
                    if(xml.verbose)
                    {
                        cluster.show(*xml << "C0=", TopLevel,"\t[", C0, "]", xreal_t::ToString) << std::endl;
                    }
                    goto CYCLE;
                }
            }
            const xreal_t S0 = score( cluster.gather(Cini,C0),SubLevel);
            Y_XML_COMMENT(xml, "|active| = " << running.size << "/" << cluster->equilibria->size);
            Y_XMLOG(xml, "S0 = " << S0.str() );
            return S0;
        }
    }

}


