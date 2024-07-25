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

        class Applicant
        {
        public:
            Applicant(const Equilibrium & _eq,
                      const xreal_t       _eK,
                      const xreal_t       _xi,
                      const XReadable &   _cc,
                      const xreal_t       _ks) noexcept :
            eq(_eq),
            eK(_eK),
            xi(_xi),
            cc(_cc),
            ks(_ks)
            {
            }

            ~Applicant() noexcept {}

            Applicant(const Applicant &_) noexcept :
            eq(_.eq),
            eK(_.eK),
            xi(_.xi),
            cc(_.cc),
            ks(_.ks)
            {
            }

            xreal_t operator()(const XReadable &C, const Level L, XMul &xmul) const
            {
                return (ks * eq.massAction(eK,xmul,C,L)).abs();
            }


            const Equilibrium &eq;
            const xreal_t      eK;
            const xreal_t      xi;
            const XReadable   &cc;
            const xreal_t      ks;

        private:
            Y_DISABLE_ASSIGN(Applicant);
        };


        typedef Small::CoopLightList<const Applicant> AList; //!< alias
        typedef AList::ProxyType                      ABank; //!< alias
        typedef AList::NodeType                       ANode; //!< alias

        class BaseBuilder : public Quantized, public Proxy<const AList>
        {
        public:
            explicit BaseBuilder(const size_t primary,
                                 const size_t  species,
                                 const ABank & appbank) :
            list(appbank),
            qfam(species,primary)
            {
            }
            

            virtual ~BaseBuilder() noexcept {}

            void init() noexcept { list.free(); qfam.free(); }
            bool keep(const Applicant &app, const Matrix<int> &topo)
            {
                if( !qfam.wouldAccept(topo[app.eq.indx[SubLevel]])) 
                    return false;

                qfam.expand();
                list << app;
                return true;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(BaseBuilder);
            AList              list;
            Orthogonal::Family qfam;

            ConstInterface & surrogate() const noexcept { return list; }
        };

        class Blender
        {
        public:
            explicit Blender(const Clusters &cls) :
            afm(),
            Ceq(cls.maxEPC,cls.maxSPC),
            phi(cls.maxSPC),
            asr(cls.maxEPC),
            obj(cls.maxEPC)
            {
            }

            virtual ~Blender() noexcept
            {
            }

            void process(const Cluster   &cl,
                         XWritable       &C0,
                         const XReadable &K,
                         XMLog           &xml)
            {

                const xreal_t zero;
                const xreal_t mOne(-1);

                Y_XML_SECTION(xml, "Blender::Process::Cluster");
                {
                    Y_XML_SECTION(xml,"Scan");
                    asr.free();
                    obj.free();
                    for(const ENode *en=cl.head;en;en=en->next)
                    {
                        const Equilibrium &eq = **en;
                        const xreal_t      eK = K[eq.indx[TopLevel]];
                        const size_t       ii = asr.size()+1;
                        XWritable         &Ci = Ceq[ii];
                        cl.transfer(Ci, SubLevel, C0, TopLevel);
                        if( afm.solve(Ci,SubLevel, eq, eK) )
                        {
                            eq.drvsMassAction(eK, phi, SubLevel, Ci, SubLevel, afm.xmul);
                            const xreal_t   xi = afm.eval(Ci, SubLevel, C0, TopLevel, eq);
                            const xreal_t   sl = eq.dot(phi, SubLevel, afm.xadd);
                            if(sl>=zero)
                            {
                                throw Exception("positive slope");
                            }

                            const Applicant app(eq,eK,xi,Ci,mOne/sl);
                            const xreal_t   ham = app(C0,TopLevel,afm.xmul);
                            asr << app;
                            obj << ham;
                            //std::cerr << eq << " @" << real_t(xi) << " => " << real_t(ham) << std::endl;
                        }
                    }

                    const size_t napp = asr.size();
                    if(napp<=0)
                    {
                        Y_XMLOG(xml, "<no active equilbrium>");
                        return;
                    }

                    HeapSort::Call(obj, Comparison::Decreasing<xreal_t>, asr);
                    assert(asr.size()==obj.size());

                    if(xml.verbose)
                    {
                        for(size_t i=1;i<=napp;++i)
                        {
                            const Applicant &app = asr[i];
                            cl.uuid.pad(xml() << app.eq,app.eq) << " : ";
                            *xml << std::setw(15) << real_t(obj[i]);
                            *xml << " @" << app.cc;
                            *xml << std::endl;
                        }
                    }
                }

                {
                    Y_XML_SECTION(xml, "Base");

                    BaseBuilder  basis(cl.size,cl.species.size,bnk);
                    const size_t nbMax = cl.Nu.rows; // max base size
                    const size_t napps = asr.size();

                    basis.init();
                    for(size_t i=1;i<=napps;++i)
                    {
                        if(basis.keep(asr[i],cl.topology) && basis->size>=nbMax) break;
                    }

                    if(xml.verbose)
                    {
                        for(const ANode *an=basis->head;an;an=an->next)
                        {
                            xml() << (**an).eq << std::endl;
                        }
                    }


                }

            }



            Aftermath            afm;
            XMatrix              Ceq;
            XArray               phi;
            ABank                bnk;
            CxxSeries<Applicant> asr;
            CxxSeries<xreal_t>   obj;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blender);
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

    lib(std::cerr << "C0=","\t[",C0,"]");


    Blender blender(clusters);
    for(const Cluster *cl = clusters->head; cl; cl=cl->next)
    {
        blender.process(*cl, C0, K, xml);
    }


    Y_SIZEOF(Applicant);

}
Y_UDONE()

