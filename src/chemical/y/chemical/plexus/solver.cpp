#include "y/chemical/plexus/solver.hpp"
#include "y/type/temporary.hpp"
#include "y/sort/heap.hpp"
#include "y/system/exception.hpp"

#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Solver:: ~Solver() noexcept {}

        Solver:: Solver(const Clusters &cls) :
        afm(),
        ceq(cls.maxEPC,cls.maxSPC),
        phi(cls.maxSPC),
        obj(cls.maxEPC),
        pps(cls.maxEPC),
        Cin(cls.maxSPC,0),
        Cex(cls.maxSPC,0),
        Cws(cls.maxSPC,0),
        bnk(),
        qdb(),
        sim(cls.maxEPC,cls.maxSPC),
        ppb(0),
        pcl(0)
        {
            for(const Cluster *cl=cls->head;cl;cl=cl->next)
            {
                qdb(cl->Nu.rows,cl->species.size,bnk);
            }
        }

        xreal_t Solver:: objectiveFunction(const XReadable &C,
                                           const Level      L)
        {
            assert(0!=ppb);
            obj.free();
            const PList &list = **ppb;
            {
                for(const PNode *pn=list.head;pn;pn=pn->next)
                {
                    obj << (**pn).objectiveFunction(C,L,afm.xmul);
                }
            }
            assert(obj.size() == list.size);
            return afm.xadd.normOf(obj);
        }


        xreal_t Solver::  operator()(const xreal_t u)
        {
            assert(0!=pcl);
            const xreal_t one = 1;
            const xreal_t v   = one - u;

            for(size_t j=pcl->species.size;j>0;--j)
            {
                const xreal_t c0 = Cin[j];
                const xreal_t c1 = Cex[j];
                xreal_t cmin  = c0;
                xreal_t cmax  = c1;
                if(cmax<cmin) Swap(cmin,cmax);
                Cws[j] = Clamp(cmin,v*c0+u*c1,cmax);
            }

            return objectiveFunction(Cws,SubLevel);
        }


        size_t Solver:: examine(const Cluster   & cl,
                                XWritable       & Ctop,
                                const XReadable & Ktop,
                                bool            & repl,
                                XMLog           & xml)
        {
            Y_XML_SECTION(xml, "Examine");
            const xreal_t zero;
            const xreal_t mOne(-1);
            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            size_t        iter = 0;
            repl               = false;
        EXAMINE:
            ++iter;
            Y_XMLOG(xml, "(*) [Examine] (#iter=" << iter << ")" );
            pps.free();

            //------------------------------------------------------------------
            //
            // Loop over each equilibrium
            //
            //------------------------------------------------------------------
            for(const ENode *en=cl.head;en;en=en->next)
            {
                //----------------------------------------------------------
                //
                // get equilibrium and its aftermath
                //
                //----------------------------------------------------------
                const Equilibrium &eq = **en;                      // equilibrium
                const xreal_t      eK = Ktop[ eq.indx[TopLevel] ]; // current constant
                const size_t       ii = pps.size() + 1;            // destination indeq
                XWritable         &cc = ceq[ii];                   // local solution filled and computed
                const Situation    id = afm.seek( cl.transfer(cc,SubLevel,Ctop,TopLevel), SubLevel, eq, eK);
                switch(id)
                {
                    case Blocked: Y_XMLOG(xml, "(-) [Blocked] " << eq); continue;
                    case Running: Y_XMLOG(xml, "(+) [Running] " << eq); break;
                    case Crucial:
                        Y_XMLOG(xml, "(!) [Crucial] " << eq << " @" << cc);
                        repl = true;
                        cl.transfer(Ctop, TopLevel, cc, SubLevel); // set crucial
                        goto EXAMINE;                                 // abort and restart
                }
                assert(Running==id);

                //----------------------------------------------------------
                //
                // store new running prospect with final computation
                //
                //----------------------------------------------------------
                eq.drvsMassAction(eK, phi, SubLevel, cc, SubLevel, afm.xmul);
                const xreal_t  slope = eq.dot(phi, SubLevel, afm.xadd);
                if(slope>=zero) 
                {
                    eq.displayCompact(std::cerr << "\tcc  = ", cc, SubLevel) << " / " << cc <<  std::endl;
                    eq.displayCompact(std::cerr << "\tphi = ", phi, SubLevel) << " / " << phi <<  std::endl;
                    eq.displayCompact(std::cerr << "\tnu  = ", cl.topology[eq.indx[SubLevel]], SubLevel) << std::endl;
                    throw Specific::Exception(eq.name.c_str(),"invalid optimal slope");
                }
                const Prospect pro(eq, eK, cc, afm.eval(cc,SubLevel,Ctop,TopLevel,eq), mOne/slope );

                pps << pro;
            }

            HeapSort::Call(pps, Prospect::Compare);
            return pps.size();
        }

        void Solver:: run(const Cluster   &cl,
                          XWritable       &Ctop,
                          const XReadable &Ktop,
                          XMLog           &xml)
        {

            Y_XML_SECTION_OPT(xml, "Solver::Cluster", " size='" << cl.size << "'");
            const size_t ndof = cl.Nu.rows;                      // degrees of freedom
            bool         repl = false;                           // replacement flag
            size_t       npmx = examine(cl,Ctop,Ktop,repl,xml);  // max prospects

            if( npmx <= 0) { Y_XMLOG(xml, "[Jammed!]"); return; }

            if(xml.verbose)
            {
                for(size_t i=1;i<=npmx;++i)
                {
                    const Prospect &pro = pps[i];
                    cl.uuid.pad(xml() << pro.eq,pro.eq);
                    *xml << " | ax =" << std::setw(15) << real_t(pro.xi.abs());
                    pro.eq.displayCompact(*xml << " @", pro.cc, SubLevel);
                    *xml << " ks=" << real_t(pro.ks);
                    *xml << std::endl;
                }
            }

            QBuilder &                 base = qdb[cl.species.size];
            const Temporary<QBuilder*> ppbT(ppb,&base);
            const Temporary<Cluster *> pclT(pcl,&Coerce(cl));

            {
                Y_XML_SECTION(xml, "SubBasis");
                base.init();
                for(size_t i=1;i<=npmx;++i)
                {
                    const Prospect &pro = pps[i];
                    if(base.grow(pro, cl.topology) && base->size >= ndof) break;
                }

                Y_XMLOG(xml, "#dof = " << std::setw(10) << ndof);
                Y_XMLOG(xml, "#pro = " << std::setw(10) << npmx);
                Y_XMLOG(xml, "#vec = " << std::setw(10) << base->size);
            }

#if 0
            const xreal_t zero;
            const xreal_t mOne(-1);
            const size_t  ndof = cl.Nu.rows; assert(ndof>0); // primary equilibria



            {
                //--------------------------------------------------------------
                //
                //
                Y_XML_SECTION(xml, "Scanning");
                //
                //
                //--------------------------------------------------------------
                bool replaced = false;
            SCAN:
                Y_XMLOG(xml, "(*) [Examine]");
                pps.free();
                for(const ENode *en=cl.head;en;en=en->next)
                {
                    //----------------------------------------------------------
                    //
                    // get equilibrium and its aftermath
                    //
                    //----------------------------------------------------------
                    const Equilibrium &eq = **en;
                    const xreal_t      eK = Ktop[ eq.indx[TopLevel] ];
                    const size_t       ii = pps.size() + 1;
                    XWritable         &cc = ceq[ii];
                    const Situation    id = afm.seek( cl.transfer(cc,SubLevel,Ctop,TopLevel), SubLevel, eq, eK);
                    switch(id)
                    {
                        case Blocked: Y_XMLOG(xml, "(-) [Blocked] " << eq); continue;
                        case Running: Y_XMLOG(xml, "(+) [Running] " << eq); break;
                        case Crucial:
                            Y_XMLOG(xml, "(!) [Crucial] " << eq << " @" << cc);
                            replaced = true;
                            cl.transfer(Ctop, TopLevel, cc, SubLevel); // set crucial
                            goto SCAN;                                 // abort and restart
                    }
                    assert(Running==id);

                    //----------------------------------------------------------
                    //
                    // store new running prospect
                    //
                    //----------------------------------------------------------
                    eq.drvsMassAction(eK, phi, SubLevel, cc, SubLevel, afm.xmul);
                    const xreal_t  slope = eq.dot(phi, SubLevel, afm.xadd);
                    if(slope>=zero) throw Specific::Exception(eq.name.c_str(),"invalid slope");
                    const Prospect pro(eq, eK, cc, afm.eval(cc,SubLevel,Ctop,TopLevel,eq), mOne/slope );
                    pps << pro;
                }

                if( pps.size() <= 0 ) {
                    Y_XMLOG(xml, "[Jammed!]");
                    return;
                }

                {
                    //----------------------------------------------------------
                    //
                    Y_XML_SECTION_OPT(xml, "Individual", " running='" << pps.size() << "' clusterSize='" << cl.size << "'" );
                    //
                    //----------------------------------------------------------
                    //HeapSort::Call(obj, Comparison::Decreasing<xreal_t>, pps);
                    HeapSort::Call(pps, Prospect::Compare);

                    if(xml.verbose)
                    {
                        for(size_t i=1;i<=pps.size();++i)
                        {
                            const Prospect &pro = pps[i];
                            cl.uuid.pad(xml() << pro.eq,pro.eq);
                            //*xml << " | ham=" << std::setw(15) << real_t(obj[i]);
                            *xml << " | ax =" << std::setw(15) << real_t(pro.xi.abs());
                            pro.eq.displayCompact(*xml << " @", pro.cc, SubLevel);
                            *xml << " ks=" << real_t(pro.ks);
                            *xml << std::endl;
                        }
                    }
                }
            }


            QBuilder &                 base = qdb[cl.species.size];
            const Temporary<QBuilder*> keepBase(ppb,&base);
            const Temporary<Cluster *> keepData(pcl,&Coerce(cl));
            {
                //--------------------------------------------------------------
                //
                //
                Y_XML_SECTION(xml, "Extracting");
                //
                //
                //--------------------------------------------------------------
                {
                    //----------------------------------------------------------
                    //
                    Y_XML_SECTION(xml, "SubFamily");
                    //
                    //----------------------------------------------------------
                    const size_t  npmx = pps.size();
                    base.init();
                    for(size_t i=1;i<=npmx;++i)
                    {
                        const Prospect &pro = pps[i];
                        if(base.grow(pro, cl.topology) && base->size >= ndof) break;
                    }

                    Y_XMLOG(xml, "#dof = " << std::setw(10) << ndof);
                    Y_XMLOG(xml, "#pro = " << std::setw(10) << npmx);
                    Y_XMLOG(xml, "#vec = " << std::setw(10) << base->size);
                }

                {
                    //----------------------------------------------------------
                    //
                    Y_XML_SECTION(xml, "PhaseSpace");
                    //
                    //----------------------------------------------------------
                    {
                        const xreal_t ham = objectiveFunction(Ctop, TopLevel);
                        Y_XMLOG(xml, "F(C0)=" << real_t(ham) );
                    }
                    for(const PNode *pn=base->head;pn;pn=pn->next)
                    {
                        const Prospect &pro = **pn;
                        const xreal_t   ham = objectiveFunction(pro.cc,SubLevel);
                        if(xml.verbose)
                        {
                            cl.uuid.pad(xml() << "F(" << pro.eq << ")",pro.eq) << " : " << std::setw(15) << real_t(ham);
                            *xml << std::endl;
                        }
                    }

                    if(base->size>=2)
                    {
                        const Prospect &lhs = **(base->head);
                        const Prospect &rhs = **(base->head->next);
                        Cin.ld(0); cl.transfer(Cin, SubLevel, lhs.cc, SubLevel);
                        Cex.ld(0); cl.transfer(Cex, SubLevel, rhs.cc, SubLevel);

                        OutputFile fp("objective.dat");
                        const size_t NP=1000;
                        for(size_t j=0;j<=NP;++j)
                        {
                            real_t u = real_t(j)/NP;
                            fp("%.15g %.15g\n", u, real_t( (*this)(u)) );
                        }
                    }


                    sim.free();
                }

            }
#endif
        }
    }
}
