#include "y/chemical/plexus/clusters.hpp"
#include "y/chemical/weasel/compiler.hpp"
#include "y/random/park-miller.hpp"
#include "y/utest/run.hpp"

#include "y/data/list/ordered.hpp"
#include "y/data/pool/cxx.hpp"
#include "y/chemical/reactive/aftermath.hpp"
#include "y/container/cxx/series.hpp"
#include "y/sort/heap.hpp"

#include "y/orthogonal/family.hpp"
#include "y/associative/hash/set.hpp"

using namespace Yttrium;
using namespace Chemical;


namespace Yttrium
{
    namespace Chemical
    {

#if 0
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


            explicit Vertex(const size_t maxSpecies) :
            XArray(maxSpecies), cost(0), next(0), prev(0)
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
            explicit Simplex(size_t maxEqs, const size_t maxSpecies) :
            Vertices(),
            pool()
            {
                ++maxEqs;
                while(maxEqs-- > 0) pool.store( new Vertex(maxSpecies) );
            }

            void free() noexcept {
                while(size>0) pool.store( popTail() )->clear();
            }

            Vertex::Pool pool;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Simplex);
        };
#endif


        class Prospect
        {
        public:
            Prospect(const Equilibrium & _eq,
                     const xreal_t       _eK,
                     const XReadable    &_cc,
                     const xreal_t       _xi,
                     const xreal_t       _ks) noexcept :
            eq(_eq),
            eK(_eK),
            cc(_cc),
            xi(_xi),
            ks(_ks)
            {

            }

            Prospect(const Prospect &_) noexcept :
            eq(_.eq),
            eK(_.eK),
            cc(_.cc),
            xi(_.xi),
            ks(_.ks)
            {

            }

            inline size_t sub() const noexcept { return eq.indx[SubLevel]; }

            inline xreal_t objectiveFunction(const XReadable &C,
                                             const Level      L,
                                             XMul &           X) const
            {
                const xreal_t value = ks * eq.massAction(eK, X, C, L);
                return value.abs();
            }

            const Equilibrium &eq;
            const xreal_t      eK;
            const XReadable   &cc;
            const xreal_t      xi;
            const xreal_t      ks;
        private:
            Y_DISABLE_ASSIGN(Prospect);
        };

        typedef Small::CoopLightList<const Prospect> PList;
        typedef PList::NodeType                      PNode;
        typedef PList::ProxyType                     PBank;

        class QBuilder : public Quantized, public Proxy<const PList>
        {
        public:
            typedef ArkPtr<size_t,QBuilder> Ptr;
            typedef SuffixSet<size_t,Ptr>   Set;

            explicit QBuilder(const size_t primary,
                              const size_t species,
                              const PBank &probank) :
            list(probank),
            qfam(species,primary)
            {
            }

            virtual ~QBuilder() noexcept
            {
            }

            const size_t & key() const noexcept { return qfam.dimensions; }

            void init() noexcept
            {
                list.free();
                qfam.free();
            }

            bool grow(const Prospect &pro, const Matrix<int> &topo)
            {
                if( !qfam.wouldAccept(topo[pro.sub()])) return false;
                list << pro;
                qfam.expand();
                return true;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(QBuilder);
            PList              list;
            Orthogonal::Family qfam;

            virtual ConstInterface & surrogate() const noexcept { return list; }
        };


        class Solver
        {
        public:
            explicit Solver(const Clusters &cls) :
            afm(),
            ceq(cls.maxEPC,cls.maxSPC),
            phi(cls.maxSPC),
            obj(cls.maxEPC),
            pps(cls.maxEPC),
            bnk()
            {
            }

            void run(const Cluster   &cl,
                     XWritable       &Ctop,
                     const XReadable &Ktop,
                     XMLog           &xml)
            {
                const xreal_t zero;
                const xreal_t mOne(-1);

                const size_t dof = cl.Nu.rows; // primary equilibria

                {
                    Y_XML_SECTION(xml, "Scan");
                SCAN:
                    Y_XMLOG(xml, "[Examine] @" << Ctop);
                    obj.free();
                    pps.free();
                    for(const ENode *en=cl.head;en;en=en->next)
                    {
                        const Equilibrium &eq = **en;
                        const xreal_t      eK = Ktop[ eq.indx[TopLevel] ];
                        const size_t       ii = obj.size() + 1;
                        XWritable         &cc = ceq[ii];
                        const Situation    id = afm.seek( cl.transfer(cc,SubLevel,Ctop,TopLevel), SubLevel, eq, eK);
                        switch(id)
                        {
                            case Blocked: Y_XMLOG(xml, "[Blocked] " << eq); continue;
                            case Running: Y_XMLOG(xml, "[Running] " << eq); break;
                            case Crucial:
                                Y_XMLOG(xml, "[Crucial] " << eq << " @" << cc);
                                cl.transfer(Ctop, TopLevel, cc, SubLevel);
                                goto SCAN; // abort and restart
                        }
                        assert(Running==id);


                        // store new running prospect
                        eq.drvsMassAction(eK, phi, SubLevel, cc, SubLevel, afm.xmul);
                        const xreal_t  slope = eq.dot(phi, SubLevel, afm.xadd);
                        if(slope>=zero) throw Exception("invalid slope");
                        const Prospect pro(eq, eK, cc, afm.eval(cc,SubLevel,Ctop,TopLevel,eq), mOne/slope );
                        pps << pro;
                        obj << pro.objectiveFunction(Ctop, TopLevel, afm.xmul);
                    }

                    if( pps.size() <= 0 ) {
                        Y_XMLOG(xml, "[Jammed!]");
                        return;
                    }

                    {
                        Y_XML_SECTION_OPT(xml, "Output", " running='" << pps.size() << "' clusterSize='" << cl.size << "'" );
                        HeapSort::Call(obj, Comparison::Decreasing<xreal_t>, pps);
                        if(xml.verbose)
                        {
                            for(size_t i=1;i<=pps.size();++i)
                            {
                                const Prospect &pro = pps[i];
                                cl.uuid.pad(xml() << pro.eq,pro.eq) << " : ";
                                *xml << std::setw(15) << real_t(obj[i]);
                                pro.eq.displayCompact(*xml << " @", pro.cc, SubLevel);
                                *xml << " ks=" << real_t(pro.ks);
                                *xml << std::endl;
                            }
                        }
                    }
                }



                {
                    Y_XML_SECTION(xml, "Base");
                    QBuilder     base(dof,cl.species.size,bnk);
                    const size_t npm = pps.size();

                    base.init();
                    for(size_t i=1;i<=npm;++i)
                    {
                        const Prospect &pro = pps[i];
                        if(base.grow(pro, cl.topology) && base->size >= dof) break;
                    }

                    Y_XMLOG(xml, "#dof = " << std::setw(10) << dof);
                    Y_XMLOG(xml, "#pro = " << std::setw(10) << npm);
                    Y_XMLOG(xml, "#vec = " << std::setw(10) << base->size);

                    for(const PNode *pn=base->head;pn;pn=pn->next)
                    {
                        Y_XMLOG(xml, " (+) " << (**pn).eq);
                    }
                }

            }

            Aftermath           afm;
            XMatrix             ceq;
            XArray              phi;
            CxxSeries<xreal_t>  obj;
            CxxSeries<Prospect> pps;
            PBank               bnk;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);
        };


    }

}


Y_UTEST(crack)
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
    Species::Conc(C0,ran,0.3);

    C0.ld(0);

    lib(std::cerr << "C0=","\t[",C0,"]");

    Solver solver(clusters);
    for(const Cluster *cl = clusters->head; cl; cl=cl->next)
    {
        solver.run(*cl, C0, K, xml);
    }



}
Y_UDONE()

