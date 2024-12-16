#include "y/chemical/plexus/solver.hpp"
#include "y/stream/libc/output.hpp"

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
                my.reset();

                bool crucial = false;
                for(const ENode *en=mix->head;en;en=en->next)
                {
                    const Equilibrium &eq  = **en;
                    const xReal        eK  = eq(K,TopLevel);
                    const size_t       ei  = my.size+1;
                    XWritable         &cc  = mix.transfer(Csolve[ei].ld(zero),SubLevel,C,L);
                    XWritable         &dc  = deltaC[ei].ld(zero);
                    const Outcome      out = solve(eq,eK,cc,SubLevel,C,L);

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

                    const xReal    xi = extent(out, C, L, dc, SubLevel);
                    const Prospect pro(out,xi,dc);
                    my << pro;
                }

                if(my.size<=0)
                {
                    Y_XML_COMMENT(xml, "all blocked");
                    return;
                }

                assert(my.size>0);
                if(crucial)
                {
                    Y_XML_SECTION(xml, "CrucialRemoval");
                    MergeSort::Call(my.removeIf(isRunning),byDecreasingAX); assert(my.size>0);

                    my.update();
                    my.show(xml);

                    // apply most crucial
                    const Prospect &pro = **my.head;
                    pro.out.upload(C,L);
                    goto PROBE;
                }

            }

            {
                Y_XML_SECTION(xml, "Find");

                // initializing
                my.update();
                mix.transfer(Cini,SubLevel,C,L);

                const xReal f0 = objectiveFunction(C,L);
                Y_XMLOG(xml, "f0 = " << real_t(f0) );
                for(ProNode *pn=my.head;pn;pn=pn->next)
                {
                    Prospect &pro = **pn;
                    assert(pro.out.st == Running);
                    assert(pro.out.eq.running(C,L));
                    assert(pro.out.eq.running(pro.out.C,pro.out.L));
                    pro.ff = objectiveFunction(pro.out.C,pro.out.L);
                }
                my.show(xml);

                {
                    OutputFile fp("pro.dat");
                    const size_t np=100;
                    for(const ProNode *pn=my.head;pn;pn=pn->next)
                    {
                        const Prospect &pro = **pn;
                        mix.transfer(Cend, SubLevel, pro.out.C, pro.out.L);

                        
                        for(size_t i=0;i<=np;++i)
                        {
                            const double u = double(i)/np;
                            const double f = double(objectiveFunction(u));
                            fp("%.15g %.15g\n",u,f);
                        }
                        fp << "\n";
                    }

                }



            }




        }
    }

}


