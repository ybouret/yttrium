
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

#include "y/associative/flexible-key.hpp"

#include "y/mkl/opt/minimize.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        using namespace MKL;

        //______________________________________________________________________
        //
        //
        //
        //! vertex of a simplex
        //
        //
        //______________________________________________________________________
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

            size_t isub() const noexcept { return eq.indx[SubLevel]; }

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

            const Equilibrium &eq; //!< equilibrium
            const xreal_t      eK; //!< eq.K
            const XReadable   &cc; //!< winning phase space
            const xreal_t      xi; //!< extent leading to cc
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
            typedef LittleEndianKey     KeyType;
            typedef LittleEndianAddress KeyType_;

            typedef ArkPtr<KeyType,Normalizer> Ptr;
            typedef SuffixSet<KeyType,Ptr>     Set;

            explicit Normalizer(const Cluster &cl) :
            rcl(cl),
            lek(rcl),
            nsp(cl.species.size),
            afm(),
            ceq(cl.size,nsp),
            aps(cl.size),
            obj(cl.size),
            Cin(nsp,0),
            Cex(nsp,0),
            Cws(nsp,0),
            dof(cl.Nu.rows),
            bnk(),
            apl(bnk),
            qfm(nsp,cl.size),
            sim(cl.size,nsp),
            tmk(dof),
            Phi(dof)
            {
                for(size_t i=1;i<=dof;++i)
                {
                    Phi.grow(i,nsp);
                }
            }

            virtual ~Normalizer() noexcept
            {
            }

            const KeyType & key() const noexcept { return lek; }

            void run(XWritable       & Ctop,
                     const XReadable &Ktop,
                     XMLog           & xml)
            {
                assert(rcl.size        == ceq.rows);
                assert(rcl.species.size== ceq.cols);
                Y_XML_SECTION_OPT(xml, "Normalizer ", " size='" << rcl.size << "' species='" << rcl.species.size << "'");
                bool    repl = false;
                size_t  nmax = compile(Ctop, Ktop, repl, xml); if(nmax<=0) { Y_XMLOG(xml, "[Jammed!]"); return; }
                xreal_t ham0 = overall(Ctop, repl, xml);
                Y_XMLOG(xml, "found " << real_t(ham0));

                return;

                for(size_t iter=0;iter<1;++iter)
                {
                    nmax = compile(Ctop, Ktop, repl, xml); if(nmax<=0) { Y_XMLOG(xml, "[Jammed!]"); return; }
                    const xreal_t ham1= overall(Ctop, repl, xml);
                    Y_XMLOG(xml, "found " << real_t(ham1));
                }

            }


            xreal_t  operator()(const xreal_t u)
            {
                const xreal_t one = 1;
                const xreal_t v   = one - u;

                for(size_t j=nsp;j>0;--j)
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

            const Cluster &    rcl; //!< reference to cluster
            const KeyType_     lek; //!< key
            const size_t       nsp; //!< number of species
            Aftermath          afm; //!< aftermath
            XMatrix            ceq; //!< storage of phase space
            Applicant::Series  aps; //!< applicant store
            CxxSeries<xreal_t> obj; //!< for objective function
            CxxArray<xreal_t>  Cin; //!< input  phase space
            CxxArray<xreal_t>  Cex; //!< output  phase space
            CxxArray<xreal_t>  Cws; //!< working phase space
            const size_t       dof; //!< primary eqs
            ABank              bnk; //!< pool of applicants
            AList              apl; //!< applicant list
            Orthogonal::Family qfm; //!< orthogonal family
            Simplex            sim; //!< simplex
            CxxSeries<size_t>  tmk; //!< temporary key
            CxxSeries<XMatrix> Phi; //!< matrices

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Normalizer);

            xreal_t objectiveFunction(const XReadable &C, const Level L)
            {
                obj.free();
                for(const ANode *an=apl.head;an;an=an->next)
                    obj << (**an).affinity(afm.xmul,C,L);
                return afm.xadd.normOf(obj);
            }



            //! after a successful compilation
            xreal_t overall(XWritable       & Ctop,
                            const bool        repl,
                            XMLog           & xml)
            {
                assert(aps.size()>0);
                const size_t dims = extract(rcl,xml);
                switch(dims)
                {
                    case 0:  // shouldn't happen
                        throw Specific::Exception("Normalizer", "no equilbrium");

                    case 1: // use single winner
                        return objectiveFunction(rcl.transfer(Ctop,TopLevel,(**apl.head).cc,SubLevel),TopLevel);

                    default: // will improve with simplex
                        break;
                }

                return improve(Ctop,repl,xml);
            }

            xreal_t  improve(XWritable       & Ctop,
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
                        const xreal_t val = sim.load( objectiveFunction(Ctop, TopLevel), rcl.species, Ctop, TopLevel).cost;
                        Y_XMLOG(xml, std::setw(15) << real_t(val) << " @Ctop");
                    }
                    else
                    {
                        Y_XMLOG(xml, "[replaced Ctop]");
                    }

                    for(const ANode *an=apl.head;an;an=an->next)
                    {
                        const Applicant &app = **an;
                        const xreal_t    val = sim.load( objectiveFunction(app.cc, SubLevel), rcl.species, app.cc, SubLevel).cost;
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

                        //Y_XMLOG(xml, "upper = " << std::setw(15) << real_t(ff.a) );
                        //Y_XMLOG(xml, "lower = " << std::setw(15) << real_t(ff.c) );


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
                        sim.load(cost,rcl.species,Cws,SubLevel);
                        Y_XMLOG(xml, "optim = " << std::setw(15) << real_t(cost)  <<  " @" << real_t(u_opt) );
                    }
                    std::cerr << "Ctop=" << Ctop << std::endl;
                    std::cerr << "Cws= " << Cws         << std::endl;
                }

                rcl.transfer(Ctop, TopLevel, Cws, SubLevel);

                // project ?

