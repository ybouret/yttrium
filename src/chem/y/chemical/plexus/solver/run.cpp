#include "y/chemical/plexus/solver.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        static inline bool isRunning(const Prospect &pro) noexcept
        {
            return Running == pro.out.st;
        }

        static inline SignType byDecreasingAX(const ProNode * const lhs,
                                              const ProNode * const rhs) noexcept
        {
            return Sign::Of( (**rhs).ax, (**lhs).ax );
        }

        void Solver:: run(XMLog &xml, XWritable &C, const Level L, const XReadable &K)
        {

            Y_XML_SECTION(xml,CallSign);
            const xReal zero;

            //------------------------------------------------------------------
            //
            //
            // probing all equilibria to build non-crucial prospects
            //
            //
            //------------------------------------------------------------------
            {

                Y_XML_SECTION(xml, "Probe");
                size_t probe = 0;
            PROBE:
                ++probe;
                Y_XML_COMMENT(xml, "probing phase space #" << probe);
                if(xml.verbose)
                {
                    mix(xml() << "C=","\t[",C,"]") << std::endl;
                }
                plist.free();
                forget();

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
                            if(crucial) continue;;
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
                    Y_XML_SECTION(xml, "CrucialRemoval");
                    MergeSort::Call(plist.removeIf(isRunning),byDecreasingAX); assert(plist.size>0);


                    update();
                    for(const ProNode *pn=plist.head;pn;pn=pn->next)
                    {
                        const Prospect &pro = **pn; assert( Crucial == pro.out.st );
                        if(xml.verbose) print(xml() << "@",pro,Justify::Left) << " => |xi| = | " << real_t(pro.xi) << " |" << std::endl;
                    }

                    const Prospect &pro = **plist.head;
                    pro.out.upload(C,L);
                    goto PROBE;
                }

            }

            {
                Y_XML_SECTION(xml, "Find");
                update();
                mix.transfer(Cini,SubLevel,C,L);
                for(ProNode *pn=plist.head;pn;pn=pn->next)
                {
                    Prospect &pro = **pn;
                    assert(pro.out.st == Running);
                    assert(pro.out.eq.running(C,L));
                    pro.revise(aftermath.xmul);
                    if(xml.verbose) print(xml() << "@",pro,Justify::Left)
                        << " : |xi| = | " << std::setw(15) << real_t(pro.xi) << " |"
                        << " ff = " << std::setw(15) << real_t(pro.ff)
                        << std::endl;
                }
            }




        }
    }

}


