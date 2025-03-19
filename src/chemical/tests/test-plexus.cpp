
#include "y/chemical/plexus/clusters.hpp"
#include "y/chemical/weasel.hpp"
#include "y/chemical/reactive/aftermath.hpp"

#include "y/data/small/heavy/list/coop.hpp"
#include "y/data/small/heavy/list/solo.hpp"

#include "y/sequence/vector.hpp"

#include "y/utest/run.hpp"

#include "y/random/mt19937.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        typedef Small::SoloHeavyList<Outcome> OutList;
        typedef OutList::NodeType             OutNode;


        class Reactor
        {
        public:

            explicit Reactor(const Cluster &persistentCluster) :
            cluster(persistentCluster),
            running(),
            Ceq(cluster->equilibria->size,cluster->species->size),
            Cini(cluster->species->size),
            Cend(cluster->species->size),
            Ctry(cluster->species->size)
            {
                running.proxy->reserve(Ceq.rows);
            }

            virtual ~Reactor() noexcept
            {
            }

            //! solve topLevel
            void operator()(XMLog &xml, XWritable &C0, const XReadable &K0);

            static SignType ByDecreasingAX(const OutNode * const lhs,
                                           const OutNode * const rhs) noexcept
            {
                return Sign::Of( (**rhs).ax, (**lhs).ax );
            }

            xreal_t score(const XReadable &C,
                          const Level      L);


            const Cluster & cluster;  //!< persistent cluster
            Aftermath       solve1D;  //!< computing 1D solution
            XAdd            x_score;  //!< helper to compute score
            OutList         running;  //!< running equilibria
            XMatrix         Ceq;      //!< workspace to store 1D solution
            XArray          Cini;     //!< initial state
            XArray          Cend;     //!< final   state
            XArray          Ctry;     //!< trial   state

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
            static bool IsRunning(const Outcome &out) noexcept
            {
                assert(Blocked!=out.st);
                return Running == out.st;
            }

            void initialize(XMLog &           xml,
                            XWritable &       C0,
                            const XReadable & K0);

        };


        xreal_t Reactor:: score(const XReadable &C,
                                const Level      L)
        {
            assert(running.size>0);
            x_score.free();
            for(const OutNode *node=running.head;node;node=node->next)
            {
                const xreal_t aff = (**node).affinity(solve1D.xadd,C,L);
                x_score << Squared(aff);
            }
            const xreal_t numer = x_score.sum();
            const xreal_t denom = running.size;
            const xreal_t ratio = numer/denom;
            return ratio.sqrt();
        }

        void Reactor:: initialize(XMLog &           xml,
                                  XWritable &       C0,
                                  const XReadable & K0)
        {
            size_t buildActive = 0;
        BUILD_ACTIVE:
            ++buildActive;
            //Y_XML_COMMENT(xml,"build active cycle#" << buildActive);
            Y_XML_SECTION_OPT(xml,"BuildActive", "cycle="<<buildActive);
            {
                running.free();
                bool emergency = false;
                for(const ENode *en=cluster->equilibria->head;en;en=en->next)
                {
                    const Components &eq  = **en;
                    const xreal_t     eK  = eq(K0,TopLevel);
                    XWritable        &cc  = cluster.gather(Ceq[running.size+1],C0);
                    const Outcome     out = solve1D(eq,eK,cc,SubLevel,C0,TopLevel);
                    switch(out.st)
                    {
                        case Blocked:
                            Y_XMLOG(xml,out);
                            continue;

                        case Crucial:
                            Y_XMLOG(xml,out);
                            emergency = true;
                            running << out;
                            continue;

                        case Running:
                            if(emergency) continue; // discard in case or emergency
                            Y_XMLOG(xml,out);
                            running << out;
                            continue;
                    }
                }
                if(emergency)
                {
                    MergeSort::Call(running.removeIf(IsRunning),ByDecreasingAX);
                    const Outcome &worst = **running.head;
                    Y_XML_COMMENT(xml,"using crucial " << worst.eq.name);
                    cluster.expand(C0,worst.cc);
                    if(xml.verbose)
                    {
                        cluster.show(*xml << "C0=", TopLevel,"\t[", C0, "]", xreal_t::ToString) << std::endl;
                    }
                    goto BUILD_ACTIVE;
                }
            }
            Y_XML_COMMENT(xml, "|active| = " << running.size << "/" << cluster->equilibria->size);
            cluster.gather(Cini,C0);
        }

        void Reactor:: operator()(XMLog &           xml,
                                  XWritable &       C0,
                                  const XReadable & K0)
        {
            Y_XML_SECTION(xml, "Reactor");
            initialize(xml,C0,K0);
            if(running.size<=0) {
                Y_XML_COMMENT(xml, "all blocked");
                return;
            }

            std::cerr << "score: " << double(score(Cini,SubLevel)) << " / " << double(score(C0,TopLevel)) << std::endl;


        }

    }

}

using namespace Yttrium;
using namespace Chemical;



Y_UTEST(plexus)
{

    Random::MT19937 ran;
    Weasel &weasel = Weasel::Instance();
    weasel << "function f(t) return 1.1 end";

    Library    lib;
    Equilibria eqs;

    for(int iarg=1;iarg<argc;++iarg)
    {
        const String data = argv[iarg];
        weasel(lib,eqs,Lingo::Module::Open(data,data) );
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    bool verbose = true;

    XMLog          xml(verbose);
    Clusters       cls(xml,eqs,0.0);

    std::cerr << "lib=" << lib << std::endl;
    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        std::cerr << *cl << std::endl;
    }

    const size_t m = lib->size();
    XVector      C0(m,0);
    XVector      C(m,0);

    if(false)
    {
        Library::Concentrations(C0,ran);
        lib.show(std::cerr << "C0=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;

        Aftermath am;
        for(Equilibria::ConstIterator it=eqs->begin();it!=eqs->end();++it)
        {
            const Equilibrium &eq = **it;
            const xreal_t      eK = eq(cls.K,TopLevel);
            std::cerr << "solving " << eq.name << std::endl;
            lib.Concentrations(C0,ran);
            C.ld(C0);
            am(eq,eK,C,TopLevel,C0,TopLevel);
        }
    }

    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        Library::Concentrations(C0,ran,0.5);
        Reactor reactor(*cl);
        reactor(xml,C0,cls.K);
    }




    Y_SIZEOF(Cluster);
    Y_SIZEOF(ClusterContent);
    Y_SIZEOF(Conservation::Law);
    Y_SIZEOF(Conservation::Laws);

}
Y_UDONE()