#if 1
                Matrix<int> Nu(apl.size,rcl.species.size);
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

            static SignType CompareBasis(const ANode * const lhs, const ANode * const rhs) noexcept
            {
                return Sign::Of( (**lhs).eq.indx[TopLevel], (**rhs).eq.indx[TopLevel] );
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
                    if( qfm.wouldAccept( cl.topology[ app.isub() ] ) )
                    {
                        apl << app;
                        qfm.expand();
                        if( apl.size >= dof) break; // base is filled
                    }
                }

                //--------------------------------------------------------------
                //
                // found the base: order by equilibria top-level
                // to build local topology
                //
                //--------------------------------------------------------------
                MergeSort::Call(apl,CompareBasis);




                //--------------------------------------------------------------
                //
                // build temporary key from ordered
                //
                //--------------------------------------------------------------
                assert(apl.size <= Min(dof,nap) );
                Y_XMLOG(xml, "#applicant  = " << aps.size());
                Y_XMLOG(xml, "#primary    = " << dof);
                Y_XMLOG(xml, "#family     = " << apl.size);
                tmk.free();

                for(const ANode *an=apl.head;an;an=an->next)
                {
                    const Applicant &app = **an;
                    tmk << app.isub();
                    if(xml.verbose)
                    {
                        app.display( xml() << "| ", cl.uuid, false) << std::endl;
                    }
                }

                Y_XMLOG(xml, "Key=" << tmk);

                //--------------------------------------------------------------
                //
                // return apl.size <= min(dof,nap)
                //
                //--------------------------------------------------------------
                return apl.size;
            }

            size_t compile(XWritable       & Ctop,
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
                for(const ENode *en=rcl.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    const xreal_t      eK = Ktop[ eq.indx[TopLevel] ];
                    const size_t       ii = aps.size()+1;
                    XWritable         &cc = rcl.transfer(ceq[ii], SubLevel, Ctop, TopLevel);
                    const Situation    st = afm.seek(cc, SubLevel, eq, eK);

                    switch(st)
                    {
                        case Blocked:
                            Y_XMLOG(xml, "[Blocked] " << eq.name);
                            continue;

                        case Crucial:
                            Y_XMLOG(xml, "[Crucial] " << eq.name);
                            rcl.transfer(Ctop, TopLevel, cc, SubLevel);
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
                    const Applicant   &lhs = aps[i]; if(xml.verbose)  lhs.display( xml() << "| ", rcl.uuid, true) << std::endl;
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


        class Solver : public Proxy<const Normalizer::Set>
        {
        public:
            static const char * const CallSign;
            typedef Normalizer::Set::Iterator      Iterator;
            typedef Normalizer::Set::ConstIterator ConstIterator;

            explicit Solver(const SharedClusters &usr) :
            cls(usr),
            ndb()
            {
                for(const Cluster *cl=(*cls)->head;cl;cl=cl->next)
                {
                    const Normalizer::Ptr ptr = new Normalizer(*cl);
                    if(!ndb.insert(ptr)) throw Specific::Exception(CallSign, "corrupted cluster address");
                }
            }

            virtual ~Solver() noexcept { }

            void run(XWritable       & Ctop,
                     const XReadable & Ktop,
                     XMLog           &  xml)
            {
                size_t n = ndb.size();
                for(Iterator it=ndb.begin();n>0;--n,++it)
                {
                    Normalizer &normalizer = **it; assert( (*cls)->owns( & normalizer.rcl ) );
                    normalizer.run(Ctop,Ktop,xml);
                }
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);
            SharedClusters  cls;
            Normalizer::Set ndb;

            virtual ConstInterface & surrogate() const noexcept
            {
                return ndb;
            }
        };


        const char * const Solver:: CallSign = "Chemical::Solver";
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

    bool           verbose = true;
    XMLog          xml(verbose);
    SharedClusters   cls = new Clusters(eqs,xml);
    const XReadable &K   = cls->K(0);

    
    XVector C0(lib->size(),0);

    Solver  solver(cls);

    for(size_t iter=0;iter<1;++iter)
    {
        Species::Conc(C0,ran,0.3);

        //C0.ld(0);

        lib(std::cerr << "C0=","\t[",C0,"]");
        solver.run(C0,K, xml);
        lib(std::cerr << "C1=","\t[",C0,"]");



    }

    Y_SIZEOF(Normalizer);
    Y_SIZEOF(Clusters);


}
Y_UDONE()
