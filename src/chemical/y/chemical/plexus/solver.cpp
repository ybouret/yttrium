
#include "y/chemical/plexus/solver.hpp"
#include "y/sort/heap.hpp"
#include "y/mkl/tao/seq/level1.hpp"
#include "y/type/temporary.hpp"
#include "y/stream/libc/output.hpp"
#include "y/data/list/ordered.hpp"
#include "y/mkl/opt/minimize.hpp"

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

        class Edge  : public Object
        {
        public:
            explicit Edge(XWritable &_C) noexcept : H(0), C(_C), next(0), prev(0) {}
            virtual ~Edge() noexcept {}
            xreal_t     H;
            XWritable  &C;
            Edge       *next;
            Edge       *prev;

            class Comparator
            {
            public:
                Comparator() {}
                ~Comparator() {}

                SignType operator()(const Edge *lhs, const Edge *rhs) const
                {
                    return Comparison::CxxDecreasing(lhs->H, rhs->H);
                }
            };

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Edge);
        };

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

                    // initialize Ci[SubLevel] with all species from C[TopLevel]
                    cl.transfer(Ci, SubLevel, C, TopLevel);

                    // solve
                    if( afm.solve(Ci, SubLevel, eq, eK) )
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

                Y_XMLOG(xml, "#found base = " << li->size << ", dims = " << nm << ", npro = " << np << ", size=" << cl.size);
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
                XMatrix           Csm(cl.Nu.rows+1,cl.species.size);
                OrderedList<Edge,Edge::Comparator,OrderedListQueryHead> edges;

                size_t j = 0;
                for(const PNode *pn=li->head;pn;pn=pn->next)
                {
                    const Prospect &pro = **pn;
                    XWritable      &csm = Csm[++j];cl.transfer(csm, SubLevel, pro.cc, SubLevel);
                    const xreal_t   ham = ObjectiveFunction(csm);
                    if(xml.verbose)
                    {
                        cl.uuid.pad(xml() << pro.eq.name,pro.eq) << " => " << real_t(ham) << "@";
                        *xml << pro.cc << std::endl;
                    }
                    Edge *edge = new Edge(csm);
                    edge->H = ham;
                    edges.store( edge );
                }

                {
                    XWritable &csm = Csm[++j]; cl.transfer(csm, SubLevel, C,TopLevel);
                    const xreal_t   ham = ObjectiveFunction(csm);
                    Y_XMLOG(xml, "ham0=" << real_t(ham));
                    Edge *edge = new Edge(csm);
                    edge->H = ham;
                    edges.store( edge );
                }


                if(xml.verbose)
                {
                    for(const Edge *edge=edges.head;edge;edge=edge->next)
                    {
                        xml() << std::setw(15) << real_t(edge->H) << "@" << edge->C << std::endl;
                    }
                }

                while( edges.size > 1 )
                {
                    AutoPtr<Edge> upper = edges.query();
                    AutoPtr<Edge> lower = edges.query();
                    std::cerr << "Look between " << real_t(upper->H) << " and " << real_t( lower->H ) << std::endl;
                    cl.transfer(Cin, SubLevel, upper->C, SubLevel);
                    cl.transfer(Cex, SubLevel, lower->C, SubLevel);
                    MKL::Triplet<xreal_t> xx = { 0, -1, 1};
                    MKL::Triplet<xreal_t> ff = { upper->H, -1, lower->H };
                    const xreal_t uu = MKL::Minimize<xreal_t>::Locate(MKL::Minimizing::Inside,  *this, xx, ff);
                    std::cerr << "uu=" << uu << "=>" << real_t(ff.b) << std::endl;

                    {
                        OutputFile  fp2("objective.dat");
                        for(size_t j=0;j<=1000;++j)
                        {
                            const real_t  u = real_t(j)/1000;
                            const xreal_t Y = ObjectiveFunction(u);
                            fp2("%g %.15g\n", u, real_t(Y));
                        }
                    }


                    lower->H = (*this)(uu);
                    cl.transfer(lower->C, SubLevel, Cws, SubLevel);
                    edges.store( lower.yield() );


                }





                if(2==li->size)
                {

                    const Prospect &lhs = ** li->head;
                    const Prospect &rhs = ** li->tail;
                    Cin.ld(0); cl.transfer(Cin,SubLevel,lhs.cc,SubLevel);
                    Cex.ld(0); cl.transfer(Cex,SubLevel,rhs.cc,SubLevel);

                    std::cerr << "look between " << lhs.eq << " and " << rhs.eq << std::endl;

                    {
                        OutputFile  fp("phase.dat");
                        OutputFile  fp2("objective.dat");
                        for(size_t j=0;j<=1000;++j)
                        {
                            const real_t  u = real_t(j)/1000;
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

            if(false)
            {
                const Prospect &pro = ** li->head;
                Cin.ld(0); cl.transfer(Cin,SubLevel,C,TopLevel);
                Cex.ld(0); cl.transfer(Cex,SubLevel,pro.cc,SubLevel);

                std::cerr << "Cin=" << Cin << std::endl;
                std::cerr << "Cex=" << Cex << std::endl;

                {
                    OutputFile  fp("phase.dat");
                    OutputFile  fp2("objective.dat");
                    for(size_t j=0;j<=1000;++j)
                    {
                        const real_t  u = real_t(j)/1000;
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


        xreal_t Solver:: ObjectiveFunction(const XReadable &C)
        {
            assert(0!=pli);
            const xreal_t zero;
            XAdd &        xadd = afm.xadd;
            XMul &        xmul = afm.xmul;

            // compute all objective functions
            const LinearlyIndependent &li = *pli;
            obj.ld(zero);
            size_t j=0;
            for(const PNode *pn=li->head;pn;pn=pn->next)
            {
                const Prospect &pro = **pn;
                const xreal_t   val = pro.ObjectiveFunction(C,xmul);
                obj[++j] = val;
            }
            assert(li->size == j);

            {
                const LightArray<xreal_t> sub(&obj[1],j);
                return xadd.normOf(obj);
            }

#if 0
            {
                xadd.free();
                for(;j>0;--j)
                    xadd << obj[j].abs();
                return xadd.sum();
            }
#endif

        }

        xreal_t Solver:: ObjectiveFunction(const xreal_t u)
        {
            assert(0!=pli);
            const xreal_t one(1);
            const xreal_t zero;

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

            // evaluate
            return ObjectiveFunction(Cws);

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
