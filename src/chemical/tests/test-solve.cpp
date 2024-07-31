
#include "y/chemical/plexus/clusters.hpp"
#include "y/chemical/weasel/compiler.hpp"
#include "y/random/park-miller.hpp"
#include "y/utest/run.hpp"

#include "y/container/cxx/series.hpp"
#include "y/chemical/reactive/aftermath.hpp"
#include "y/sort/heap.hpp"
#include "y/system/exception.hpp"
#include "y/orthogonal/family.hpp"
#include "y/data/list/ordered.hpp"

#include "y/mkl/opt/minimize.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        using namespace MKL;

        class Vertex : public Object, public XArray
        {
        public:
            typedef CxxPoolOf<Vertex> Pool;
            struct Comparator
            {
                inline SignType operator()(const Vertex * const lhs, const Vertex * const rhs) const noexcept
                {
                    return Comparison::CxxDecreasing(lhs->cost, rhs->cost);
                }
            };


            explicit Vertex(const size_t species) :
            XArray(species), cost(0), next(0), prev(0)
            {

            }

            virtual ~Vertex() noexcept {}

            void clear() noexcept { ld(cost=0); }

            xreal_t cost;
            Vertex *next;
            Vertex *prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Vertex);
        };

        typedef OrderedList<Vertex,Vertex::Comparator,OrderedListQueryHead> Vertices;

        class Simplex : public Vertices
        {
        public:
            explicit Simplex(size_t       maxEqs,
                             const size_t maxSpecies) :
            Vertices(),
            pool(),
            dims(maxSpecies)
            {
                ++maxEqs;
                while(maxEqs-- > 0) pool.store( new Vertex(dims) );
            }

            void free( Vertex * const vtx) noexcept
            {
                assert(0!=vtx);
                pool.store( vtx )->clear();
            }

            void free() noexcept {
                while(size>0) free( popTail() );
            }

            const Vertex & load(const xreal_t    cost,
                                const SList     &spec,
                                const XReadable &C,
                                const Level      L)
            {
                AutoPtr<Vertex> ptr = pool.size ? pool.query() : new Vertex(dims);
                Vertex         &vtx = *ptr; assert(Memory::OutOfReach::Are0(&vtx[1],vtx.size()*sizeof(xreal_t)));
                vtx.cost = cost;
                for(const SNode *sn=spec.head;sn;sn=sn->next)
                {
                    const size_t * const indx = (**sn).indx;
                    vtx[ indx[SubLevel] ] = C[ indx[L] ];
                }
                this->store( ptr.yield() );
                return vtx;
            }


            Vertex::Pool pool;
            const size_t dims;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Simplex);
        };

        class Applicant
        {
        public:
            typedef CxxSeries<Applicant> Series;

            Applicant(const Equilibrium &  _eq,
                      const xreal_t        _eK,
                      const XReadable    & _cc,
                      const xreal_t        _xi) noexcept :
            eq(_eq),
            eK(_eK),
            cc(_cc),
            xi(_xi),
            ax( xi.abs() )
            {
            }

            Applicant(const Applicant &_) noexcept :
            eq(_.eq),
            eK(_.eK),
            cc(_.cc),
            xi(_.xi),
            ax(_.ax)
            {
            }

            size_t sub() const noexcept { return eq.indx[SubLevel]; }

            real_t affinity(XMul            &X,
                            const XReadable &C,
                            const Level      L) const
            {
                return eq.affinity(eK, X, C, L);
            }


            static int Compare(const Applicant &lhs, const Applicant &rhs) noexcept
            {
                return Comparison::Increasing<xreal_t>(lhs.ax,rhs.ax);
            }


            std::ostream & display(std::ostream   &os,
                                   const Assembly &uuid,
                                   const bool      full) const
            {
                uuid.pad(os<< eq, eq);
                os << " | ax =" << std::setw(15) << real_t(ax);
                if(full)
                {
                    eq.displayCompact(os << " @",cc, SubLevel);
                }
                return os;
            }

            const Equilibrium &eq;
            const xreal_t      eK;
            const XReadable   &cc;
            const xreal_t      xi;
            const xreal_t      ax; //!< |xi|

        private:
            Y_DISABLE_ASSIGN(Applicant);
        };

        typedef Small::CoopLightList<const Applicant> AList;
        typedef AList::NodeType                       ANode;
        typedef AList::ProxyType                      ABank;


        class Normalizer : public Quantized, public Counted
        {
        public:
            explicit Normalizer(const Cluster &cl) :
            afm(),
            ceq(cl.size,cl.species.size),
            aps(cl.size),
            obj(cl.size),
            Cin(cl.species.size,0),
            Cex(cl.species.size,0),
            Cws(cl.species.size,0),
            dof(cl.Nu.rows),
            bnk(),
            apl(bnk),
            qfm(cl.species.size,cl.size),
            sim(cl.size,cl.species.size)
            {
            }

            virtual ~Normalizer() noexcept
            {
            }



            void run(const Cluster   & cl,
                     XWritable       & Ctop,
                     const XReadable &Ktop,
                     XMLog           & xml)
            {
                assert(cl.size<=ceq.rows);
                assert(cl.species.size==ceq.cols);
                Y_XML_SECTION_OPT(xml, "Normalizer ", " size='" << cl.size << "' species='" << cl.species.size << "'");
                bool          repl = false;
                const size_t  nmax = compile(cl, Ctop, Ktop, repl, xml); if(nmax<=0) { Y_XMLOG(xml, "[Jammed!]"); return; }
                const xreal_t cost = overall(cl, Ctop, repl, xml);

                Y_XMLOG(xml, "found " << real_t(cost));
            }

            xreal_t objectiveFunction(const XReadable &C, const Level L)
            {
                obj.free();
                for(const ANode *an=apl.head;an;an=an->next)
                {
                    obj << (**an).affinity(afm.xmul, C, L);
                }
                return afm.xadd.normOf(obj);
            }

            xreal_t  operator()(const xreal_t u)
            {
                const xreal_t one = 1;
                const xreal_t v   = one - u;

                for(size_t j=Cin.size();j>0;--j)
                {
                    const xreal_t c0   = Cin[j];
                    const xreal_t c1   = Cex[j];
                    xreal_t       cmin = c0;
                    xreal_t       cmax = c1;
                    if(cmax<cmin) Swap(cmin,cmax);
                    Cws[j] = Clamp(cmin,v*c0+u*c1,cmax);
                }
                return objectiveFunction(Cws,SubLevel);
            }



            Aftermath          afm;
            XMatrix            ceq;
            Applicant::Series  aps;
            CxxSeries<xreal_t> obj;
            CxxArray<xreal_t>  Cin;
            CxxArray<xreal_t>  Cex;
            CxxArray<xreal_t>  Cws;
            const size_t       dof; //!< primary eqs
            ABank              bnk; //!< pool of applicants
            AList              apl; //!< applicant list
            Orthogonal::Family qfm; //!< orthogonal family
            Simplex            sim; //!< simplex

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Normalizer);

            //! after a successful compilation
            xreal_t overall(const Cluster   & cl,
                            XWritable       & Ctop,
                            const bool        repl,
                            XMLog           & xml)
            {
                assert(aps.size()>0);
                const size_t dims = extract(cl,xml);
                switch(dims)
                {
                    case 0:  // shouldn't happen
                        throw Specific::Exception("Normalizer", "no equilbrium");

                    case 1: // use single winner
                        return objectiveFunction(cl.transfer(Ctop,TopLevel,(**apl.head).cc,SubLevel),TopLevel);

                    default: // will improve with simplex
                        break;
                }

                return improve(cl,Ctop,repl,xml);
            }

            xreal_t  improve(const Cluster   & cl,
                             XWritable       & Ctop,
                             const bool        repl,
                             XMLog           & xml)
            {
                Y_XML_SECTION(xml, "Improve");
                assert(apl.size>0);

                {
                    Y_XML_SECTION(xml, "Simplex");
                    // load simplex
                    sim.free();
                    if(!repl)
                    {
                        const xreal_t val = sim.load( objectiveFunction(Ctop, TopLevel), cl.species, Ctop, TopLevel).cost;
                        Y_XMLOG(xml, std::setw(15) << real_t(val) << " @Ctop");
                    }
                    else
                    {
                        Y_XMLOG(xml, "[replaced Ctop]");
                    }

                    for(const ANode *an=apl.head;an;an=an->next)
                    {
                        const Applicant &app = **an;
                        const xreal_t    val = sim.load( objectiveFunction(app.cc, SubLevel), cl.species, app.cc, SubLevel).cost;
                        Y_XMLOG(xml,std::setw(15) << real_t(val) << " @" << app.eq);
                    }

                }

                {
                    Y_XML_SECTION(xml, "Search");
                    unsigned long cycle = 0;
                    const size_t  m     = Cin.size();
                    OutputFile fp("objective.dat");

                    while(sim.size>1)
                    {
                        ++cycle;
                        Y_XMLOG(xml, "#cycle = " << cycle);
                        Triplet<xreal_t> xx = {  0, -1,  1 };
                        Triplet<xreal_t> ff = { -1, -1, -1 };

                        // extract upper/lower and load Cin/Cex
                        {
                            AutoPtr<Vertex> upper = sim.query();
                            AutoPtr<Vertex> lower = sim.query();

                            ff.a = upper->cost;
                            ff.c = lower->cost;

                            for(size_t j=m;j>0;--j)
                            {
                                Cin[j] = (*upper)[j];
                                Cex[j] = (*lower)[j];
                            }
                            sim.free( upper.yield() );
                            sim.free( lower.yield() );
                        }

                        Y_XMLOG(xml, "upper = " << std::setw(15) << real_t(ff.a) );
                        Y_XMLOG(xml, "lower = " << std::setw(15) << real_t(ff.c) );


                        {
                            const real_t offset = cycle-1;
                            const size_t np(1000);
                            for(size_t j=0;j<=np;++j)
                            {
                                const real_t u = real_t(j)/np;
                                fp("%.15g %.15g\n", offset+u,  (real_t( (*this)(u) ) ) );
                            }
                            fp << "\n";
                        }

                        const xreal_t u_opt = Minimize<xreal_t>::Locate(Minimizing::Inside, *this, xx, ff);
                        const xreal_t cost  = (*this)(u_opt);
                        sim.load(cost,cl.species,Cws,SubLevel);
                        Y_XMLOG(xml, "optim = " << std::setw(15) << real_t(cost)  <<  " @" << real_t(u_opt) );
                    }
                    std::cerr << "Ctop=" << Ctop << std::endl;
                    std::cerr << "Cws= " << Cws         << std::endl;
                }

                cl.transfer(Ctop, TopLevel, Cws, SubLevel);

                // project ?

#if 0
                Matrix<int> Nu(apl.size,cl.species.size);
                size_t ii = 0;
                for(const ANode *pn=apl.head;pn;pn=pn->next)
                {
                    (**pn).eq.topology(Nu[++ii],SubLevel);
                }
                std::cerr << "Nu=" << Nu << std::endl;
                std::cerr << "dC = Nu' * inv(Nu*Nu')*Nu*(Cws-Ctop)" << std::endl;
                std::cerr << "[Ctop Ctop+dC Cws]" << std::endl;
#endif

                return sim.head->cost;
            }

            size_t   extract(const Cluster &cl, XMLog &xml)
            {
                Y_XML_SECTION(xml, "Extract");

                //--------------------------------------------------------------
                //
                // reset applicant/ortho family
                //
                //--------------------------------------------------------------

                apl.free();
                qfm.free();

                //--------------------------------------------------------------
                //
                // loop over applicants
                //
                //--------------------------------------------------------------
                const size_t nap = aps.size();
                for(size_t i=1;i<=nap;++i)
                {
                    const Applicant &app = aps[i];
                    if( qfm.wouldAccept( cl.topology[ app.sub() ] ) )
                    {
                        apl << app;
                        qfm.expand();
                        if( apl.size >= dof) break; // base is filled
                    }
                }

                //--------------------------------------------------------------
                //
                // return apl.size <= min(dof,nap)
                //
                //--------------------------------------------------------------
                assert(apl.size <= Min(dof,nap) );
                Y_XMLOG(xml, "#applicant  = " << aps.size());
                Y_XMLOG(xml, "#primary    = " << dof);
                Y_XMLOG(xml, "#family     = " << apl.size);
                if(xml.verbose)
                {
                    for(const ANode *an=apl.head;an;an=an->next)
                    {
                        const Applicant &app = **an;
                        app.display( xml() << "| ", cl.uuid, false) << std::endl;
                    }
                }
                return apl.size;
            }

            size_t compile(const Cluster   & cl,
                           XWritable       & Ctop,
                           const XReadable & Ktop,
                           bool            & repl,
                           XMLog           &  xml)
            {
                Y_XML_SECTION(xml, "Compile");

                // examine all equilibria
                repl        = false;
                size_t iter = 0;
            EXAMINE:
                ++iter;
                Y_XMLOG(xml, "[Examine] [#iter=" << iter << "]");
                aps.free();
                for(const ENode *en=cl.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    const xreal_t      eK = Ktop[ eq.indx[TopLevel] ];
                    const size_t       ii = aps.size()+1;
                    XWritable         &cc = cl.transfer(ceq[ii], SubLevel, Ctop, TopLevel);
                    const Situation    st = afm.seek(cc, SubLevel, eq, eK);

                    switch(st)
                    {
                        case Blocked:
                            Y_XMLOG(xml, "[Blocked] " << eq.name);
                            continue;

                        case Crucial:
                            Y_XMLOG(xml, "[Crucial] " << eq.name);
                            cl.transfer(Ctop, TopLevel, cc, SubLevel);
                            repl = true;
                            goto EXAMINE;

                        case Running:
                            Y_XMLOG(xml, "[Running] " << eq.name);
                            break;
                    }

                    assert(Running==st);
                    const Applicant app(eq,eK,cc, afm.eval(cc, SubLevel, Ctop, TopLevel, eq) );
                    aps << app;
                }

                // sorting and cross-checking
                HeapSort::Call(aps, Applicant::Compare);
                const size_t nmax = aps.size();
                for(size_t i=1;i<=nmax;++i)
                {
                    const Applicant   &lhs = aps[i]; if(xml.verbose)  lhs.display( xml() << "| ", cl.uuid, true) << std::endl;
                    const Equilibrium &lEq = lhs.eq;
                    const char * const lid = lEq.name.c_str();

                    if( !lEq.reac.accounted(Ctop, TopLevel) ) throw Specific::Exception(lid, "missing reactant(s) on top-level");
                    if( !lEq.prod.accounted(Ctop, TopLevel) ) throw Specific::Exception(lid, "missing product(s) on top-level");

                    for(size_t j=1;j<=nmax;++j)
                    {
                        const Applicant   &rhs = aps[j];
                        const char * const rid = rhs.eq.name.c_str();
                        if(!lEq.reac.accounted(rhs.cc,SubLevel)) throw Specific::Exception(lid, "missing reactant(s) in '%s'", rid);
                        if(!lEq.prod.accounted(rhs.cc,SubLevel)) throw Specific::Exception(lid, "missing reactant(s) in '%s'", rid);
                    }
                }

                return nmax;
            }

        };

    }
}



using namespace Yttrium;
using namespace Chemical;





Y_UTEST(solve)
{

    Weasel::Compiler  &weasel = Weasel::Compiler::Instance();
    Library            lib;
    LuaEquilibria      eqs;
    Random::ParkMiller ran;

    for(int i=1;i<argc;++i)
    {
        weasel(lib,eqs,Jive::Module::OpenData(argv[i],argv[i]));
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    bool     verbose = true;
    XMLog    xml(verbose);
    Clusters clusters(eqs,xml);
    const XReadable &K = clusters.K(0);

    XVector C0(lib->size(),0);

    for(size_t iter=0;iter<1;++iter)
    {
        Species::Conc(C0,ran,0.3);

        //C0.ld(0);

        lib(std::cerr << "C0=","\t[",C0,"]");

        for(const Cluster *cl=clusters->head;cl;cl=cl->next)
        {
            Normalizer normalize(*cl);
            normalize.run(*cl, C0, K, xml);
            lib(std::cerr << "C1=","\t[",C0,"]");
        }



    }

    Y_SIZEOF(Normalizer);


}
Y_UDONE()
