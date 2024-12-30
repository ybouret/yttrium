#include "y/chemical/plexus/solver.hpp"
#include "y/stream/libc/output.hpp"
#include "y/chemical/plexus/mix.hpp"
#include "y/system/exception.hpp"

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



        static inline String toReal(const xReal &x) {
            return x.str();
        }

       

        void Solver:: run(XMLog &xml, XWritable &C, const Level L, const XReadable &K)
        {

            Y_XML_SECTION(xml,CallSign);
            OutputFile::Overwrite("score.dat");

            //------------------------------------------------------------------
            //
            //
            // initialize errors and cycle
            //
            //
            //------------------------------------------------------------------
            crs.reset();
            Cerr.ld(zero);
            size_t cycle = 0;
        PROBE:
            ++cycle;

            //------------------------------------------------------------------
            //
            //
            //
            // probing all equilibria, with crucial states detection
            //
            //
            //
            //------------------------------------------------------------------
            for(size_t probe=1;;++probe)
            {
                Y_XML_COMMENT(xml, "Probing cycle #" << cycle << "." << probe);

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
                    Y_XML_COMMENT(xml, "All blocked !!");
                    return; // no error as well
                }

                //--------------------------------------------------------------
                //
                // crucial prospects ?
                //
                //--------------------------------------------------------------
                assert(my.size>0);
                if(crucial)
                {
                    Y_XML_COMMENT(xml, "Removing crucial states");

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

                //--------------------------------------------------------------
                //
                // no crucial and at least one running: break
                //
                //--------------------------------------------------------------
                assert(my.size>0);
                break;
            }
            my.update();

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
            Y_XML_COMMENT(xml, "#Running = " << my.size << " / " << mix->size);
            const xReal f0 = objectiveFunction( mix.transfer(Cini,SubLevel,C,L), SubLevel );
            Y_XMLOG(xml, "f0 = " << f0.str() );


            {
                AppendFile fp("score.dat");
                const double ans = double(f0);
                if(ans>0.0)
                    fp("%g %.15g\n", double(cycle), log10(ans));
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
            Y_XML_COMMENT(xml, "Optimize Prospects");

            //------------------------------------------------------------------
            //
            //
            // study costs
            //
            //
            //------------------------------------------------------------------
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
                    Y_XMLOG(xml, "(+) " << Justify(pro.eq.name,my.maxKeySize,Justify::Right) << " => " <<  f_opt.str() );
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
                    Y_XMLOG(xml, "(0) " << Justify(pro.eq.name,my.maxKeySize,Justify::Right));
                    mix.transfer(pro.C,pro.L,C,L);
                    pro.xi = pro.ax = zero;
                    pro.dc.ld(zero);
                    pro.ff = f0;
                }
            }

            //------------------------------------------------------------------
            //
            //
            // sort by increasing costs
            //
            //------------------------------------------------------------------
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
            Y_XML_COMMENT(xml, "Searching better score #" << cycle);
            const Prospect &pro = **my.head;
            xReal           f1  = pro.ff;      assert(f1<=f0);
            Y_XMLOG(xml, "@" << OptimalProspect << " " << toReal(f1) << " / " << toReal(f0) << " @" << pro.key());

#if 0
            {
                Cend.ld(pro.C);
                save("pro.dat",100);
            }
