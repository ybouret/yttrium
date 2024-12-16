#include "y/chemical/plexus/solver.hpp"
#include "y/stream/libc/output.hpp"

#include "y/mkl/opt/minimize.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        static inline bool isRunning(const Prospect &pro) noexcept
        {
            return Running == pro.st;
        }

        static inline SignType byDecreasingAX(const ProNode * const lhs,
                                              const ProNode * const rhs) noexcept
        {
            return Sign::Of( (**rhs).ax, (**lhs).ax );
        }

        static inline SignType byIncreasingFF(const ProNode * const lhs,
                                              const ProNode * const rhs) noexcept
        {
            return Sign::Of( (**lhs).ff, (**rhs).ff );
        }

        void Solver:: run(XMLog &xml, XWritable &C, const Level L, const XReadable &K)
        {

            Y_XML_SECTION(xml,CallSign);


            {
                //--------------------------------------------------------------
                //
                //
                // probing all equilibria to build non-crucial prospects
                //
                //
                //--------------------------------------------------------------
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

                    //----------------------------------------------------------
                    // keep crucial part
                    //----------------------------------------------------------
                    MergeSort::Call(my.removeIf(isRunning),byDecreasingAX); assert(my.size>0);
                    my.update();
                    my.show(xml);

                    //----------------------------------------------------------
                    // apply most crucial
                    //----------------------------------------------------------
                    const Prospect &pro = **my.head;
                    pro.upload(C,L);
                    goto PROBE;
                }

            }

            {
                Y_XML_SECTION(xml, "Find");


                //--------------------------------------------------------------
                //
                //
                // Initialize with all outcome
                //
                //
                //--------------------------------------------------------------
                my.update();
                mix.transfer(Cini,SubLevel,C,L);
                const xReal f0 = objectiveFunction(C,L);
                Y_XML_COMMENT(xml, "initial state");
                Y_XMLOG(xml, "f0 = " << real_t(f0) );
                for(ProNode *pn=my.head;pn;pn=pn->next)
                {
                    Prospect &pro = **pn;
                    assert(pro.st == Running);
                    assert(pro.eq.running(C,L));
                    assert(pro.eq.running(pro.C,pro.L));
                    pro.ff = objectiveFunction(pro.C,pro.L);
                }
                my.show(xml);

                {

                    OutputFile fp("pro0.dat");
                    const size_t np=100;
                    for(const ProNode *pn=my.head;pn;pn=pn->next)
                    {
                        const Prospect &pro = **pn;
                        mix.transfer(Cend, SubLevel, pro.C, pro.L);


                        for(size_t i=0;i<=np;++i)
                        {
                            const double u = double(i)/np;
                            const double f = double(objectiveFunction(u));
                            fp("%.15g %.15g\n",u,f);
                        }
                        fp << "\n";
                    }
                }

                if(1==my.size)
                {
                    const Prospect &pro = **my.head;
                    Y_XML_COMMENT(xml, "single equilibrium '" << pro.key() << "'");
                    mix.transfer(C,L,pro.C,pro.L);
                    return;
                }


                //--------------------------------------------------------------
                //
                //
                // Refining
                //
                //
                //--------------------------------------------------------------
                Y_XML_COMMENT(xml, "refine  state");
                Solver &F = *this;
                for(ProNode *pn=my.head;pn;pn=pn->next)
                {
                    Prospect &pro = **pn;
                    mix.transfer(Cend, SubLevel, pro.C, pro.L);
                    XTriplet    x     = {zero, zero, one    };
                    XTriplet    f     = {f0,   f0,   pro.ff };
                    const xReal x_opt = MKL::Minimize<xReal>::Locate(MKL::Minimizing::Inside, F, x, f);
                    const xReal f_opt = F(x_opt);


                    if(f_opt<f0)
                    {
                        // update prospect
                        Y_XMLOG(xml, "(+) " << Justify(pro.eq.name,my.maxKeySize,Justify::Right) << " @" << real_t(x_opt) << " => " << real_t(f_opt) );
                        mix.transfer(pro.C, pro.L, Ctmp, SubLevel);
                        pro.ax = (pro.xi = pro.extent(xadd, C, L)).abs();
                        pro.ff = f_opt;
                    }
                    else
                    {
                        // degenerate prospect
                        Y_XMLOG(xml, "(-) " << Justify(pro.eq.name,my.maxKeySize,Justify::Right));
                        mix.transfer(pro.C,pro.L,C,L);
                        pro.xi = pro.ax = zero;
                        pro.dc.ld(zero);
                        pro.ff = f0;
                    }
                }

                //--------------------------------------------------------------
                //
                //
                // sort by increasing FF
                //
                //
                //--------------------------------------------------------------
                Y_XML_COMMENT(xml, "ordered state");
                MergeSort::Call(my,byIncreasingFF);
                my.show(xml);

                {

                    OutputFile fp("pro1.dat");
                    const size_t np=100;
                    for(const ProNode *pn=my.head;pn;pn=pn->next)
                    {
                        const Prospect &pro = **pn;
                        mix.transfer(Cend, SubLevel, pro.C, pro.L);

                        for(size_t i=0;i<=np;++i)
                        {
                            const double u = double(i)/np;
                            const double f = double(objectiveFunction(u));
                            fp("%.15g %.15g\n",u,f);
                        }
                        fp << "\n";
                    }
                }


                buildODE(xml,f0);




            }




        }
    }

}


