
#include "y/chemical/plexus/solver.hpp"
#include "y/sort/heap.hpp"
#include "y/mkl/tao/seq/level1.hpp"
#include "y/type/temporary.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        Solver:: ~Solver() noexcept
        {
        }

        Solver:: Solver(const Clusters &cls) :
        Ceq(cls.maxEPC,cls.maxSPC),
        dCe(cls.maxEPC,cls.maxSPC),
        Phi(cls.maxEPC,cls.maxSPC),
        Cin(cls.maxSPC,0),
        Cex(cls.maxSPC,0),
        Cws(cls.maxSPC,0),
        afm(),
        bnk(),
        pps(cls.maxEPC),
        lis(),
        sql(),
        sqp(),
        sdb(),
        obj(cls.maxEPC,0),
        pli(0)
        {
            // prepare LinearlyIndependent Set
            for(const Cluster *cl=cls->head;cl;cl=cl->next)
                lis(cl->size,cl->species.size,bnk);

        }
        

        void Solver:: process(XWritable       &C,
                              const Cluster   &cl,
                              const XReadable &K,
                              XMLog           &xml)
        {
            Y_XML_SECTION_OPT(xml, "Solver::Cluster", " eqs='" << cl.size << "'");

            {
                Y_XML_SECTION(xml, "Probe");
                //--------------------------------------------------------------
                //
                // Initialize prospects
                //
                //--------------------------------------------------------------
                pps.free();
                sdb.free();

                //--------------------------------------------------------------
                //
                // Store all prospects matching active equilibria
                //
                //--------------------------------------------------------------
                for(const ENode *en=cl.head;en;en=en->next)
                {
                    const Equilibrium    &eq   = **en;
                    const size_t * const  in   = eq.indx;
                    const xreal_t         eK   = K[in[TopLevel]];
                    const size_t          isub = in[SubLevel];
                    XWritable            &Ci   = Ceq[isub];
                    XWritable            &Di   = dCe[isub];
                    XWritable            &phi  = Phi[isub];
                    if( afm.solve(Ci, SubLevel, C, TopLevel, eq, eK) )
                    {
                        const xreal_t  xi = afm.eval(Di,Ci,SubLevel,C,TopLevel,eq);
                        const Prospect pro(eq,eK,xi,Ci,Di,phi);
                        pps << pro;
                    }
                }

                if(pps.size()<=0)
                {
                    Y_XMLOG(xml, "[[no active equilibrium]]");
                    return;
                }

                //--------------------------------------------------------------
                //
                // order them in decreasing |xi|
                //
                //--------------------------------------------------------------
                HeapSort::Call(pps, Prospect::Compare);
                if(xml.verbose)
                {
                    for(size_t i=1;i<=pps.size();++i)
                    {
                        const Prospect &pro = pps[i];
                        cl.uuid.pad(xml() << pro.eq.name,pro.eq) << " @" << std::setw(15) << real_t(pro.xi);
                        pro.eq.displayCompact(std::cerr, pro.cc, SubLevel) << std::endl;
                    }
                }
            }

            LinearlyIndependent &li = lis[cl.species.size];
            const Temporary<LinearlyIndependent *> tempLI(pli,&li);
            {
                Y_XML_SECTION(xml, "Basis");

                //--------------------------------------------------------------
                //
                // find basis
                //
                //--------------------------------------------------------------
                const size_t         np = pps.size(); // number of prospect(s)
                const size_t         nm = cl.Nu.rows; // max base size
                li.init();
                for(size_t i=1;i<=np;++i)
                {
                    Prospect &pro = pps[i];
                    if(li.keep(pro,cl.topology) && li->size >= nm) break;
                }

                Y_XMLOG(xml, "#found = " << li->size << " / " << nm << " / " << np);
                //--------------------------------------------------------------
                //
                // update prospect(s) in basis
                //
                //--------------------------------------------------------------
                for(PNode *pn=li->head;pn;pn=pn->next)
                {
                    Prospect &pro = **pn;
                    pro.update(afm.xadd, afm.xmul);
                    if(xml.verbose)
                    {
                        cl.uuid.pad(xml() << pro.eq.name,pro.eq);
                        *xml <<        " @" << std::setw(15) << real_t(pro.xi);
                        *xml << " : slope=" << std::setw(15) << real_t(pro.sl);
                        pro.eq.displayCompact(std::cerr << " @", pro.cc, SubLevel) << std::endl;
                    }
                    pro.eq.record(sdb);
                }
                sdb.display<Species>(std::cerr << "species=") << std::endl;

            }

            {
                const Prospect &pro = ** li->head;
                Cin.ld(0); cl.transfer(Cin,SubLevel,C,TopLevel);
                Cex.ld(0); cl.transfer(Cex,SubLevel,pro.cc,SubLevel);

                if(2==li->size)
                {
                    OutputFile  fp("phase.dat");
                    OutputFile  fp2("objective.dat");

                    const Prospect &pro1 = **li->head;
                    const Prospect &pro2 = **li->tail;
                    assert( &pro1 != &pro2 );
                    
                    pro1.eq.displayCompact(std::cerr << "@" << pro1.eq, pro1.cc, SubLevel) << std::endl;
                    pro2.eq.displayCompact(std::cerr << "@" << pro2.eq, pro2.cc, SubLevel) << std::endl;


                    Cin.ld(0); cl.transfer(Cin,SubLevel,pro1.cc,SubLevel);
                    Cex.ld(0); cl.transfer(Cex,SubLevel,pro2.cc,SubLevel);
                    std::cerr << "Cin=" << Cin << std::endl;
                    std::cerr << "Cex=" << Cex << std::endl;

                    std::cerr << pro1.eq << "_in=" << real_t(pro1.eq.massAction(pro1.eK, afm.xmul, Cin, SubLevel)) << std::endl;
                    std::cerr << pro1.eq << "_ex=" << real_t(pro1.eq.massAction(pro1.eK, afm.xmul, Cex, SubLevel)) << std::endl;

                    std::cerr << pro2.eq << "_in=" << real_t(pro2.eq.massAction(pro2.eK, afm.xmul, Cin, SubLevel)) << std::endl;
                    std::cerr << pro2.eq << "_ex=" << real_t(pro2.eq.massAction(pro2.eK, afm.xmul, Cex, SubLevel)) << std::endl;


                    for(size_t j=0;j<=100;++j)
                    {
                        const real_t  u = real_t(j)/100;
                        const xreal_t Y = ObjectiveFunction(u);
                        fp("%g",u);
                        for(size_t i=1;i<=li->size;++i)
                        {
                            fp(" %.15g", real_t(obj[i]) );
                        }
                        fp << "\n";
                        fp2("%g %.15g\n", u, real_t(Y));
                    }
                }

            }




        }


        xreal_t Solver:: ObjectiveFunction(const xreal_t u)
        {
            assert(0!=pli);
            const xreal_t zero;
            const xreal_t one(1);

            XAdd &xadd = afm.xadd;
            XMul &xmul = afm.xmul;
            xadd.free();

            // construct trial concentration from recorded species
            Cws.ld(zero);
            for(AddressBook::Iterator it=sdb.begin();it!=sdb.end();++it)
            {
                const Species & s     = *static_cast<const Species *>(*it);
                const size_t    i     = s.indx[SubLevel];
                const xreal_t   c0    = Cin[i];
                const xreal_t   c1    = Cex[i];
                xreal_t         lower = c0;
                xreal_t         upper = c1;
                if(upper<lower) Swap(lower,upper);
                assert(lower<=upper);
                const xreal_t v = one-u;
                Cws[i] = Clamp(lower,v*c0+u*c1,upper);
            }

            // compute all objective functions
            const LinearlyIndependent &li = *pli;
            obj.ld(zero);
            size_t j=0;
            for(const PNode *pn=li->head;pn;pn=pn->next)
            {
                const Prospect &pro = **pn;
                const xreal_t   val = pro.ObjectiveFunction(Cws,xmul);
                obj[++j] = val;
            }
            assert(li->size == j);
            
            {
                const LightArray<xreal_t> sub(&obj[1],j);
                return xadd.normOf(obj);
            }

        }


        void Solver:: sqDrop(Squad *squad) noexcept
        {
            assert(0!=squad);
            sqp.store(squad)->free();
        }


        void Solver:: sqFree() noexcept
        {
            while(sql.size>0)
                sqDrop(sql.popTail());

        }

        Squad * Solver:: sqMake(Prospect &first)
        {
            Squad *squad = (sqp.size <= 0) ? new Squad(bnk) : sqp.query();
            try        { (*squad) << first;    }
            catch(...) { sqDrop(squad); throw; }
            return squad;
        }


        void Solver:: sqFrom(const PList        &plist,
                             const Matrix<bool> &attached,
                             XMLog              &xml)
        {
            Y_XML_SECTION(xml, "Squad");

            //------------------------------------------------------------------
            //
            // initialize squads
            //
            //------------------------------------------------------------------
            sqFree();
            for(PNode *pn=plist.head;pn;pn=pn->next)
            {
                Prospect &pro = **pn;
                //--------------------------------------------------------------
                //
                // look for accepting squad
                //
                //--------------------------------------------------------------
                for(Squad *squad=sql.head;squad;squad=squad->next)
                {
                    if(squad->accept(pro,attached))
                    {
                        (*squad) << pro;
                        goto CHECK_FUSION;
                    }
                }
                //--------------------------------------------------------------
                //
                // create a new squad
                //
                //--------------------------------------------------------------
                sql.pushTail( sqMake(pro) );
                continue;

                //--------------------------------------------------------------
                //
                // check fusion
                //
                //--------------------------------------------------------------
            CHECK_FUSION:
                std::cerr << "TODO! " << __FILE__ << ":" << __LINE__ << std::endl;
                ;
            }

            for(const Squad *squad=sql.head;squad;squad=squad->next)
            {
                Y_XMLOG(xml, "squad@" << *squad);
            }

        }

    }


}