#endif

            //------------------------------------------------------------------
            //
            //
            // early returns detectetion
            //
            //
            //------------------------------------------------------------------
            bool decreased = false;

            if(f1<=0.0)
            {
                Y_XML_COMMENT(xml, "Zero '" << pro.eq.name << "'!");
                mix.transfer(C,L,pro.C,pro.L);
                return; // no error
            }


            if(1==my.size)
            {
                Y_XML_COMMENT(xml, "Lone '" << pro.eq.name << "'!");
                mix.transfer(C,L,pro.C,pro.L);
                goto COMPUTE_ERROR;
            }

            //------------------------------------------------------------------
            //
            //
            // check if prospect decreased f0
            //
            //
            //------------------------------------------------------------------
            assert(f1>0.0);
            if(f1<f0)
            {
                decreased = true;
                mix.transfer(C,L,pro.C,pro.L);
                Y_XML_COMMENT(xml, "Keep " << OptimalProspect);
            }
            else
            {
                assert(f1>=f0);
                Y_XML_COMMENT(xml, "Drop " << OptimalProspect);
            }

            
            //------------------------------------------------------------------
            //
            //
            // check if DynamicalSystem decreased current state
            //
            //
            //------------------------------------------------------------------
            {
                const xReal ftmp = computeStepDS(xml,f0);
                Y_XMLOG(xml, "@" << DynamicalSystem << " " << toReal(ftmp) << " / " << toReal(f1) );
                if(ftmp<f1)
                {
                    decreased = true;
                    f1=ftmp;
                    mix.transfer(C,L,Ctmp,SubLevel);
                    Y_XML_COMMENT(xml, "Keep " << DynamicalSystem);
                    if(f1<=0.0)
                    {
                        Y_XML_COMMENT(xml,"Zero " << DynamicalSystem << "!");
                        return;
                    }
                }
                else
                {
                    Y_XML_COMMENT(xml, "Drop " << DynamicalSystem);
                }
            }
            assert(f1>0.0);


            //------------------------------------------------------------------
            //
            //
            // check if AlgebraicSystem decreased current state
            //
            //
            //------------------------------------------------------------------
            {
                const xReal ftmp = computeStepAS(xml,f0);
                Y_XMLOG(xml, "@" << AlgebraicSystem << " " << toReal(ftmp) << " / " << toReal(f1) );
                if(ftmp<f1)
                {
                    decreased = true;
                    f1=ftmp;
                    mix.transfer(C,L,Ctmp,SubLevel);
                    Y_XML_COMMENT(xml, "Keep " << AlgebraicSystem);
                    if(f1<=0.0)
                    {
                        Y_XML_COMMENT(xml,"Zero " << AlgebraicSystem << "!");
                        return;
                    }
                }
                else
                {
                    Y_XML_COMMENT(xml, "Drop " << AlgebraicSystem << "!");
                }
            }

            assert(f1>0.0);




            if(decreased)
            {
                Y_XML_COMMENT(xml, "Decreased Objective Function");
                goto PROBE;
            }


            //------------------------------------------------------------------
            //
            //
            //
            // C is computed to the minimal numeric value
            //
            //
            //
            //------------------------------------------------------------------
        COMPUTE_ERROR:
            Y_XML_COMMENT(xml,"Computing Residual Concentrations");
            for(const ProNode *pn=my.head;pn;pn=pn->next)
            {
                const Prospect &pro   = **pn;
                const xReal     score = pro.score(xmul,C,L);
                if(xml.verbose)
                    mix->print( xml(), pro.eq) << " $ " << real_t(score) << std::endl;

                //--------------------------------------------------------------
                // computing denominator
                //--------------------------------------------------------------
                xadd.free();
                for(Components::ConstIterator it=pro.eq->begin();it!=pro.eq->end();++it)
                {
                    const Component &cm     = *it;
                    const Actor     &a      = cm.actor;
                    const xReal      factor = a.xn * a.xn / a.sp(C,L);
                    xadd << factor;
                }
                const xReal denom = xadd.sum();

                //--------------------------------------------------------------
                // computing first order extent
                //--------------------------------------------------------------
                const xReal xiErr = score/denom;

                //--------------------------------------------------------------
                // computing delta for each species
                //--------------------------------------------------------------
                for(Components::ConstIterator it=pro.eq->begin();it!=pro.eq->end();++it)
                {
                    const Component &cm     = *it;
                    const Actor     &a      = cm.actor;
                    const xReal      dc     = (a.xn*xiErr).abs(); if(dc<=0.0) continue;
                    const Species   &sp     = a.sp;
                    InSituMax(sp(Cerr,SubLevel),dc);
                }
            }

            Y_XML_COMMENT(xml,"Computing Fractionnal Errors");
            for(const SNode *sn=mix->species.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                const xReal    dc = sp(Cerr,SubLevel);
                const xReal    cc = sp(C,L);
                if(xml.verbose)
                    mix->sformat.print(xml() << "d_|", sp, Justify::Left)
                    << "| = " << std::setw(15) << real_t(dc)
                    << "/"    << std::setw(15) << real_t(cc)
                    << std::endl;
                if(cc>0.0 && dc>0.0) {
                    const xReal err = dc/cc;
                    crs(sp,err);
                }
            }

            if(crs->size<=0) throw Specific::Exception(CallSign, "inconsistent errors!");
            Y_XMLOG(xml, "maxError: "  << *(crs->tail) );
            
            throw Exception("not decreased, not implemented");
        }
    }

}


