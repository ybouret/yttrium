#include "y/chemical/plexus/solver.hpp"
#include "y/stream/libc/output.hpp"

//#include "y/mkl/opt/minimize.hpp"
#include "y/exception.hpp"

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



        static inline String toReal(const xReal x) {
            return Formatted::Get("%.15g", double(x) );
        }

        void Solver:: run(XMLog &xml, XWritable &C, const Level L, const XReadable &K)
        {

            Y_XML_SECTION(xml,CallSign);
            OutputFile::Overwrite("score.dat");

            size_t cycle = 0;
        PROBE:
            ++cycle;

            //------------------------------------------------------------------
            //
            //
            //
            // probing all equilibria, with crucial state detection
            //
            //
            //
            //------------------------------------------------------------------
            for(size_t probe=1;;++probe)
            {
                Y_XML_COMMENT(xml, "probing cycle #" << cycle << "." << probe);

                //--------------------------------------------------------------
                //
                //
                //  initialize probe
                //
                //
                //--------------------------------------------------------------
                my.reset();
                bool crucial = false;
                if(xml.verbose)
                    mix(xml() << "C=","\t[", mix.transfer(Ctmp, SubLevel, C, L),"]",toReal) << std::endl;

                //--------------------------------------------------------------
                //
                //
                //  loop over equilibria
                //
                //
                //--------------------------------------------------------------
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
                        case Blocked: if(crucial) continue; // shortcut
                            Y_XMLOG(xml, "[Blocked] " << eq);
                            continue;

                        case Crucial: crucial = true;
                            Y_XMLOG(xml, "[Crucial] " << eq);
                            break;

                        case Running: if(crucial) continue; // shortcut
                            Y_XMLOG(xml, "[Running] " << eq);
                            break;
                    }

                    const xReal    xi = extent(out, C, L, dc, SubLevel);
                    const Prospect pro(out,xi,dc);
                    my << pro;
                }

                //--------------------------------------------------------------
                //
                //
                //  let's check what we got
                //
                //
                //--------------------------------------------------------------

                //--------------------------------------------------------------
                //
                // no active prospect ?
                //
                //--------------------------------------------------------------
                if(my.size<=0)
                {
                    Y_XML_COMMENT(xml, "all blocked");
                    return;
                }

                //--------------------------------------------------------------
                //
                // crucial prospects ?
                //
                //--------------------------------------------------------------
                assert(my.size>0);
                if(crucial)
                {
                    Y_XML_COMMENT(xml, "removing crucial");

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
                    continue;
                }

                assert(my.size>0);
                break;
            }


            //------------------------------------------------------------------
            //
            //
            //
            // we have my.size>0 of running prospects:
            // - objectiveFunction is defined
            // - setup f0 @ Cini=C
            //
            //
            //------------------------------------------------------------------
            Y_XML_COMMENT(xml, "running #" << my.size << "/" << mix->size);
            my.update();
            const xReal f0 = objectiveFunction( mix.transfer(Cini, SubLevel, C, L), SubLevel );
            Y_XMLOG(xml, "f0 = " << real_t(f0) );
            assert(isAcceptable(Cini,SubLevel));

            {
                OutputFile fp("score.dat",true);
                fp("%g %.15g\n", double(cycle), double(f0));
            }

            //------------------------------------------------------------------
            //
            //
            //
            // Optimize all prospects
            //
            //
            //
            //------------------------------------------------------------------
            Y_XML_COMMENT(xml, "optimize prospects");
            for(ProNode *pn=my.head;pn;pn=pn->next)
            {
                Prospect &  pro   = **pn; assert(isAcceptable(pro.C,pro.L));
                const xReal f_opt = optimize(f0, pro.ff = objectiveFunction( mix.transfer(Cend,SubLevel,pro.C,pro.L), SubLevel ));
                if(f_opt<f0)
                {
                    //----------------------------------------------------------
                    //
                    // update prospect to f_opt @ Ctmp
                    //
                    //----------------------------------------------------------
                    Y_XMLOG(xml, "(+) " << Justify(pro.eq.name,my.maxKeySize,Justify::Right) << " => " << real_t(f_opt) );
                    mix.transfer(pro.C, pro.L, Ctmp, SubLevel);
                    pro.ax = (pro.xi = pro.extent(xadd, C, L, pro.dc, SubLevel)).abs();
                    pro.ff = f_opt;
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // degenerate prospect
                    //
                    //----------------------------------------------------------
                    Y_XMLOG(xml, "(-) " << Justify(pro.eq.name,my.maxKeySize,Justify::Right));
                    mix.transfer(pro.C,pro.L,C,L);
                    pro.xi = pro.ax = zero;
                    pro.dc.ld(zero);
                    pro.ff = f0;
                }
            }
            MergeSort::Call(my,byIncreasingFF);
            my.show(xml);


            //------------------------------------------------------------------
            //
            //
            //
            // Check most promising prospect
            //
            //
            //
            //------------------------------------------------------------------
            Y_XML_COMMENT(xml, "searching better score #" << cycle);
            const Prospect &pro = **my.head;
            xReal           f1  = pro.ff;      assert(f1<=f0);
            Y_XMLOG(xml, "[pro] " << toReal(f1) << " / " << toReal(f0) << " @" << pro.key());

            {
                Cend.ld(pro.C);
                save("pro.dat",100);
            }

            //------------------------------------------------------------------
            //
            //
            // early return on single equilibrium/zero score
            //
            //
            //------------------------------------------------------------------
            if(1==my.size || f1 <= 0.0)
            {
                Y_XML_COMMENT(xml, "numerical solution '" << pro.eq.name << "'");
                mix.transfer(C,L,pro.C,pro.L);
                return;
            }

            //------------------------------------------------------------------
            //
            //
            // check if prospect decreased f0
            //
            //
            //------------------------------------------------------------------
            bool decreased = false;
            if(f1<f0)
            {
                decreased = true;
                mix.transfer(C,L,pro.C,pro.L);
                Y_XML_COMMENT(xml, "keep [pro] result");
            }
            else
            {
                Y_XML_COMMENT(xml, "drop [pro] result");
            }





            //------------------------------------------------------------------
            //
            //
            // check if ode decreased current state
            //
            //
            //------------------------------------------------------------------
            {
                const xReal ftmp = computeStepDS(xml,f0);
                Y_XMLOG(xml, "[ode] " << toReal(ftmp) << " / " << toReal(f1) );
                if(ftmp<f1)
                {
                    decreased = true;
                    f1=ftmp;
                    mix.transfer(C,L,Ctmp,SubLevel);
                    Y_XML_COMMENT(xml, "keep [ode] result");
                }
                else
                {
                    Y_XML_COMMENT(xml, "drop [ode] result");
                }
            }

            
            //------------------------------------------------------------------
            //
            //
            // check if nra decreased current state
            //
            //
            //------------------------------------------------------------------
            {
                const xReal ftmp = computeStepAS(xml,f0);
                Y_XMLOG(xml, "[nra] " << toReal(ftmp) << " / " << toReal(f1) );
                if(ftmp<f1)
                {
                    decreased = true;
                    f1=ftmp;
                    mix.transfer(C,L,Ctmp,SubLevel);
                    Y_XML_COMMENT(xml, "keep [nra] result");
                }
                else
                {
                    Y_XML_COMMENT(xml, "drop [nra] result");
                }
            }





            if(decreased)
            {
                Y_XML_COMMENT(xml, "decreased");
                goto PROBE;
            }


        }
    }

}


